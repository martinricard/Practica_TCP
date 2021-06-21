#pragma once

#include "../GameLib/TCPSocket.h"
#include "../GameLib/Timer.h"
struct Client
{
	TCPSocket* tcpSocket;
	bool protocolConnected;
	std::string userName;
	bool firstTime = true;
	int enumListener;
	std::list<TCPSocket*> clients;

public:
	
	
	Client();
	~Client();
	void Username();
	void JoinGame();
	void LineCout();


	void RecievingThread();
	void SendingThread();

	void ListenerConnection();

	void ClientLoop();

};

