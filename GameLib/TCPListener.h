#pragma once
#include <SFML\Network.hpp>

class TCPListener
{
private:
	sf::TcpListener tcpListener;

public:
	TCPListener();
	~TCPListener();

	sf::TcpListener& GetListener();
	sf::Socket::Status Listen(unsigned short port, const sf::IpAddress& ipAddress);
	sf::Socket::Status Accept(sf::TcpSocket*);
	void Disconnect();
};
