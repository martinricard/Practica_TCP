#include "Client.h"

Client::Client()
{
	tcpSocket = new TCPSocket();
	listener = new TCPListener();
	selector = new TCPSocketSelector();
	status = new TCPStatus();

}

Client::~Client()
{
}


//Cuando el cliente recibe un manageChallenge_Q, lo recibe, solucionar el challenge se hace en la parte donde recibe el paquete


void Client::LineCout() {
	std::cout << std::endl<<"-------------------------------------------------------------" << std::endl;

}

//Conseguimos un random float para la perdida de paquetes
static float GetRandomFloat() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(0.f, 1.f);
	return dis(gen);
}
bool Client::CheckPlayersReady() {
		LineCout();
		bool ready = true;
		system("cls");
		if (!playerReady) {
			ready = false;
		}
		for (auto it : clients) {
			if (it->GetReady()) {
				std::cout << "El jugador " << it->GetLocalPort() << " esta ready";
			}
			else{
				std::cout << "El jugador " << it->GetLocalPort() << " no esta ready";
				ready = false;
			}
		}
		if (ready) {
			LineCout();
			std::cout << "            TODOS ESTAN READY";
			LineCout();

			gameBegin = true;

		}
		return ready;



	
}
void Client::CheckPlayersReady2() {
	LineCout();
	bool ready = true;
	system("cls");
	if (!playerReady) {
		ready = false;
	}
	for (auto it : clients) {
		if (it->GetReady()) {
			std::cout << "El jugador " << it->GetLocalPort() << " esta ready";
		}
		else {
			std::cout << "El jugador " << it->GetLocalPort() << " no esta ready";
			ready = false;
		}
	}
	if (ready) {
		LineCout();
		std::cout << "            TODOS ESTAN READY";
		LineCout();

		gameBegin = true;

	}




}
void Client::JoinOrCreateRoom() {//Envia los paquetes
	bool createdOrJoinAGame = false;
	while (createdOrJoinAGame==false)
	{

			sf::Packet packet;
		sf::IpAddress ip = sf::IpAddress::LocalHost;
		unsigned short port = SERVER_PORT;//Modificar este magic number
		std::string message;
		std::string nombreSala;
		std::string contraseña;
		std::string numeroJugadores;
		std::cout << std::endl << "Quieres crear, buscar o unirte a partida?(c/b/u): ";
		std::cin >> message;
		packet << EnumToString(LISTENER::CREAR_PARTIDA);
		packet << std::to_string(tcpSocket->GetLocalPort());
		if (message == "c") {
			packet << message;
			std::cout << std::endl << "Indica el nombre de la sala: ";
			std::cin >> nombreSala;
			packet << nombreSala;
			std::cout << std::endl << "Indica numero de jugadores:(3-6) ";
			std::cin >> numeroJugadores;
			packet << numeroJugadores;
			std::cout << std::endl << "Quieres poner contraseña?(s/n): ";
			std::cin >> message;
			if (message == "s") {
				std::cout << std::endl << "Indica la contraseña?: ";
				std::cin >> contraseña;
				packet << contraseña;
				tcpSocket->Send(packet);
				createdOrJoinAGame = true;
			}
			else {
				packet << "none";
				tcpSocket->Send(packet);
				createdOrJoinAGame = true;

			}
		}
		else if (message == "b") {
			std::string numOfPlayers;
			std::string withPassword;
			std::cout << std::endl << "Con cuantos jugadores quieres la sala?(3-6, n para no poner filtro. ";
			std::cin >> numOfPlayers;
			
			std::cout << std::endl << "Quieres mostrar partidas con contraseña?(s/n) ";
			std::cin >> withPassword;
			packet.clear();
			packet << EnumToString(LISTENER::BUSCAR_PARTIDA);
			packet << std::to_string(tcpSocket->GetLocalPort());
			packet << numOfPlayers;
			packet << withPassword;
			tcpSocket->Send(packet);

		}
		else if (message == "u") {
			std::string nameOfRoom;
			std::string password;
			std::cout << std::endl << "A que sala te quieres unir? ";
			std::cin >> nameOfRoom;
			std::cout << std::endl << "Escribe la contraseña: (si no tiene escribe none)";
			std::cin >> password;
			packet.clear();
			packet << EnumToString(LISTENER::UNIRSE_PARTIDA);
			packet << std::to_string(tcpSocket->GetLocalPort());
			packet << nameOfRoom;
			packet << password;
			createdOrJoinAGame = true;
			tcpSocket->Send(packet);

		}
	} 
}




