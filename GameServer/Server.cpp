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
		else if(status->GetStatus()==sf::Socket::Disconnected) {
			selector->Remove(it->GetSocket());
			int aux;
			for (int i = 0;i < clients.size();i++) {
				if (clients[i]->GetRemotePort() == it->GetRemotePort()) {
					aux = i;
				}
			}
			clients.erase(clients.begin() + aux);
			std::cout << "SE HA DESCONECTADO UN CLIENTE " << std::endl;
			
		}

	}
}

void Server::RecievingThread() {
	while (true) {
		if (clients.size() != 0) {
			for (auto it : clients) {
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
							if (tag == LISTENER::CREAR_PARTIDA) {
								for (auto it : clients) {
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
											partidas.push_back(partida);
											SendClients(*it, *partida);

											partida->clients.push_back(it);
											int aux;
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
							else if (tag == LISTENER::UNIRSE_PARTIDA) {
								std::string nameOfRoom;
								std::string password;
								packet >> nameOfRoom;
								packet >> password;

								if (partidas.size() != 0) {
									for (auto it2 : partidas) {
										if (it2->clients[0]->nombreSala == nameOfRoom && password == it2->clients[0]->password) {
											SendClients(*it, *it2);
											it2->clients.push_back(it);

											int aux;
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
							else if (tag == LISTENER::BUSCAR_PARTIDA) {
								std::string numeroJugadores;
								std::string conContra;
								int numeroJugadoresInt;
								packet >> numeroJugadores;
								packet >> conContra;
								if (numeroJugadores != "n") {
									numeroJugadoresInt = std::stoi(numeroJugadores);
								}
								int size = 0;
								if (partidas.size() != 0) {
									for (auto it2 : partidas) {
										if (numeroJugadores == "n") {
											if (conContra == "n" && it2->clients[0]->password == "none") {
												size++;
											}
											else if (conContra == "s" && it2->clients[0]->password != "none") {
												size++;
											}
										}

										else {
											if (numeroJugadoresInt == it2->clients[0]->numeroJugadores) {
												if (conContra == "n" && it2->clients[0]->password == "none") {
													size++;
												}
												else if (conContra == "s" && it2->clients[0]->password != "none") {
													size++;
												}
											}
										}
									}
									packet.clear();
									packet << EnumToString(BUSCAR_PARTIDA);
									packet << std::to_string(size);
									for (auto it2 : partidas) {
										if (numeroJugadores == "n") {
											if (conContra == "n" && it2->clients[0]->password == "none") {
												packet << it2->clients[0]->nombreSala;
												packet << std::to_string(it2->clients.size());
												packet << std::to_string(it2->clients[0]->numeroJugadores);
											}
											else if (conContra == "s" && it2->clients[0]->password != "none") {
												packet << it2->clients[0]->nombreSala;
												packet << std::to_string(it2->clients.size());
												packet << std::to_string(it2->clients[0]->numeroJugadores);
											}
										}

										else {
											if (numeroJugadoresInt == it2->clients[0]->numeroJugadores) {
												if (conContra == "n" && it2->clients[0]->password == "none") {
													packet << it2->clients[0]->nombreSala;
													packet << std::to_string(it2->clients.size());
													packet << std::to_string(it2->clients[0]->numeroJugadores);
												}
												else if (conContra == "s" && it2->clients[0]->password != "none") {
													packet << it2->clients[0]->nombreSala;
													packet << std::to_string(it2->clients.size());
													packet << std::to_string(it2->clients[0]->numeroJugadores);
												}
											}
										}
									}
									it->Send(packet);
								}



							}
							/*else if (auxiliar == "u") {
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
					}*/

						}
					}
				}

			}
		}
	}
}





void Server::SendClients(TCPSocket& socket, Match partida) {
	sf::Packet packet;
	unsigned short port;
	std::string stringPort;
	std::string tag = EnumToString(ENVIAR_CLIENTESACTUALES);
	packet << tag;
	if (partida.clients.size() != 0) {
		packet << std::to_string(partida.clients[0]->numeroJugadores);
	}
	packet << std::to_string(partida.clients.size());

	for (auto it : partida.clients) {
	

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

	std::thread t(&Server::RecievingThread, this);
	t.detach();
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

					//SendClients(*tcpSocket);
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
	else if (_listener == CAMBIO_CARTA) {
		return "CAMBIO_CARTA";

	}
}
LISTENER Server::StringToEnum(std::string _string) {
	if (_string == "ENVIAR_CLIENTESACTUALES") {
		return ENVIAR_CLIENTESACTUALES;
	}
	else if (_string == "ENVIAR_NUEVOCLIENTE") {
		return ENVIAR_NUEVOCLIENTE;
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
	else if (_string == "CAMBIO_CARTA") {
		return LISTENER::CAMBIO_CARTA;
	}
}
