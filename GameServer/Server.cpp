#include "Server.h"



Server::Server()
{
	this->tcpSocket = new TCPSocket();

}

Server::~Server()
{
}





//Server Loop
void Server::ControlServidor()
{
	bool running = true;
	// Create a socket to listen to new connections
	sf::TcpListener listener;
	sf::Socket::Status status = listener.listen(50000);
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al abrir listener\n";
		exit(0);
	}

	// Create a selector
	sf::SocketSelector selector;
	// Add the listener to the selector
	selector.add(listener);
	// Endless loop that waits for new connections
	while (running)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				TCPSocket* client = new TCPSocket;

				if (listener.accept(*client->GetSocket()) == sf::Socket::Done)
				{
					if (clients.size() >= 1) {//si hay mas conectarse
						// Add the new client to the clients list
						std::cout << "Llega el cliente con puerto: " << client->GetRemotePort() << std::endl;
						std::cout << "El cliente con puerto: " << client->GetRemotePort() << " queda a la espera junto con " << clients.size() << " clientes mas" << std::endl;

						//conectar este client con los que ya hay por TCP
						std::list<TCPSocket*>::iterator it = clients.begin();
						sf::Packet packet;
						packet << LISTENER::CONEXION_NUEVO_PLAYER;
						packet << clients.size();

						for (it; it != clients.end(); it++) {
							packet << client->GetRemotePort();

							if (status == sf::Socket::Done) {
								tcpSocket->Send(packet);
							}
						}
						packet.clear();



						//Client* newClient = new Client("none", client->GetSocket(), client->GetRemotePort());
						//ourClients[client->GetRemotePort()] = newClient;

						clients.push_back(client);
						// Add the new client to the selector so that we will
						// be notified when he sends something
						selector.add(*client->GetSocket());
					}
					else {//si esta solo esperar




						// Add the new client to the clients list
						std::cout << "Llega el cliente con puerto: " << client->GetRemotePort() << std::endl;
						//Client* newClient = new Client("none", client->GetSocket(), client->GetRemotePort());
						//ourClients[client->GetRemotePort()] = newClient;
						clients.push_back(client);
						// Add the new client to the selector so that we will
						// be notified when he sends something
						selector.add(*client->GetSocket());
					}





				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					std::cout << "Error al recoger conexi�n nueva\n";
					delete client;
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				for (std::list<TCPSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					TCPSocket& client = **it;
					if (selector.isReady(listener))
					{
						// The client has sent some data, we can receive it
						sf::Packet packet;
						status = client.Receive(packet);
						if (status == sf::Socket::Done)
						{
							std::string strRec;
							packet >> strRec;

							if (clients.size() > 0) {
								if (strRec.at(0) == 'u' && strRec.at(1) == ':') {//si tiene u: antes es el nombre de usuario
									strRec.erase(strRec.begin(), strRec.begin() + 1);
									//ourClients[client.getRemotePort()]->userName = strRec;
								}
								else {
									//std::cout << "He recibido " << strRec << " del puerto " << client.getRemotePort() << " con username" << ourClients[client.getRemotePort()]->userName << std::endl;
									std::cout << std::endl;
								}
							}
						}
						else if (status == sf::Socket::Disconnected)
						{
							//selector.remove(client-);
							std::cout << "Elimino el socket que se ha desconectado\n";
						}
						else
						{
							std::cout << "Error al recibir de " << client.GetSocket() << std::endl;
						}
					}
				}
			}
		}
	}

}