void Client::ManageReady(sf::Packet &packet, TCPSocket* _tcpSocket) {
	for (auto it : clients) {
		if (it->GetRemotePort() == _tcpSocket->GetRemotePort()) {
			it->SetReady(true);
		}
	}
	CheckPlayersReady2();
}
void Client::checkReady() {
	if (!playerReady) {
		std::string message;
		std::cout << "Escribe ready para empezar la partida: ";
		std::cin >> message;
		if (message == "ready") {
			std::cout << "Estas listo para empezar la partida, hay que comprobar el resto de jugadores. " << std::endl;
			playerReady = true;
			CheckPlayersReady2();
			sf::Packet packet;
				for (auto it : clients) {

					packet << EnumToString(LISTENER::READY);
					status->SetStatus(it->Send(packet));
					if (status->GetStatus() == sf::Socket::Done)
					{
						std::cout << "El paquete se ha enviado correctamente\n";
						packet.clear();
					}
					else {
						std::cout << "El paquete no se ha podido enviar\n";
					}
				}

			
		}
	}

}




		
/*
void Client::SendingThread() {//Envia los paquetes
	sf::Packet packet;
	sf::IpAddress ip = sf::IpAddress::LocalHost;
	unsigned short port = SERVER_PORT;//Modificar este magic number
	std::string message;
	while (true)
	{
		if (protocolConnected) {
			std::cout << std::endl << "Escribe el mensaje que quieras enviar: ";
			std::cin >> message;
			packet.clear();

			if (tcpSocket->tcpStatus == sf::Socket::Done) {
				std::cout << std::endl << "Se ha enviado: " << message << std::endl;
				packet.clear();

				message = "";
				//std::cout << std::endl << "Escribe el mensaje que quieras enviar: ";

			}
			else {
				std::cout << "Ha habido un error enviando el paquete";
			}
		}
	
	}
}
*/
void Client::AssignDeck()
{
	system("cls");
	deck = new Deck();
	if (idPlayer == 0) {
		seed = tcpSocket->GetRemotePort();
		deck->MixDeck(seed);
	}
	else {
		for (int i = 0;i < clients.size();i++) {
			if (clients[i]->GetID() == 0) {
				seed = clients[i]->GetRemotePort();
				deck->MixDeck(seed);
			
			}
		}
	}

}

void Client::AsignTurns()
{
	//ESTE 5 SERAN LOS NUMEROS DE JUGADORES QUE HAY EN LA PARTIDA
	for (int i = 0;i < 5;i++) {
		playerCards[i] = new PlayerCards();
		playerCards[i]->actualTurn = 0;
		playerCards[i]->isPlaying = true;
	
	}

	for (int i = 0; i < deck->deck.size();i++) {
		if (i <= 7)playerCards[0]->giveCard(*deck->deck[i]);
		else if(i>7&&i<=14)playerCards[1]->giveCard(*deck->deck[i]);
		else if(i>14&&i<=21)playerCards[2]->giveCard(*deck->deck[i]);
		else if(i>21&&i<=28)playerCards[3]->giveCard(*deck->deck[i]);
		else playerCards[4]->giveCard(*deck->deck[i]);
		
	}
}

