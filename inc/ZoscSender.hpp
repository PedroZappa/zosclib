#ifndef ZOSC_SENDER_H
#define ZOSC_SENDER_H

#include "Zosc.hpp"

/**
* @brief Class for sending OSC messages over UDP
* @details This class encapsulates the logic for sending OSC messages over UDP.
* It uses ASIO to handle the I/O operations.
*
* @class OscSender
* @ingroup OSC Tools
**/
class OscSender {
  public:
	// Constructor: Initialize with host and port for sending OSC messages
	OscSender(const std::string &host, uint16_t port);

	// Send a single OSC message
	void sendMessage(const ZoscMessage &message);
	// Send an OSC bundle
	void sendBundle(const ZoscBundle &bundle);
	// Send raw OSC data
	void sendRaw(const std::vector<uint8_t> &data);

  private:
	std::string _host;             // Host address to send OSC messages to
	uint16_t _port;                // Port to send OSC messages to
	boost::asio::io_context _ioContext;   // I/O context for asynchronous operations
	boost::asio::ip::udp::socket _socket; // UDP socket for sending data
	boost::asio::ip::udp::endpoint _endpoint; // Destination endpoint for sending messages
};

#endif // OSC_SENDER_HPP
