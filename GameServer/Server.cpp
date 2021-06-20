#include "Server.h"



Server::Server()
{
	this->tcpSocket = new TCPSocket();

}

Server::~Server()
{
}





//Server Loop
void Server::ServerLoop()
{

	//this->tcpSocket->Bind(50000);

	
	


	
	while (true)
	{
	//tcpSocket->tcpStatus = tcpSocket->Receive(packet, ip, port);


		if (tcpSocket->tcpStatus == sf::Socket::Done) {

		}
	

	


	}
}



//Enviamos las posiciones en un thread