void Client::Waiting4Players() {
	Sleep(5000);
		while (clients.size() < 4) {
			if (selector->Wait()) {
				if (selector->isReady(&listener->GetListener())) {
					TCPSocket* client = new TCPSocket();
					if (listener->Accept(client->GetSocket()) == sf::Socket::Done)
					{
						std::cout << "Connexion recibia de: " << client->GetRemotePort() << std::endl;
						selector->Add(client->GetSocket());
						client->SetID(clients.size() + 1);
						clientMutex.lock();
						clients.push_back(client);
						clientMutex.unlock();
						std::cout << "Hay " << clients.size() << " clientes conectados a este cliente." << std::endl;
					}
					else {
						delete client;
						std::cout << "Error al recibir un player." << std::endl;
						exit(0);
					}
				}

			}
		}
		game = true;

	
	
}

void Client::RecievingThread() {
	while (!this->getClients) {
		sf::Packet packet;
		status->SetStatus(tcpSocket->Receive(packet));
		if (status->GetStatus() != sf::Socket::Done)
		{
			std::cout << "Error al recibir el paquete\n";
		}
		else {
			std::cout << "Se ha recibido un paquete\n";
			std::string stringTag;
			packet >> stringTag;
			LISTENER tag = StringToEnum(stringTag);
			std::string size;
			int sizeInt;
			std::string nombreSala;
			std::string numeroJugadoresActuales;
			std::string numeroJugadoresPartida;
			std::string stringPort;
			std::string numOfPlayers;
			int auxiliarNumOfPlayers;
			switch (tag)
			{
		
			case BUSCAR_PARTIDA:
				packet >> size;
				sizeInt = std::stoi(size);

				if (sizeInt != 0) {
					for (int i = 0;i < sizeInt;i++ ) {
						packet >> nombreSala;
						packet >> numeroJugadoresActuales;
						packet >> numeroJugadoresPartida;
						std::cout << nombreSala << "   (" << numeroJugadoresActuales << "/" << numeroJugadoresPartida << ")" << std::endl;
					}
				}
				break;
				case ENVIAR_CLIENTESACTUALES:
				
				packet >> numOfPlayers;
				auxiliarNumOfPlayers = std::stoi(numOfPlayers);
			
				int port;
				for (int i = 0;i < auxiliarNumOfPlayers;i++) {

					packet >> stringPort;
					port = std::stoi(stringPort);
					TCPSocket* client = new TCPSocket;

					status->SetStatus(client->Connect("localhost", port, sf::milliseconds(15.f)));
					if (status->GetStatus() == sf::Socket::Done) {
						client->SetID(i);
						std::cout << "Se ha conectado con el cliente " << port << std::endl;
						clients.push_back(client);
						selector->Add(client->GetSocket());
						getClients = true;

					}
					else {
						std::cout << "Error al conectarse con el jugador" << std::endl;
						delete client;
					}

				}
				idPlayer = auxiliarNumOfPlayers;
			default:
				break;
			}

			}
		}
	}



LISTENER Client::GetTag(sf::Packet& packet) {
	std::string auxiliar;
	packet >> auxiliar;
	return StringToEnum(auxiliar);
}


void Client::ClientsListener() {
	while (true) {
		for (auto it : clients) {
			if (selector->Wait()) {
				if (selector->isReady(it->GetSocket()))
				{
					sf::Packet packet;
					status->SetStatus(it->Receive(packet));
					if (status->GetStatus() == sf::Socket::Done) {
						LISTENER tag = GetTag(packet);
						switch (tag)
						{
						case READY:
							ManageReady(packet, it);
							break;

						case PASAR_TURNO:
							PasarTurno();
							break;
						}


					}
					else if (status->GetStatus()==sf::Socket::Disconnected) {
						selector->Remove(it->GetSocket());
						int aux = -1;
						for (int i = 0;i < clients.size();i++) {
							if (clients[i]->GetRemotePort() == it->GetRemotePort()) {
								aux = i;
							}
						}
						clients.erase(clients.begin() + aux);
					}
				}
			}
		}
	}
}

