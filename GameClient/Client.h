#pragma once

#include "../GameLib/TCPSocket.h"
#include "../GameLib/TCPSocketSelector.h"
#include "../GameLib/TCPListener.h"
#include "../GameLib/TCPStatus.h"

#include "../GameLib/Timer.h"
struct Client
{
	TCPSocket* tcpSocket;
	TCPListener* listener;
	TCPSocketSelector* selector;
	TCPStatus* status;

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
	void GetConnectedPlayers();
	void SendingThread();

	void ListenerConnection();

	void ConnectServer();

	void ClientLoop();

};

