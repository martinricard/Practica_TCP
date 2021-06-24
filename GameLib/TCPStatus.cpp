#include "TCPStatus.h"

TCPStatus::TCPStatus()
{

}

sf::Socket::Status TCPStatus::GetStatus()
{
	return status;
}

sf::Socket::Status TCPStatus::SetStatus(sf::Socket::Status _status)
{
	return status = _status;
}