#include "Client.h"

Client::Client()
{


}

Client::~Client()
{
}
//Se introduce el username del cliente
void Client::Username() {
	LineCout();

	std::cout <<std::endl<< "                           CLIENT                         " << std::endl;
	LineCout();
	std::cout << "Introduce your username: ";
	std::cin >> username;
}
//Se une a la partida si escribe y, sino se cierra la consola
void Client::JoinGame() {
	std::string confirmation;
	std::cout << std::endl << "Do you want to join or create a game? (y/n): ";
	std::cin >> confirmation;
	if (confirmation == "y" || confirmation == "Y") {
		system("cls");
	}
	else {
		exit(0);
	}
}

//Cuando el cliente recibe un manageChallenge_Q, lo recibe, solucionar el challenge se hace en la parte donde recibe el paquete


void Client::LineCout() {
	std::cout << std::endl<<"-------------------------------------------------------------" << std::endl;

}

//Conseguimos un random float para la perdida de paquetes
static float GetRandomFloat() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(0.f, 1.f);
	return dis(gen);
}







//Aqui sera donde reciba todos los paquetes y los maneje dependiendo de su tag
void Client::RecievingThread() {//Escucha los paquetes que envia el servidor
	sf::Packet packet;
	sf::IpAddress ip;
	unsigned short port;
	std::string message;
	int recieverInt;

	while (true)
	{
		//Tenemos un porcentaje de perdida para los paquetes
		//tcpSocket->tcpStatus = tcpSocket->Receive(packet, ip, port);
		packet >> recieverInt;
		if (tcpSocket->tcpStatus == sf::Socket::Done) {
			switch (recieverInt) {


			}
		}



		else {
			std::cout << "Se ha perdido el paquete";
		}

	}
}
		
		

void Client::SendingThread() {//Envia los paquetes
	sf::Packet packet;
	sf::IpAddress ip = sf::IpAddress::LocalHost;
	unsigned short port = SERVER_PORT;//Modificar este magic number
	std::string message;
	while (true)
	{
		if (protocolConnected) {
			std::cout << std::endl << "Escribe el mensaje que quieras enviar: ";
			std::cin >> message;
			packet.clear();

			if (tcpSocket->tcpStatus == sf::Socket::Done) {
				std::cout << std::endl << "Se ha enviado: " << message << std::endl;
				packet.clear();

				message = "";
				//std::cout << std::endl << "Escribe el mensaje que quieras enviar: ";

			}
			else {
				std::cout << "Ha habido un error enviando el paquete";
			}
		}
	
	}
}

void Client::ClientLoop()
{

	Username();
	JoinGame();

	std::thread tRecieve(&Client::RecievingThread, this);
	tRecieve.detach();
	std::thread tSend(&Client::SendingThread, this);
	tSend.detach();
	
	
	while (true)
	{
	if (protocolConnected) {
		
		
		}
	}
}
