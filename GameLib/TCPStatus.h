#pragma once
#include <SFML/Network.hpp>
class TCPStatus
{
private:
	sf::Socket::Status status;
public:
	TCPStatus();
	sf::Socket::Status GetStatus();
	sf::Socket::Status SetStatus(sf::Socket::Status _status);
};