void Client::ChangeCardsBetweenPlayers(int _actualPlayer, int _changePlayer, CULTURA _culture, MIEMBRO_FAMILIA _familia) {
	Card* card = new Card(_culture, _familia);
	playerCards[_actualPlayer]->giveCard(*card);
	playerCards[_changePlayer]->eraseCard(*card);


}


void Client::PasarTurno() {
	int actualTurn = playerCards[idPlayer]->actualTurn;

	for (int i = 0; i < playerCards.size();i++) {
		if (actualTurn == 4) {
			playerCards[i]->actualTurn = 0;
		}
		else {
			playerCards[i]->actualTurn = playerCards[i]->actualTurn + 1;
		}
	}
}

void Client::SendPasarTurno() {
	sf::Packet packet;
	for (auto it : clients) {

		packet << EnumToString(LISTENER::PASAR_TURNO);
		status->SetStatus(it->Send(packet));
		if (status->GetStatus() == sf::Socket::Done)
		{
			std::cout << "El paquete se ha enviado correctamente\n";
			packet.clear();
		}
		else {
			std::cout << "El paquete no se ha podido enviar\n";
		}
	}

}
void Client::ChooseCard() {
	do
	{
		std::cout << "Elige jugador al que robar:(Numero) ";
		std::cin >> player2Steal;
		std::cout << std::endl;
	} while (player2Steal >6&&idPlayer==player2Steal);
	int auxCultura;
	do
	{
		std::cout << "Elige la cultura:  0: ARABE   1: BANTU   2: CHINA   3: ESQUIMAL   4: INDIA   5: MEXICANA   6: TIROLESA " << std::endl;
		std::cin >> auxCultura;
		cultura = (CULTURA)auxCultura;

	} while (auxCultura > 6);
	int auxFamilia;
	do
	{
		std::cout << "Elige la familia:  0: ABUELO   1: ABUELA   2: PADRE   3: MADRE   4: HIJO   5: HIJA" << std::endl;
		std::cin >> auxFamilia;
		familia = (MIEMBRO_FAMILIA)auxFamilia;

	} while (auxFamilia > 5);
	if (CheckCard(player2Steal, cultura, familia)) {
		std::cout << "El jugador tiene la carta";
		ChangeCardsBetweenPlayers(idPlayer, player2Steal, cultura, familia);
		ManageGame();
	
	
	
	}
	else {
		std::cout << "El jugador no tiene la carta";
		PasarTurno();
		SendPasarTurno();
		ManageGame();

	}

}

void Client::GetConnectedPlayers() {
	sf::Packet packet;
	status->SetStatus(tcpSocket->Receive(packet));

	if (status->GetStatus() != sf::Socket::Done)
	{
		std::cout << "Error al recibir el paquete\n";
	}
	else {
		std::cout << "Se ha recibido un paquete\n";
		std::string stringTag;
		packet >> stringTag;
		LISTENER tag = StringToEnum(stringTag);
		if (tag == LISTENER::ENVIAR_CLIENTESACTUALES) {
			std::string numOfPlayers;
			int auxiliarNumOfPlayers;
			packet >> numOfPlayers;
			auxiliarNumOfPlayers = std::stoi(numOfPlayers);
			std::string stringPort;
			int port;
			for (int i = 0;i < auxiliarNumOfPlayers;i++) {

				packet >> stringPort;
				port = std::stoi(stringPort);
				TCPSocket* client = new TCPSocket;

				status->SetStatus(client->Connect("localhost", port, sf::milliseconds(15.f)));
				if (status->GetStatus() == sf::Socket::Done) {
					client->SetID(i);
					std::cout << "Se ha conectado con el cliente " << port << std::endl;
					clients.push_back(client);
					selector->Add(client->GetSocket());
				}
				else {
					std::cout << "Error al conectarse con el jugador" << std::endl;
					delete client;
				}

			}
			idPlayer = auxiliarNumOfPlayers;


		}
	}
}
//Aqui nos conectamos al bss y tambien abrimos el listener
void Client::ConnectServer() {
	status->SetStatus(tcpSocket->Connect("localhost", 50000, sf::milliseconds(15.f)));
	if (status->GetStatus() != sf::Socket::Done)
	{
		std::cout << "Error al establecer conexion\n";
		//exit(0);
	}
	else
	{
		std::cout << "Se ha establecido conexion\n";
		status->SetStatus(listener->Listen(tcpSocket->GetLocalPort(), sf::IpAddress::LocalHost));
		if (status->GetStatus() == sf::Socket::Done) {
			std::cout << "Se ha abierto el listener\n";
			selector->Add(&listener->GetListener());
		}
		else {
			std::cout << "Error al abrir el listener\n";
			//exit(0);

		}
	}
}

