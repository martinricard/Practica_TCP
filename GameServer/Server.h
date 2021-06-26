#pragma once
#include "../GameLib/TCPSocket.h"
#include "../GameLib/TCPListener.h"
#include "../GameLib/TCPSocketSelector.h"
#include "../GameLib/TCPStatus.h"
#include "../GameLib/Match.h"
class Server
{
	// Create a list to store the future clients
	std::vector<TCPSocket*> clients;

	std::list<Match*>partidas;

	TCPSocket* tcpSocket;
	TCPListener* listener;
	TCPStatus* status;
	TCPSocketSelector* selector;
	bool onRoad;


	//std::map<short, Client*> ourClients;


public:
	
	
	Server();
	~Server();
	void SendNewClient(TCPSocket& socket);
	void RecievingThread();
	void SendClients(TCPSocket& socket);
	void ServerListener();
	void ControlServidor();
	std::string EnumToString(LISTENER _listener);
	LISTENER StringToEnum(std::string _string);
	bool IsClientInMap(unsigned short checkPort);
	

};


