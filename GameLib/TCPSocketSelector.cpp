#include "TcpSocketSelector.h"

std::mutex mtx;

void TCPSocketSelector::Add(sf::TcpSocket* socket)
{
    mtx.lock();
    socketSelector.add(*socket);
    mtx.unlock();
}

void TCPSocketSelector::Add(sf::TcpListener* listener)
{
    mtx.lock();
    socketSelector.add(*listener);
    mtx.unlock();
}

void TCPSocketSelector::Remove(sf::TcpSocket* socket)
{
    mtx.lock();
    socketSelector.remove(*socket);
    mtx.unlock();
}

void TCPSocketSelector::Remove(sf::TcpListener* socket)
{
    mtx.lock();
    socketSelector.remove(*socket);
    mtx.unlock();
}

bool TCPSocketSelector::Wait()
{
    return socketSelector.wait();
}

bool TCPSocketSelector::isReady(sf::TcpListener* listener)
{
    return socketSelector.isReady(*listener);
}

bool TCPSocketSelector::isReady(sf::TcpSocket* socket)
{
    return socketSelector.isReady(*socket);
}


