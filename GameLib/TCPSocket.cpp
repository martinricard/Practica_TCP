#include "TCPSocket.h"

TCPSocket::TCPSocket()
{
    tcpSocket = new sf::TcpSocket;
}

TCPSocket::TCPSocket(sf::TcpSocket* _tcpSocket)
{
    _tcpSocket = new sf::TcpSocket;
}

TCPSocket::~TCPSocket()
{
    delete[] this->tcpSocket;
}

void TCPSocket::SetID(int _id)
{
    id = _id;
}

int TCPSocket::GetID()
{
    return id;
}

sf::TcpSocket* TCPSocket::GetSocket()
{
    return tcpSocket;
}

void TCPSocket::SetSocket(sf::TcpSocket* _tcpSocket) {
    tcpSocket = _tcpSocket;
}

sf::IpAddress TCPSocket::GetRemoteIp() 
{
    return tcpSocket->getRemoteAddress();
}

std::string TCPSocket::GetRemoteAddress()
{
    return tcpSocket->getRemoteAddress().toString();
}

unsigned short TCPSocket::GetRemotePort() {
    return tcpSocket->getRemotePort();
}
unsigned short TCPSocket::GetLocalPort() {
    return tcpSocket->getLocalPort();
}
sf::Socket::Status TCPSocket::Connect(const std::string localhost, unsigned short port, sf::Time sec) {
    return tcpSocket->connect(localhost, port, sec);
}
void TCPSocket::Disconnect()
{
    tcpSocket->disconnect();
}

sf::Socket::Status TCPSocket::Receive(sf::Packet& pack)
{
  
    return tcpSocket->receive(pack);
}

sf::Socket::Status TCPSocket::Send(sf::Packet pack)
{
    return tcpSocket->send(pack);
}

