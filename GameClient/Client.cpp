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
void Client::RecievingThread() {
	//sf::TcpListener listener;
	////sf::Socket::Status status = listener.listen(tcpSocket->GetLocalPort());
	//// Create a selector
	//sf::SocketSelector *selector;
	//// Add the listener to the selector
	//if (status != sf::Socket::Done)
	//{
	//	std::cout << "Error al abrir listener\n";
	//	exit(0);
	//}
	//else {
	//	//selector->add(listener);
	//}



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
		unsigned short port;
		if (enumListener == LISTENER::CONEXION_NUEVO_PLAYER) {
			for (int i = 0;i < auxiliarNumOfPlayers;i++) {
				packet >> port;
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
		status->SetStatus(listener->Listen(tcpSocket->GetRemotePort(), sf::IpAddress::LocalHost));
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
	//std::thread recievePackets(&Client::GetConnectedPlayers, this);
	//recievePackets.detach();
	GetConnectedPlayers();
	while (true) {

	}
}
