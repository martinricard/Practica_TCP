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
void Client::CheckPlayersReady() {
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


	
}
void Client::ManageReady(sf::Packet &packet, TCPSocket* _tcpSocket) {
	for (auto it : clients) {
		if (it->GetRemotePort() == _tcpSocket->GetRemotePort()) {
			it->SetReady(true);
		}
	}
	CheckPlayersReady();
}
void Client::checkReady() {
	if (!playerReady) {
		std::string message;
		std::cout << "Escribe ready para empezar la partida: ";
		std::cin >> message;
		if (message == "ready") {
			std::cout << "Estas listo para empezar la partida, hay que comprobar el resto de jugadores. " << std::endl;
			playerReady = true;

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
}

void Client::Waiting4Players() {
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
	while (true) {
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
void Client::ManageGame() {
	if (idPlayer == playerCards[0]->actualTurn)
	{

	}
}


void Client::ClientLoop()
{
	ConnectServer();

	GetConnectedPlayers();
	
	Waiting4Players();

	AssignDeck();
	AsignTurns();
	std::thread clientsSelector(&Client::ClientsListener, this);
	clientsSelector.detach();

	while (game) {
		checkReady();

		while (gameBegin) {

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
}
