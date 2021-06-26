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

void Server::RecievingThread() {
	while (true) {
		for (auto it : clientsSinAsignar) {
			if (selector->Wait()) {
				if (selector->isReady(it->GetSocket())) {
					sf::Packet packet;
					status->SetStatus(it->Receive(packet));
					if (status->GetStatus() == sf::Socket::Done)
					{
						std::string stringTag;
						packet >> stringTag;
						LISTENER tag = StringToEnum(stringTag);
						unsigned short port;
						std::string auxiliar;
						packet >> auxiliar;
						port = std::stoi(auxiliar);
						if (tag == LISTENER::DATOS_PARTIDA) {
							for (auto it : clientsSinAsignar) {
								std::cout << it->GetRemotePort();
								std::cout << port;
								if (port == it->GetRemotePort()) {
									packet >> auxiliar;
									if (auxiliar == "c") {
										packet >> auxiliar;
										it->nombreSala = auxiliar;
										std::cout << it->nombreSala;
										packet >> auxiliar;
										it->numeroJugadores = std::stoi(auxiliar);
										packet >> auxiliar;
										if (auxiliar == "s") {
											packet >> auxiliar;
											it->password = auxiliar;
										}
										else {
											it->password = "none";
										}
										Match* partida = new Match;
										partida->clients.push_back(it);

										partidas.push_back(partida);

									}
									else if (auxiliar == "u") {
										packet.clear();
										std::cout << "ENTRAMOS2";
										packet << EnumToString(LISTENER::DATOS_PARTIDA);
										if (partidas.size() != 0) {
											std::cout << "ENTRAMOS3";

											packet << std::to_string(partidas.size());
											for (auto it2 : partidas) {
												packet << it2->clients[0]->nombreSala;
												packet << std::to_string(it2->clients.size());
												packet << it2->clients[0]->numeroJugadores;

											}
											it->Send(packet);
										}
									}
								}
							}
						}
					}
				}
			}

		}
	}
}


//void Server::SendNewClient(TCPSocket& socket) {
//	sf::Packet packet;
//	unsigned short port = socket.GetRemotePort();;
//	std::string stringPort = std::to_string(port);;
//	
//
//	for (auto it : clients) {
//		packet << EnumToString(ENVIAR_NUEVOCLIENTE);
//		packet << stringPort;
//		status->SetStatus(it->Send(packet));
//		if (status->GetStatus() == sf::Socket::Done) {
//			std::cout << "El paquete de enviar clientes se ha enviado correctamente" << std::endl;
//			packet.clear();
//		}
//		else {
//			std::cout << "El paquete de enviar clientes no se ha enviado correctamente" << std::endl;
//		}
//	}
//}
//void Server::SendClients(TCPSocket& socket) {
//	sf::Packet packet;
//	unsigned short port;
//	std::string stringPort;
//	std::string tag = EnumToString(ENVIAR_CLIENTESACTUALES);
//	packet << tag;
//	packet << std::to_string(clients.size());
//
//	for (auto it : clients) {
//	
//
//		port =it->GetRemotePort();
//		stringPort = std::to_string(port);
//		packet << stringPort;
//
//		
//	}
//	status->SetStatus(socket.Send(packet));
//	if (status->GetStatus() == sf::Socket::Done) {
//		std::cout << "El paquete de enviar clientes se ha enviado correctamente" << std::endl;
//		packet.clear();
//	}
//	else {
//		std::cout << "El paquete de enviar clientes no se ha enviado correctamente" << std::endl;
//	}
//}

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

	std::thread Recieving(&Server::RecievingThread,this);
	Recieving.detach();
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
				TCPSocket* client = new TCPSocket();

				if (listener->Accept(client->GetSocket()) == sf::Socket::Done)
				{
					std::cout << "Llega el cliente con puerto: " << client->GetRemotePort() << std::endl;
					std::cout << "El cliente con puerto: " << client->GetRemotePort() << " queda a la espera junto con " << clientsSinAsignar.size() << " clientes mas" << std::endl;

					//SendClients(*tcpSocket);
					//SendNewClient(*tcpSocket);
					selector->Add(client->GetSocket());
					clientsSinAsignar.push_back(client);


				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					std::cout << "Error al recoger conexión nueva\n";
					delete client;
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
	else if (_listener == DATOS_PARTIDA) {
		return "DATOS_PARTIDA";


	}
}
LISTENER Server::StringToEnum(std::string _string) {
	if (_string == "ENVIAR_CLIENTESACTUALES") {
		return ENVIAR_CLIENTESACTUALES;
	}
	else if (_string == "ENVIAR_NUEVOCLIENTE") {
		return ENVIAR_NUEVOCLIENTE;
	}
	else if (_string == "DATOS_PARTIDA") {
		return LISTENER::DATOS_PARTIDA;
	}
}
