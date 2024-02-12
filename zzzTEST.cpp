#include <iostream>
#include <unistd.h>
#include<fstream>

int main(int ac, char **av)
{
	if (ac != 2)
		return 0;
	int PtoC[2],CtoP[2],fd1,fd2,fd3;
	// std::ifstream file(av[1]);
	// if (!file.is_open())
	// 	std::cout << "not open\n";
	std::string str, fullstr, line;
	fullstr = av[1];
	// while (std::getline(file, line))
	// {
	// 	fullstr.append(line);
	// 	fullstr += "\n";
	// }
	pipe(PtoC);
	pipe(CtoP);
	pid_t fd = fork();
	if (!fd)
	{
		char buff[3000];
		close(fd1);
		close(fd2);
		close(fd3);

		close(PtoC[1]);
		close(CtoP[0]);
		int fd = read(PtoC[0], buff, sizeof(buff));
		if (fd == -1)
			std::cout << "error\n";
		buff[fd] = '\0';
		
		std::string str;
		str = buff;

		std::cout << "from child : \n[" << buff << "]\n" << '\n';
		dup2(CtoP[1], 1);
		// write(CtoP[1], "executed in child\n", 19);
		close(PtoC[0]);
		char *const argv[] = {"/bin/bash", buff, nullptr};
		if (execv("/bin/bash", argv) == -1)
			std::cout << "error exec\n";
		close(CtoP[1]);
		exit(0);
	}
	// std::cout << "par file is : \n" << fullstr << '\n';
	std::cout << "send from parent " << '\n';

	close(PtoC[0]);
	close(CtoP[1]);

	write(PtoC[1], fullstr.c_str(), fullstr.length());
	char buffer[3000];
	int xd = read(CtoP[0], buffer, 3000);
	buffer[xd] = '\0';
	close(PtoC[1]);
	close(CtoP[0]);
	std::cout << "response from child: \n" << buffer << " " << strlen(buffer) << '\n';

	return (0);
}