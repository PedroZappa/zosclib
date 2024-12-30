/// @file main.cpp
/// @brief Main entry point
/// @Author: Zedro

#include "inc/ZoscBundle.hpp"
#include "inc/ZoscMessage.hpp"
#include "inc/ZoscSender.hpp"
#include "inc/zosc.hpp"
#include <csignal>

// #define IP "127.0.0.1"
#define IP "192.168.1.168"
// #define IP "192.168.1.138"
#define PORT 9000

static volatile bool _listening = true;

static void SIGINT_handler(int sig);

int main(void) {
	ZoscMessage message("/test");
	message.addArgument(42);
	message.addArgument(3.14f);
	message.addArgument("Yo, whirl!");
	message.addArgument(std::vector<uint8_t>{1, 2, 3, 4});

	signal(SIGINT, &SIGINT_handler);

	const std::vector<OscArg> &args = message.getArgs();
	for (const auto &arg : args)
		std::cout << arg << std::endl;

	std::cout << "Done Looping" << std::endl;

	std::string serialData = message.serialize();
	std::cout << serialData << std::endl;

	ZoscMessage deSerial = deSerial.deserialize(serialData);
	std::cout << deSerial.getAddress() << std::endl;
	const std::vector<OscArg> &deSerialArgs = deSerial.getArgs();
	for (const auto &arg : deSerialArgs)
		std::cout << arg << std::endl;
	std::cout << deSerialArgs.size() << std::endl;

	// Create a sender
	std::cout << "Sending message" << std::endl;
	OscSender sender(IP, PORT);
	message.addArgument(42);
	sender.sendMessage(message);
	std::cout << "Message sent" << std::endl;

	// Listen for messages
	ZoscReceiver receiver(PORT);
	receiver.setMessageCallback([](const ZoscMessage &msg) {
		std::cout << "Received message: " << msg.getAddress() << std::endl;
	});
	while (_listening) {
		receiver.start();
	}
	receiver.stop();

	return (0);
}

/// @brief SIGINT handler
/// @param sig Signal number
static void SIGINT_handler(int sig) {
	(void)sig;
	_listening = false;
}
