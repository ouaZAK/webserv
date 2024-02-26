// #include <iostream>
// #include <unistd.h>
// #include<fstream>

// int main(int ac, char **av, char **env)
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
// 		char *const argv[] = {buff, "cgi/te.py", NULL};
// 		// char **s;
// 		// s[0] = strdup()
// 		// char *const env[] = {"/bin/bash", buff, nullptr};
// 		if (execve(buff, argv, env) == -1)
// 			std::cout << "error exec\n";
// 		close(CtoP[1]);
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
// // "    _cgi_envs["CONTENT_LENGTH"] = ft_itos(request.content.size());
// //     _cgi_envs["CONTENT_TYPE"] =  request.getHeader("Content-Type");
// //     _cgi_envs["QUERY_STRING"] = request.query_string;
// //     _cgi_envs["REQUEST_METHOD"] = request.method;
// //     _cgi_envs["REQUEST_URI"] = request.uri;
// //     _cgi_envs["SERVER_NAME"] = request.getHeader("Host");
// //     _cgi_envs["SERVER_SOFTWARE"] = "nginy/1.0";
// //     _cgi_envs["REDIRECT_STATUS"] = "200";
// //     _cgi_envs["HTTP_CONNECTION"] = request.getHeader("Connection");
// //     _cgi_envs["HTTP_HOST"] = request.getHeader("Host");
// //     _cgi_envs["HTTP_USER_AGENT"] = request.getHeader("User-Agent");
// //     _cgi_envs["HTTP_ACCEPT"] = request.getHeader("Accept");
// //     _cgi_envs["HTTP_VERSION"] = "HTTP/1.1";
// //     _cgi_envs["SERVER_PROTOCOL"] = "HTTP/1.1";
// //     _cgi_envs["SERVER_NAME"] = request.getHeader("Host");
// //     _cgi_envs["PATH_INFO"] = request.uri;
// //     _cgi_envs["SERVER_SOFTWARE"] = "nginy/1.0";"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	if (file.is_open())
		file.close();
	cout << "CGI IS DETECTED\n";
	int filefd = 0;
	int pid = fork();
	char **env;
	env = cgi_envatment(envv);
	if (pid == 0)
	{
		if (method == "POST")
		{
			size_t start = request.find("\r\n\r\n");
			if (start != std::string::npos)
			{
				int pipfd[2];
				start += 4;
				std::string body = request.substr(start);
				if (pipe(pipfd) == -1) 
					throw (std::runtime_error("pipe"));

				ssize_t bytes_written = write(pipfd[1], body.c_str(), body.size());
				if (bytes_written == -1 || static_cast<size_t>(bytes_written) != body.size())
					throw(std::runtime_error("write"));
				close(pipfd[1]);
				if (dup2(pipfd[0], 0) == -1)
					std::cerr << "dup2 failed";
				close(pipfd[0]);
			}
			else
				throw(std::runtime_error("Unable to find the start of the body"));
		
		}

		filefd = open("cgi.html", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		dup2(filefd, 1);
		close(filefd);
		char *argv[2];
		argv[0] = (char *)filename.c_str();
		argv[1] = NULL;
		
		if (execve(filename.c_str(), argv, env) == -1)
			kill(getpid(), SIGKILL);
	}
	else if (pid < 0)
	{
		cout << "Error fork" << endl;
	}
	else
	{
		int monitor_process_id = fork();
		if (monitor_process_id == 0)
		{
			sleep(5);
			// cout << "---------------()---------------\n";
			kill(pid, SIGTERM);
			kill(getpid(), SIGKILL);
		}
		else if (monitor_process_id < 0)
			cout << "Error fork" << endl;
		else
		{
			int status;
			waitpid(pid, &status, 0);
			kill(monitor_process_id, SIGKILL);
			if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL))
			{

				// cout << "---------------501 error---------------\n";
				this->status = 501;
				filename = "./501.html";
			}
			else if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGTERM))
			{
				// cout << "---------------408 error---------------\n";
				this->status = 408;
				filename = "./408.html";
			}
			else
				filename = "./cgi.html";
		}
	}
	for(int i = 0; env[i] != NULL; i++) {
		delete[] env[i];
	}
	delete[] env;

    return 0;
}
