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
	int CtoP[2];
    pipe(CtoP);
    
    if (pipe(CtoP) < 0)
    {
        std::cerr << "pipe failed";
    }
    pid_t pid = fork();

    if (pid == -1) 
	{
        // Error handling
        std::cerr << "Fork failed";
        return 1;
    } 
	else if (pid == 0) 
	{
        // Child process
        std::cout << "Child process running..." << std::endl;

      
		close(CtoP[0]);
        dup2(CtoP[1], 1);
        if (execve(args[0], args, envp) == -1)
        {
            std::cout << "EXECVE ERROR\n";
        }
        close(CtoP[1]);
        std::cout << "Child process finished." << std::endl;
        return 0;
    } 
	else 
	{
        // Parent process
        std::cout << "Parent process waiting for child..." << std::endl;
        
        bool childInLoop = true; // Flag indicating if child is in loop
        
        while (childInLoop) {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG); // Non-blocking wait
            if (result == 0) {
                std::cout << "Child still running, parent continuing..." << std::endl;
                sleep(2); // Delay before checking again
            } else if (result == -1) {
                std::cerr << "Error in waitpid";
                break;
            } else {
                std::cout << "Child process finished, parent exiting loop." << std::endl;
                childInLoop = false;
            }
        }

        // Parent continues its execution
        std::cout << "Parent process continuing..." << std::endl;
        // Additional processing here
    }

    return 0;
}
