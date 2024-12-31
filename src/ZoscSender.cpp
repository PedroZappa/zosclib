/**
 * @defgroup zosc_network Network OSC Components
 * @{
 */

#include "../inc/ZoscSender.hpp"

/* ************************************************************************** */
/*                                Constructors                                */
/* ************************************************************************** */

/// @brief Constructor to initialize the host, port, and socket
/// @param host The host address to send OSC messages to
/// @param port The port to send OSC messages to
OscSender::OscSender(const std::string &host, uint16_t port)
	: _host(host), _port(port), _socket(_ioContext) {
	// Set up the endpoint to send data to the specified host and port
	_endpoint =
		asio::ip::udp::endpoint(asio::ip::address::from_string(host), port);

	// Open the UDP socket

	try {
		_socket.open(asio::ip::udp::v4());
		std::cout << "Socket opened successfully for host: " << _host
				  << " on port: " << _port << std::endl;
	} catch (const std::exception &e) {
		std::cerr << "Error opening socket: " << e.what() << std::endl;
		throw;
	}
}

/* ************************************************************************** */
/*                                  Methods                                   */
/* ************************************************************************** */

/// @brief Send an OSC message
/// @param message The OSC message to send
void OscSender::sendMessage(const ZoscMessage &message) {
	// Serialize the OSC message to a string
	std::string serialMsg = message.serialize();
	std::cout << "Serialized message: " << serialMsg << std::endl;
	// Convert the string to a byte array for transmission
	std::vector<uint8_t> data(serialMsg.begin(), serialMsg.end());
	// Send the data using the socket
	std::cout << "Sending to endpoint: " << _endpoint.address().to_string()
			  << ":" << _endpoint.port() << std::endl;
	sendRaw(data);
}

/// @brief Send an OSC bundle
/// @param bundle The OSC bundle to send
void OscSender::sendBundle(const ZoscBundle &bundle) {
	// Serialize the OSC bundle to a string
	std::string serialBundle = bundle.serialize();
	// Convert the string to a byte array for transmission
	std::vector<uint8_t> data(serialBundle.begin(), serialBundle.end());
	// Send the data using the socket
	std::cout << "Sending to endpoint: " << _endpoint.address().to_string()
			  << ":" << _endpoint.port() << std::endl;
	sendRaw(data);
}

/// @brief Send raw data over UDP
/// @param data The data to send
void OscSender::sendRaw(const std::vector<uint8_t> &data) {
	try {
		// Send data to the endpoint (host and port)
		std::size_t bytesSent = _socket.send_to(asio::buffer(data), _endpoint);
		std::cout << "Sent " << bytesSent << " bytes to "
				  << _endpoint.address().to_string() << ":" << _endpoint.port()
				  << std::endl;
	} catch (const std::exception &e) {
		std::cerr << "Error sending data: " << e.what() << std::endl;
	}
}
/** @} */
