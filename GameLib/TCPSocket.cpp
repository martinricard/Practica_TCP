#include "TCPSocket.h"

TCPSocket::TCPSocket()
{
    tcpSocket = new sf::TcpSocket;
}

TCPSocket::TCPSocket(sf::TcpSocket* _tcpSocket)
{
    tcpSocket = _tcpSocket;
}

TCPSocket::~TCPSocket()
{
}

unsigned short TCPSocket::GetLocalPort()
{
    return 50000;
}



void TCPSocket::unBind()
{
    tcpSocket->disconnect();
}

//sf::Socket::Status TCPSocket::Receive(sf::Packet& pack, sf::IpAddress& ip, unsigned short& port)
//{
//    this->tcpStatus = tcpSocket->receive(pack, ip, port);
//    return tcpStatus;
//}
//
//sf::Socket::Status TCPSocket::Send(sf::Packet pack, sf::IpAddress ip, unsigned short port)
//{
//    this->tcpStatus = tcpSocket->send(pack, ip, port);
//    return tcpStatus;
//}
//
//sf::Socket::Status TCPSocket::Bind(unsigned short port)
//{
//    this->tcpStatus = tcpSocket->bind(port);
//    return tcpStatus;
//}
//
