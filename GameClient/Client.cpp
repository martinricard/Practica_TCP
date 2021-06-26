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






		

void Client::JoinOrCreateRoom() {//Envia los paquetes
	sf::Packet packet;
	sf::IpAddress ip = sf::IpAddress::LocalHost;
	unsigned short port = SERVER_PORT;//Modificar este magic number
	std::string message;
	std::string nombreSala;
	std::string contraseña;
	std::string numeroJugadores;
	std::cout << std::endl << "Quieres crear o unirte a partida?(c/u): ";
	std::cin >> message;
	packet << EnumToString(DATOS_PARTIDA);
	packet << std::to_string(tcpSocket->GetLocalPort());
	if (message == "c") {
		packet << message;
		std::cout << std::endl << "Indica el nombre de la sala: ";
		std::cin >> nombreSala;
		packet << nombreSala;
		std::cout << std::endl << "Indica numero de jugadores: ";
		std::cin >> numeroJugadores;
		packet << numeroJugadores;
		std::cout << std::endl << "Quieres poner contraseña?(s/n): ";
		std::cin >> message;
		if (message == "s") {
			std::cout << std::endl << "Indica la contraseña?: ";
			std::cin >> contraseña;
			packet << contraseña;
			tcpSocket->Send(packet);

		}
		else {
			packet << "no";
			tcpSocket->Send(packet);
		}
	}
	else if (message == "u") {
		std::string numOfPlayers;
		std::string withPassword;
		std::cout << std::endl << "Con cuantos jugadores quieres buscar?(3-6, n para no poner filtro. ";
		std::cin >> numOfPlayers;

		std::cout << std::endl << "Quieres mostrar partidas con contraseña? ";
		std::cin >> withPassword;
		packet.clear();
		packet << EnumToString(LISTENER::DATOS_PARTIDA);
		packet << std::to_string(tcpSocket->GetLocalPort());
		std::cout << std::to_string(tcpSocket->GetLocalPort());
		packet << "u";
		tcpSocket->Send(packet);

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
			if (tag == LISTENER::DATOS_PARTIDA) {
				std::cout << "ENTRAMOS";
				std::string numOfPlayers;
				packet >> numOfPlayers;
				int numOfPartidas = std::stoi(numOfPlayers);
				std::string auxiliar2;
				if (numOfPartidas != 0) {
					for (int i = 0;i < numOfPartidas;i++) {
						packet >> auxiliar2;
						std::cout << auxiliar2 << std::endl;
						packet >> auxiliar2;
						std::cout << auxiliar2 << std::endl;
						packet >> auxiliar2;
						std::cout << auxiliar2 << std::endl;
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

				status->SetStatus(client->Connect("localhost", port, sf::milliseconds(15.f)));
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
	JoinOrCreateRoom();
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
	else if (_listener == DATOS_PARTIDA) {
		return "DATOS_PARTIDA";


	}
}
LISTENER Client::StringToEnum(std::string _string) {
	if (_string == "ENVIAR_CLIENTESACTUALES") {
		return LISTENER::ENVIAR_CLIENTESACTUALES;
	}
	else if (_string == "ENVIAR_NUEVOCLIENTE") {
		return LISTENER::ENVIAR_NUEVOCLIENTE;
	}
	else if (_string == "DATOS_PARTIDA") {
		return LISTENER::DATOS_PARTIDA;
	}
}
