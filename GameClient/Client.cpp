#include "Client.h"

Client::Client()
{


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

void Client::ListenerConnection() {
	sf::TcpListener listenerConnection;
	//sf::Socket::Status status = listenerConnection.listen(tcpSocket->GetLocalPort());
	sf::Socket::Status status = listenerConnection.listen(50000);
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al abrir listener\n";
		exit(0);
	}

	// Create a selector
	sf::SocketSelector selector;
	// Add the listener to the selector
	selector.add(listenerConnection);
	// Endless loop that waits for new connections
	while (!false)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			
				sf::Packet packet;
				// The listener is ready: there is a pending connection
				TCPSocket* client = new TCPSocket;
				status = client->Receive(packet);
				packet >> enumListener;
					int numOfPlayers;
					packet >> numOfPlayers;
					unsigned short port;
					if (enumListener == LISTENER::CONEXION_NUEVO_PLAYER) {
						for (int i = 0;i < numOfPlayers;i++) {
							packet >> port;
							tcpSocket->Connect("localhost", port, sf::milliseconds(15.f));
							clients.push_back(client);
						}
					}

				}
			}
		}
	




void Client::ClientLoop()
{

	TCPSocket socket;
	sf::Socket::Status status = socket.Connect("localhost", 50000, sf::milliseconds(15.f));
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al establecer conexion\n";
		exit(0);
	}
	else
	{
		std::cout << "Se ha establecido conexion\n";



	}
	std::string str = "hola";
	std::string userName;
	if (firstTime) {
		std::cout << "Elige un userName: ";
		std::cin >> userName;
		std::cout << std::endl;
		//std::getline(std::cin, userName);
		userName = "u:" + userName;

		sf::Packet packet;
		packet << userName;
		status = socket.Send(packet);
		if (status != sf::Socket::Done)
		{
			std::cout << "Error al enviar userName\n";
		}
		else {
		
		}
		std::cout << std::endl;


		firstTime = false;
	}

	do
	{
		ListenerConnection();

		std::cout << "Escribe ... ";
		std::cin >> str;
		//std::getline(std::cin, str);


		sf::Packet packet;
		packet << str;
		status = socket.Send(packet);
		if (status != sf::Socket::Done)
		{
			std::cout << "Error al enviar\n";
		}
		std::cout << std::endl;
	} while (str != "exit");
	socket.Disconnect();

}
