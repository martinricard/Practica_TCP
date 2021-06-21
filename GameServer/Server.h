#pragma once
#include "../GameLib/TCPSocket.h"
class Server
{
	TCPSocket* tcpSocket;

	std::mutex servermtx;
	sf::TcpListener listener;
	sf::SocketSelector selector;


	// Create a list to store the future clients
	std::list<TCPSocket*> clients;
	//std::map<short, Client*> ourClients;


public:
	
	
	Server();
	~Server();
	void ControlServidor();
	bool IsClientInMap(unsigned short checkPort);
	

};


