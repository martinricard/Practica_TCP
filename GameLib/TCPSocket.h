#pragma once
#include <SFML/Network.hpp>
#include <mutex>
#include <string>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <map>
#include "Types.h"
#include <chrono>
#include <cstdlib>
#include <list>
#include <mutex>
#include <random>
#include <vector>
#include <numeric>
#include <windows.h>

class TCPSocket
{
	sf::TcpSocket* tcpSocket;
	int id;
	bool ready;

public:
	std::string nombreSala;
	std::string password;
	int numeroJugadores;
	sf::Socket::Status tcpStatus;
	TCPSocket();
	TCPSocket(sf::TcpSocket* _tcpSocket);
	~TCPSocket();
	void SetID(int _id);
	int GetID();


	sf::TcpSocket* GetSocket();

	void SetSocket(sf::TcpSocket*);

	sf::IpAddress GetRemoteIp();

	
	std::string GetRemoteAddress();

	unsigned short GetRemotePort();

	sf::Socket::Status Connect(const std::string localhost, unsigned short port, sf::Time sec);

	void Disconnect();

	unsigned short GetLocalPort();

	sf::Socket::Status Receive(sf::Packet& pack);

	sf::Socket::Status Send(sf::Packet pack);

	bool GetReady();
	
	void SetReady(bool _ready);

};