bool Client::CheckCard(int _id, CULTURA _cultura, MIEMBRO_FAMILIA _familia) {
	Card* card = new Card(_cultura, _familia);
	return playerCards[_id]->checkCard(*card);
}



void Client::ManageGame() {
	if (idPlayer == playerCards[idPlayer]->actualTurn)
	{
		if (playerCards[idPlayer]->isPlaying) {
			system("cls");

			std::cout << "Estas son tus cartas: " << std::endl;
			playerCards[idPlayer]->PrintHand();
			ChooseCard();
		}
	}
	else {
		//std::cout << "Player id: "<<idPlayer << std::endl;
		//std::cout << "Actual Turn: " << playerCards[idPlayer]->actualTurn << std::endl;
		Sleep(1000);
	}
}


void Client::ClientLoop()
{
	ConnectServer();
	std::thread recievingThread(&Client::RecievingThread, this);
	recievingThread.detach();
	JoinOrCreateRoom();
	//GetConnectedPlayers();
	
	Waiting4Players();

	AssignDeck();
	AsignTurns();
	std::thread clientsSelector(&Client::ClientsListener, this);
	clientsSelector.detach();

	while (game) {
		checkReady();

		while (gameBegin) {
			ManageGame();
		}
	}
}
std::string Client::EnumToString(LISTENER _listener) {
	if (_listener == ENVIAR_CLIENTESACTUALES) {
		return "ENVIAR_CLIENTESACTUALES";
	}
	else if (_listener == ENVIAR_NUEVOCLIENTE) {
		return "ENVIAR_NUEVOCLIENTE";

	}
	else if (_listener == READY) {
		return "READY";

	}
	else if (_listener == DATOS_PARTIDA) {
		return "DATOS_PARTIDA";

	}
	else if (_listener == BUSCAR_PARTIDA) {
		return "BUSCAR_PARTIDA";

	}
	else if (_listener == CREAR_PARTIDA) {
		return "CREAR_PARTIDA";

	}
	else if (_listener == UNIRSE_PARTIDA) {
		return "UNIRSE_PARTIDA";

	}
	else if (_listener == PASAR_TURNO) {
		return "PASAR_TURNO";

	}
}
LISTENER Client::StringToEnum(std::string _string) {
	if (_string == "ENVIAR_CLIENTESACTUALES") {
		return LISTENER::ENVIAR_CLIENTESACTUALES;
	}
	else if (_string == "ENVIAR_NUEVOCLIENTE") {
		return LISTENER::ENVIAR_NUEVOCLIENTE;
	}
	else if (_string == "READY") {
		return LISTENER::READY;
	}
	else if (_string == "DATOS_PARTIDA") {
		return LISTENER::DATOS_PARTIDA;
	}
	else if (_string == "BUSCAR_PARTIDA") {
		return LISTENER::BUSCAR_PARTIDA;
	}
	else if (_string == "CREAR_PARTIDA") {
		return LISTENER::CREAR_PARTIDA;
	}
	else if (_string == "UNIRSE_PARTIDA") {
		return LISTENER::UNIRSE_PARTIDA;
	}
	else if (_string == "PASAR_TURNO") {
		return LISTENER::PASAR_TURNO;
	}
}
