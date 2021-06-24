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



void Server::SendClients(TCPSocket& socket) {
	sf::Packet packet;
	packet << LISTENER::CONEXION_NUEVO_PLAYER;
	packet << clients.size();
	for (auto it : clients) {
		packet << it->GetRemotePort();
	}

	status->SetStatus(socket.Send(packet));
	if (status->GetStatus() == sf::Socket::Done) {
		std::cout << "El paquete de enviar clientes se ha enviado correctamente"<<std::endl;
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
	//		else
	//		{
	//			// The listener socket is not ready, test all other sockets (the clients)
	//			for (std::list<TCPSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
	//			{
	//				TCPSocket& client = **it;
	//				if (selector->isReady(&listener->GetListener()))
	//				{
	//					// The client has sent some data, we can receive it
	//					sf::Packet packet;
	//					status->SetStatus(client.Receive(packet));
	//					if (status->GetStatus() == sf::Socket::Done)
	//					{
	//						std::string strRec;
	//						packet >> strRec;

	//						if (clients.size() > 0) {
	//							if (strRec.at(0) == 'u' && strRec.at(1) == ':') {//si tiene u: antes es el nombre de usuario
	//								strRec.erase(strRec.begin(), strRec.begin() + 1);
	//								//ourClients[client.getRemotePort()]->userName = strRec;
	//							}
	//							else {
	//								//std::cout << "He recibido " << strRec << " del puerto " << client.getRemotePort() << " con username" << ourClients[client.getRemotePort()]->userName << std::endl;
	//								std::cout << std::endl;
	//							}
	//						}
	//					}
	//					else if (status->GetStatus() == sf::Socket::Disconnected)
	//					{
	//						//selector.remove(client-);
	//						std::cout << "Elimino el socket que se ha desconectado\n";
	//					}
	//					else
	//					{
	//						std::cout << "Error al recibir de " << client.GetSocket() << std::endl;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}






