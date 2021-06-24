#pragma once
#include "../GameLib/TCPSocket.h"
#include "../GameLib/TCPListener.h"
#include "../GameLib/TCPSocketSelector.h"
#include "../GameLib/TCPStatus.h"

class Server
{
	// Create a list to store the future clients
	std::list<TCPSocket*> clients;


	TCPSocket* tcpSocket;
	TCPListener* listener;
	TCPStatus* status;
	TCPSocketSelector* selector;
	


	//std::map<short, Client*> ourClients;


public:
	
	
	Server();
	~Server();
	void SendClients(TCPSocket& socket);
	void ControlServidor();
	bool IsClientInMap(unsigned short checkPort);
	

};


