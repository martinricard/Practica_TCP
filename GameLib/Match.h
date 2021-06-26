#pragma once
#include "TCPSocket.h"
#include <list>
struct Match {
	std::vector<TCPSocket*>clients;
};