#ifndef ZOSC_SENDER_H
#define ZOSC_SENDER_H

#include "zosc.hpp"

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
	asio::io_context _ioContext;   // I/O context for asynchronous operations
	asio::ip::udp::socket _socket; // UDP socket for sending data
	asio::ip::udp::endpoint _endpoint; // Destination endpoint for sending messages
};

#endif // OSC_SENDER_HPP
