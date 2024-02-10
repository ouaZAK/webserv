#include <iostream>
#include <unistd.h>
#include<fstream>

int main(int ac, char **av)
{
	if (ac != 2)
		return 0;
	int pip[2],fd1,fd2,fd3;
	std::ifstream file(av[1]);
	if (!file.is_open())
		std::cout << "not open\n";
	std::string str, fullstr;
	while (file >> str)
	{
		fullstr.append(str);
		fullstr += "\n";
	}
	pipe(pip);
	pid_t fd = fork();
	if (!fd)
	{
		char buff[3000];
		close(fd1);
		close(fd2);
		close(fd3);
		int fd = read(pip[0], buff, sizeof(buff));
		buff[fd] = '\0';
		close(pip[0]);
		std::cout << "from child : \n" << buff << '\n';
		write(pip[1], "executed in child\n", 19);
		close(pip[1]);
		exit(0);
	}
	std::cout << "par file is : \n" << fullstr << '\n';
	std::cout << "send from parent " << '\n';
	write(pip[1], &fullstr, fullstr.size());
	sleep(1);
	char buffer[3000];
	read(pip[0], buffer, sizeof(buffer));
	close(pip[1]);
	close(pip[0]);
	std::cout << "response from child: \n" << buffer << '\n';

	return (0);
}