#include "Server.h"



Server::Server()
{
	tcpSocket = new TCPSocket();
	listener = new TCPListener();
	selector = new TCPSocketSelector();
	status = new TCPStatus();
}

Server::~Server()
{
	delete[] tcpSocket;
	delete[] listener;
	delete[] selector;
	delete[] status;
}


void Server::SendNewClient(TCPSocket& socket) {
	sf::Packet packet;
	unsigned short port = socket.GetRemotePort();;
	std::string stringPort = std::to_string(port);;
	

	for (auto it : clients) {
		packet << EnumToString(ENVIAR_NUEVOCLIENTE);
		packet << stringPort;
		status->SetStatus(it->Send(packet));
		if (status->GetStatus() == sf::Socket::Done) {
			std::cout << "El paquete de enviar clientes se ha enviado correctamente" << std::endl;
			packet.clear();
		}
		else {
			std::cout << "El paquete de enviar clientes no se ha enviado correctamente" << std::endl;
		}
	}
}
void Server::SendClients(TCPSocket& socket) {
	sf::Packet packet;
	unsigned short port;
	std::string stringPort;
	std::string tag = EnumToString(ENVIAR_CLIENTESACTUALES);
	packet << tag;
	packet << std::to_string(clients.size());

	for (auto it : clients) {
	

		port =it->GetRemotePort();
		stringPort = std::to_string(port);
		packet << stringPort;

		
	}
	status->SetStatus(socket.Send(packet));
	if (status->GetStatus() == sf::Socket::Done) {
		std::cout << "El paquete de enviar clientes se ha enviado correctamente" << std::endl;
		packet.clear();
	}
	else {
		std::cout << "El paquete de enviar clientes no se ha enviado correctamente" << std::endl;
	}
}

void Server::ServerListener() {

	status->SetStatus(listener->Listen(50000, sf::IpAddress::LocalHost));
	if (status->GetStatus() != sf::Socket::Done)
	{
		std::cout << "Error al abrir iniciar al servidor" << std::endl;;
		exit(0);
	}
	else {
		std::cout << "El servidor se ha vinculado correctamente"<<std::endl;
		onRoad = true;

		// Add the listener to the selector
		selector->Add(&listener->GetListener());
	}

}
//Server Loop
void Server::ControlServidor()
{
	// Create a socket to listen to new connections
	ServerListener();

	// Endless loop that waits for new connections
	while (onRoad)
	{
		// Make the selector wait for data on any socket
		if (selector->Wait())
		{
			// Test the listener
			if (selector->isReady(&listener->GetListener()))
			{
				// The listener is ready: there is a pending connection
				tcpSocket = new TCPSocket();

				if (listener->Accept(tcpSocket->GetSocket()) == sf::Socket::Done)
				{
					std::cout << "Llega el cliente con puerto: " << tcpSocket->GetRemotePort() << std::endl;
					std::cout << "El cliente con puerto: " << tcpSocket->GetRemotePort() << " queda a la espera junto con " << clients.size() << " clientes mas" << std::endl;

					SendClients(*tcpSocket);
					SendNewClient(*tcpSocket);
					selector->Add(tcpSocket->GetSocket());
					clients.push_back(tcpSocket);


				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					std::cout << "Error al recoger conexión nueva\n";
					delete tcpSocket;
				}
			}
		}
	}
}



std::string Server::EnumToString(LISTENER _listener) {
	if (_listener == ENVIAR_CLIENTESACTUALES) {
		return "ENVIAR_CLIENTESACTUALES";
	}
	else if (_listener == ENVIAR_NUEVOCLIENTE) {
		return "ENVIAR_NUEVOCLIENTE";

	}
}
LISTENER Server::StringToEnum(std::string _string) {
	if (_string == "ENVIAR_CLIENTESACTUALES") {
		return ENVIAR_CLIENTESACTUALES;
	}
	else if (_string == "ENVIAR_NUEVOCLIENTE") {
		return ENVIAR_NUEVOCLIENTE;
	}
}
