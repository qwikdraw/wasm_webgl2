#include "Client.hpp"

Client::Client() :
window(_window_target),
input(_input_target, &window),
player(&input, 42)
{
	if (socket.Connect(SERVER_IP, 3000) != Socket::success)
		std::cerr << "Error connecting to server" << std::endl;
	last_connect_time = std::chrono::system_clock::now();
}
