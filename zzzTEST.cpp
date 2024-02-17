#include <iostream>
#include <unistd.h>
#include<fstream>

// int main(int ac, char **av)
// {
// 	if (ac != 2)
// 		return 0;
// 	int PtoC[2],CtoP[2],fd1,fd2,fd3;
// 	// std::ifstream file(av[1]);
// 	// if (!file.is_open())
// 	// 	std::cout << "not open\n";
// 	std::string str, fullstr, line;
// 	fullstr = av[1];
// 	// while (std::getline(file, line))
// 	// {
// 	// 	fullstr.append(line);
// 	// 	fullstr += "\n";
// 	// }
// 	pipe(PtoC);
// 	pipe(CtoP);
// 	pid_t fd = fork();
// 	if (!fd)
// 	{
// 		char buff[3000];
// 		close(fd1);
// 		close(fd2);
// 		close(fd3);

// 		close(PtoC[1]);
// 		close(CtoP[0]);
// 		int fd = read(PtoC[0], buff, sizeof(buff));
// 		if (fd == -1)
// 			std::cout << "error\n";
// 		buff[fd] = '\0';
		
// 		std::string str;
// 		str = buff;

// 		std::cout << "from child : \n[" << buff << "]\n" << '\n';
// 		dup2(CtoP[1], 1);
// 		// write(CtoP[1], "executed in child\n", 19);
// 		close(PtoC[0]);
// 		char *const argv[] = {"/bin/bash", buff, nullptr};
// 		if (execv("/bin/bash", argv) == -1)
// 			std::cout << "error exec\n";
// 		close(CtoP[1]);
// 		exit(0);
// 	}
// 	// std::cout << "par file is : \n" << fullstr << '\n';
// 	std::cout << "send from parent " << '\n';

// 	close(PtoC[0]);
// 	close(CtoP[1]);

// 	write(PtoC[1], fullstr.c_str(), fullstr.length());
// 	char buffer[3000];
// 	int xd = read(CtoP[0], buffer, 3000);
// 	buffer[xd] = '\0';
// 	close(PtoC[1]);
// 	close(CtoP[0]);
// 	std::cout << "response from child: \n" << buffer << " " << strlen(buffer) << '\n';

// 	return (0);
// }
unsigned int r(unsigned int d1, unsigned int d2,unsigned int d3,unsigned int d4)
{
	return (d1 << 24 | d2 << 16 | d3 << 8 | d4);
}
int	convert(std::string str)
{
	if (str.empty())
		return (-1);
	// 10.10.12.2
	size_t pos1, pos2, pos3;
	pos1 = str.find(".",0);
	std::string s1, s2, s3, s4;
	s1 = str.substr(0, pos1);
	std::cout << "s1: " << s1 << '\n';
	pos2 = str.find(".", pos1 + 1);
	s2 = str.substr(pos1 + 1, pos2 - (pos1 + 1));
	std::cout << "s2: " << s2 << '\n';
	pos3 = str.find(".", pos2 + 1);
	s3 = str.substr(pos2 + 1, pos3 - (pos2 + 1));
	std::cout << "s3: " << s3 << '\n';
	s4 = str.substr(pos3 + 1, str.length() - (pos3 + 1));
	std::cout << "s4: " << s4 << '\n';

	double d1, d2, d3, d4;
	d1 = strtod(s1.c_str(), NULL);
	d2 = strtod(s2.c_str(), NULL);
	d3 = strtod(s3.c_str(), NULL);
	d4 = strtod(s4.c_str(), NULL);
	unsigned int uc = r(d1, d2, d3, d4);
	std::cout << "uc : " << uc << '\n';
	return (1);
}

int main()
{
	int x = convert("127.0.0.1");
}