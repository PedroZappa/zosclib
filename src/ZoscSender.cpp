#include "../inc/ZoscSender.hpp"

/* ************************************************************************** */
/*                                Constructors                                */
/* ************************************************************************** */

/// @brief Constructor to initialize the host, port, and socket
OscSender::OscSender(const std::string &host, uint16_t port)
	: _host(host), _port(port), _socket(_ioContext) {
	// Set up the endpoint to send data to the specified host and port
	_endpoint =
		asio::ip::udp::endpoint(asio::ip::address::from_string(host), port);

	// Open the UDP socket
	_socket.open(asio::ip::udp::v4());
}

/* ************************************************************************** */
/*                                  Methods                                   */
/* ************************************************************************** */

// Serialize and send an OSC message
void OscSender::sendMessage(const ZoscMessage &message) {
	// Serialize the OSC message to a string
	std::string serialMsg = message.serialize();

	// Convert the string to a byte array for transmission
	std::vector<uint8_t> data(serialMsg.begin(), serialMsg.end());

	// Send the data using the socket
	sendRaw(data);
}

// Serialize and send an OSC bundle
void OscSender::sendBundle(const ZoscBundle &bundle) {
	// Serialize the OSC bundle to a string
	std::string serialBundle = bundle.serialize();

	// Convert the string to a byte array for transmission
	std::vector<uint8_t> data(serialBundle.begin(), serialBundle.end());

	// Send the data using the socket
	sendRaw(data);
}

// Send raw data via UDP socket
void OscSender::sendRaw(const std::vector<uint8_t> &data) {
	try {
		// Send data to the endpoint (host and port)
		_socket.send_to(asio::buffer(data), _endpoint);
	} catch (const std::exception &e) {
		std::cerr << "Error sending data: " << e.what() << std::endl;
	}
}
