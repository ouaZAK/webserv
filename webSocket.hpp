
#pragma once

class webSocket
{
	private:
		int	newClientSocket;
		int	maxSocket;
		int	serverSocket;

	public:
		webSocket();
		webSocket(int newClientSocket, int maxSocket, int serverSocket);
		~webSocket();
		int	getNewClientSocket();
		int	getmaxSocket();
		int	getserverSocket();
};