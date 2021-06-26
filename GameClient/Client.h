#pragma once

#include "../GameLib/TCPSocket.h"
#include "../GameLib/TCPSocketSelector.h"
#include "../GameLib/TCPListener.h"
#include "../GameLib/TCPStatus.h"

#include "../GameLib/Timer.h"
#include "../GameLib/Deck.h"
#include "../GameLib/PlayerCards.h"

struct Client
{
	int idPlayer, seed;
	TCPSocket* tcpSocket;
	TCPListener* listener;
	TCPSocketSelector* selector;
	TCPStatus* status;

	bool protocolConnected;
	std::string userName;
	bool firstTime = true;
	int enumListener;
	std::vector<TCPSocket*> clients;
	Deck* deck;

	std::map<int, PlayerCards*>playerCards;
	
	bool playerReady = false;

	bool game = false;
public:
	

	Client();
	~Client();
	void Username();
	void JoinGame();
	void LineCout();

	void AssignDeck();
	void AsignTurns();
	void RecievingThread();
	LISTENER GetTag(sf::Packet& packet);
	void ClientsListener();
	void GetConnectedPlayers();
	void ThreadReady();
	void ManageReady(sf::Packet& packet, TCPSocket* tcpSocket);
	void checkReady();
	void SendingThread();

	void ListenerConnection();

	void ConnectServer();

	void ClientLoop();

	std::string EnumToString(LISTENER _listener);

	LISTENER StringToEnum(std::string _string);

};

