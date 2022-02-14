#!/bin/bash

test_line () {
	printf "\e[0m"
	touch 1 2
	./our_microshell $@ > 1 2>&1
	./microshell $@ > 2 2>&1
	if diff 1 2
	then
		printf "\e[32mOk\n"
	else
		printf "\e[31mFail\n"
		echo $@
	fi
	rm -f 1 2
}

printf "\e[34mCompile\n"
gcc -g -Wall -Werror -Wextra our_microshell.c -o our_microshell
gcc -g -Wall -Werror -Wextra microshell.c -o microshell
printf "\e[34mTest\n"
rm -f out.res leaks.res out
test_line /bin/ls
test_line /bin/cat microshell.c
test_line /bin/ls microshell.c
test_line /bin/ls salut
test_line ";"
test_line ";" ";"
test_line ";" ";" /bin/echo OK
test_line ";" ";" /bin/echo OK ";"
test_line ";" ";" /bin/echo OK ";" ";"
test_line ";" ";" /bin/echo OK ";" ";" ";" /bin/echo OK
test_line /bin/ls "|" /bin/grep microshell
test_line /bin/ls "|" /bin/grep microshell "|" /bin/grep micro
test_line /bin/ls "|" /bin/grep microshell "|" /bin/grep micro "|" /bin/grep shell "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro
test_line /bin/ls "|" /bin/grep microshell "|" /bin/grep micro "|" /bin/grep shell "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep micro "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell "|" /bin/grep shell
test_line /bin/ls ewqew "|" /bin/grep micro "|" /bin/cat -n ";" /bin/echo dernier ";" /bin/echo
test_line /bin/ls "|" /bin/grep micro "|" /bin/cat -n ";" /bin/echo dernier ";" /bin/echo ftest ";"
test_line /bin/echo ftest ";" /bin/echo ftewerwerwerst ";" /bin/echo werwerwer ";" /bin/echo qweqweqweqew ";" /bin/echo qwewqeqrtregrfyukui ";"
test_line /bin/ls ftest ";" /bin/ls ";" /bin/ls werwer ";" /bin/ls microshell.c ";" /bin/ls subject.fr.txt ";"
test_line /bin/ls "|" /bin/grep micro ";" /bin/ls "|" /bin/grep micro ";" /bin/ls "|" /bin/grep micro ";" /bin/ls "|" /bin/grep micro ";"
test_line /bin/cat subject.fr.txt "|" /bin/grep a "|" /bin/grep b ";" /bin/cat subject.fr.txt ";"
test_line /bin/cat subject.fr.txt "|" /bin/grep a "|" /bin/grep w ";" /bin/cat subject.fr.txt ";"
test_line /bin/cat subject.fr.txt "|" /bin/grep a "|" /bin/grep w ";" /bin/cat subject.fr.txt
test_line /bin/cat subject.fr.txt ";" /bin/cat subject.fr.txt "|" /bin/grep a "|" /bin/grep b "|" /bin/grep z ";" /bin/cat subject.fr.txt
test_line ";" /bin/cat subject.fr.txt ";" /bin/cat subject.fr.txt "|" /bin/grep a "|" /bin/grep b "|" /bin/grep z ";" /bin/cat subject.fr.txt
test_line blah "|" /bin/echo OK
test_line blah "|" /bin/echo OK ";"
printf "\e[34mDone\e[0m\n"
