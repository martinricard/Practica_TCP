#pragma once

#include "../GameLib/TCPSocket.h"
#include "../GameLib/Timer.h"
struct Client
{
	TCPSocket* tcpSocket;
	bool protocolConnected;
	std::mutex clientMtx;
	std::string username;


public:
	
	
	Client();
	~Client();
	void Username();
	void JoinGame();
	void LineCout();


	void RecievingThread();
	void SendingThread();

	void ClientLoop();

};

