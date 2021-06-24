#pragma once
#include "TcpListener.h"
#include <SFML\Network.hpp>
#include <mutex>

class TCPSocketSelector
{
private:
	sf::SocketSelector socketSelector;

public:
	void Add(sf::TcpSocket*);
	void Add(sf::TcpListener*);

	void Remove(sf::TcpSocket*);
	void Remove(sf::TcpListener*);

	//Wait for connections
	bool Wait();

	//Wait for connections
	bool isReady(sf::TcpListener*);
	bool isReady(sf::TcpSocket*);
};