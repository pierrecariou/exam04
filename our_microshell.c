#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int fd[20000][2];
int count;

int		ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

void	print(char *str, char *str1)
{
	write(2, str, ft_strlen(str));
	write(2, str1, ft_strlen(str1));
	write(2, "\n", 1);
}

void	error()
{
	write(2, "error: fatal\n", 13);
	exit(1);
}

void	free_cmds(char **cmds)
{
	int i = 0;
	while (cmds[i])
		free(cmds[i++]);
	free(cmds);
	cmds = NULL;
}

void	fill_cmd(char **argv, char **cmds, int b, int e)
{
	int i = 0;
	int k = 0;

	while (b < e) {
		if (!(cmds[i] = malloc(sizeof(char) * (ft_strlen(argv[b]) + 1))))
			error();
		while (argv[b][k]) {
			cmds[i][k] = argv[b][k];
			k++;
		}
		cmds[i][k] = 0;
		b++;
		i++;
		k = 0;
	}
}

void	exec(int l, int l_b, char **cmds, char **envp)
{
	if (l_b) {
		if (dup2(fd[count - 1][0], STDIN_FILENO) == -1)
			error();
		close(fd[count - 1][0]);
		close(fd[count - 1][1]);
	}
	if (l) {
		if (dup2(fd[count][1], STDOUT_FILENO) == -1)
			error();
		close(fd[count][0]);
		close(fd[count][1]);
	}
	execve(cmds[0] , cmds, envp);
	print("error: cannot execute ", cmds[0]);
}

void	exec_cmds(char **cmds, char **envp, char *limit, char *limit_b)
{
	int l = (limit && !strcmp(limit, "|")) ? 1 : 0;
	int l_b = (limit_b && !strcmp(limit_b, "|")) ? 1 : 0;

	if (l) {
		if (pipe(fd[count]) == -1)
			error();
	}
	pid_t  pid = fork();

	if (pid == -1)
		error();
	if (pid == 0) {
		exec(l, l_b, cmds, envp);
		exit(0);
	}
	if (l_b) {
		close(fd[count - 1][0]);
		close(fd[count - 1][1]);
	}
	if (waitpid(pid, NULL, 0) == -1)
		error();
	count++;
}

void	exec_cd(char **cmds, int count)
{
	if (count != 2)
		print("error: cd: bad arguments", "");
	else if (chdir(cmds[1]) != 0)
		print("error: cd: cannot change ", cmds[1]);
}

void	read_cmds(char **argv, int i, char **envp)
{
	int b;
	char **cmds;

	while (argv[i] && !strcmp(argv[i], ";"))
		i++;
	if (!argv[i])
		return ;
	b = i;
	while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
		i++;
	if (!(cmds = malloc(sizeof(char *) * ((i - b) + 1))))
		error();
	cmds[i - b] = NULL;
	fill_cmd(argv, cmds, b, i);
	if (strcmp(cmds[0], "cd"))
		exec_cmds(cmds, envp, argv[i], argv[b - 1]);
	else
		exec_cd(cmds, i - b);
	free_cmds(cmds);
	while (argv[i] && (!strcmp(argv[i], ";") || !strcmp(argv[i], "|")))
		i++;
	if (argv[i])
		return read_cmds(argv, i, envp);
	return ;
}

int		main(int argc, char **argv, char **envp)
{
	if (argc == 1)
		return (1);
	count = 0;
	read_cmds(argv, 1, envp);
	return (0);
}
