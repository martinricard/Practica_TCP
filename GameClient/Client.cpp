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
//Se une a la partida si escribe y, sino se cierra la consola
void Client::JoinGame() {
	std::string confirmation;
	std::cout << std::endl << "Do you want to join or create a game? (y/n): ";
	std::cin >> confirmation;
	if (confirmation == "y" || confirmation == "Y") {
		system("cls");
	}
	else {
		exit(0);
	}
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
	deck = new Deck();
	if (idPlayer == 0) {
		seed = tcpSocket->GetRemotePort();
		deck->MixDeck(seed);
	}
	else {
		for (int i = 0;i < clients.size();i++) {
			if (clients[i]->GetID() == 0) {
				seed = clients[i]->GetID();
				deck->MixDeck(seed);
			
			}
		}
	}

}

void Client::AsignTurns()
{
	for (int i = 0;i < 5;i++) {
		playerCards[i] = new PlayerCards();

	
	}
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
			if (tag == LISTENER::ENVIAR_NUEVOCLIENTE) {
				int port;
				std::string stringPort;

				packet >> stringPort;
				port = std::stoi(stringPort);
				TCPSocket* client = new TCPSocket;

				status->SetStatus(client->Connect("localhost", port, sf::milliseconds(15.f)));
				if (status->GetStatus() == sf::Socket::Done) {
					std::cout << "Se ha conectado con el cliente " << port << std::endl;
					client->SetID(clients.size() + 1);
					clients.push_back(client);
					selector->Add(client->GetSocket());
				}
				else {
					std::cout << "Error al conectarse con el jugador" << std::endl;
					delete client;
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
		packet >> enumListener;
		std::string numOfPlayers;
		int auxiliarNumOfPlayers;
		packet >> numOfPlayers;
		auxiliarNumOfPlayers = std::stoi(numOfPlayers);
		std::string stringPort;
		int port;
		if (enumListener == LISTENER::ENVIAR_CLIENTESACTUALES) {
			for (int i = 0;i < auxiliarNumOfPlayers;i++) {
				
				packet >> stringPort;
				port = std::stoi(stringPort);
				TCPSocket* client = new TCPSocket;

				status->SetStatus(tcpSocket->Connect("localhost", port, sf::milliseconds(15.f)));
				if (status->GetStatus() == sf::Socket::Done) {
					std::cout << "Se ha conectado con el cliente " << port << std::endl;
					clients.push_back(client);
					selector->Add(client->GetSocket());
				}
				else {
					std::cout << "Error al conectarse con el jugador" << std::endl;
					delete client;
				}

			}


		}


		}
	
}
//Aqui nos conectamos al bss y tambien abrimos el listener
void Client::ConnectServer() {
	status->SetStatus(tcpSocket->Connect("localhost", 50000, sf::milliseconds(15.f)));
	if (status->GetStatus() != sf::Socket::Done)
	{
		std::cout << "Error al establecer conexion\n";
		exit(0);
	}
	else
	{
		std::cout << "Se ha establecido conexion\n";
		status->SetStatus(listener->Listen(tcpSocket->GetLocalPort(), sf::IpAddress::LocalHost));
		if (status->GetStatus() == sf::Socket::Done) {
			std::cout << "Se ha abierto el listener\n";

		}
		else {
			std::cout << "Error al abrir el listener\n";
			exit(0);

		}
	}
}



void Client::ClientLoop()
{
	ConnectServer();

//	GetConnectedPlayers();

	std::thread listeningRecieving(&Client::RecievingThread, this);
	listeningRecieving.detach();

	while (true) {

	}
}
std::string Client::EnumToString(LISTENER _listener) {
	if (_listener == ENVIAR_CLIENTESACTUALES) {
		return "ENVIAR_CLIENTESACTUALES";
	}
	else if (_listener == ENVIAR_NUEVOCLIENTE) {
		return "ENVIAR_NUEVOCLIENTE";

	}
}
LISTENER Client::StringToEnum(std::string _string) {
	if (_string == "ENVIAR_CLIENTESACTUALES") {
		return LISTENER::ENVIAR_CLIENTESACTUALES;
	}
	else if (_string == "ENVIAR_NUEVOCLIENTE") {
		return LISTENER::ENVIAR_NUEVOCLIENTE;
	}
}
