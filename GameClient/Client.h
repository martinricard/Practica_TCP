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
	bool getClients = false;
	bool protocolConnected;
	std::string userName;
	bool firstTime = true;
	int enumListener;
	std::vector<TCPSocket*> clients;
	Deck* deck;

	std::map<int, PlayerCards*>playerCards;
	
	bool playerReady = false;
	std::vector<std::string> aMensajes;

	bool game = false;
	std::mutex clientMutex;
	bool gameBegin = false;
	bool createdOrJoinAGame;
	bool indicaciones = false;
	int tamañoSala = 0;
	int player2Steal;
	bool waitingAnswer1, waitingAnswer2, waitingAnswer3;
	CULTURA cultura;
	MIEMBRO_FAMILIA familia;
public:
	

	Client();
	~Client();
	void Username();
	void JoinGame();
	void LineCout();

	void AssignDeck();
	void AsignTurns();
	void Waiting4Players();
	void RecievingThread();
	LISTENER GetTag(sf::Packet& packet);
	void ManageCambioCarta(sf::Packet& packet);
	void ManageMessage(sf::Packet& packet);
	void ClientsListener();
	void ChangeCardsBetweenPlayers(int _actualPlayer, int _changePlayer, CULTURA _culture, MIEMBRO_FAMILIA _familia);
	void PasarTurno();
	void InterfazChat();
	void SendPasarTurno();
	void ChooseCard();
	void GetConnectedPlayers();
	bool CheckPlayersReady();
	void CheckPlayersReady2();

	void JoinOrCreateRoom();
	void ManageReady(sf::Packet& packet, TCPSocket* tcpSocket);
	void checkReady();
	void SendingThread();

	void ListenerConnection();

	void ConnectServer();

	bool CheckCard(int _id, CULTURA _cultura, MIEMBRO_FAMILIA _familia);

	void SendCambioCarta(int _id, int playerToChange, CULTURA _cultura, MIEMBRO_FAMILIA _familia);





	void ManageGame();

	void ClientLoop();

	std::string EnumToString(LISTENER _listener);

	LISTENER StringToEnum(std::string _string);

	CULTURA StringToCultura(std::string _string);

	MIEMBRO_FAMILIA StringToFamilia(std::string _string);

	std::string FamiliaToString(MIEMBRO_FAMILIA _familia);

	std::string CulturaToString(CULTURA _cultura);



};

