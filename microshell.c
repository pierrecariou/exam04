#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define DEFAULT 0
#define PIPE 1

typedef struct s_cmd	t_cmd;

struct					s_cmd
{
	char	**args;
	size_t	args_length;
	int		type;
	t_cmd	*next;
};

size_t	ft_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		++len;
	return len;
}

void	error(char *str)
{
	write(2, str, ft_strlen(str));
}

void	fatal()
{
	error("error: fatal\n");
	exit(1);
}

void	**ft_tabdup(void **old, size_t len)
{
	void	**new;

	if (!(new = malloc(sizeof(*old) * (len + 1))))
		return 0;
	new[len] = 0;
	while (len--)
		new[len] = old[len];
	return new;
}

t_cmd	*get(char ***args)
{
	t_cmd	*cmd;
	size_t	len;

	if (!(cmd = malloc(sizeof(t_cmd))))
		fatal();
	cmd->next = 0;
	len = 0;
	while ((*args)[len] && strcmp((*args)[len], ";") && strcmp((*args)[len], "|"))
		++len;
	cmd->args_length = len;
	if (!(cmd->args = (char **)ft_tabdup((void **)*args, cmd->args_length)))
		fatal();
	cmd->type = DEFAULT;
	if ((*args)[len])
	{
		if (*(*args)[len] == '|')
			cmd->type = PIPE;
		++len;
	}
	*args += len;
	return cmd;
}

void	cd(t_cmd cmd)
{
	if (cmd.args_length != 2)
		error("error: cd: bad arguments\n");
	else if (chdir(cmd.args[1]) == -1)
	{
		error("error: cd: cannot change directory to ");
		error(cmd.args[1]);
		error("\n");
	}
}

void	execute(t_cmd cmd, char **env)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		fatal();
	if (pid == 0)
	{
		execve(cmd.args[0], cmd.args, env);
		error("error: cannot execute ");
		error(cmd.args[0]);
		error("\n");
		exit(127);
	}
	waitpid(pid, 0, 0);
}

void	dispatch(t_cmd cmd, char **env)
{
	if (strcmp(cmd.args[0], "cd") == 0)
		cd(cmd);
	else
		execute(cmd, env);
}

void	dispatch_fork(t_cmd cmd, char **env)
{
	int	pipefd[2];
	int	pid;

	if (pipe(pipefd) == -1)
		fatal();
	pid = fork();
	if (pid < 0)
		fatal();
	dup2(pipefd[!pid], !pid);
	if (pid == 0)
	{
		dispatch(cmd, env);
		exit(0);
	}
	waitpid(pid, 0, 0);
	close(pipefd[0]);
	close(pipefd[1]);
}

int		main(int argc, char **argv, char **env)
{
	t_cmd	*cmd;
	t_cmd	*tmp;

	if (argc == 1)
		return 0;
	++argv;
	cmd = get(&argv);
	tmp = cmd;
	while (argv[0])
	{
		tmp->next = get(&argv);
		tmp = tmp->next;
	}
	while (cmd)
	{
		if (cmd->args_length)
		{
			if (cmd->type == PIPE)
				dispatch_fork(*cmd, env);
			else
				dispatch(*cmd, env);
		}
		tmp = cmd->next;
		free(cmd->args);
		free(cmd);
		cmd = tmp;
	}
	return 0;
}
