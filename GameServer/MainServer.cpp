#pragma once
#include "Server.h"

int main()
{
	srand(time(NULL));

	Server servidor;
	
	servidor.ControlServidor();

	return 0;

}