#pragma once
#include "../GameLib/TCPSocket.h"
#include "../GameLib/PlayerInfo.h"
class Server
{
	TCPSocket* tcpSocket;

	std::mutex servermtx;
	sf::TcpListener listener;
	sf::SocketSelector selector;


	std::list<sf::TcpSocket*> clients;
	std::map<short, PlayerInfo*> ourClients;
public:
	
	
	Server();
	~Server();
	bool IsClientInMap(unsigned short checkPort);
	
	void ServerLoop();

};


