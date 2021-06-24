#include "TCPListener.h"

TCPListener::TCPListener()
{
}

TCPListener::~TCPListener()
{
}

sf::TcpListener& TCPListener::GetListener()
{
    return tcpListener;
}

sf::Socket::Status TCPListener::Listen(unsigned short port, const sf::IpAddress& ipAddress)
{

    return tcpListener.listen(port, ipAddress);
}

sf::Socket::Status TCPListener::Accept(sf::TcpSocket* socket)
{
    return tcpListener.accept(*socket);
}

void TCPListener::Disconnect()
{
    tcpListener.close();
}
