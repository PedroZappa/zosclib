#ifndef ZOSC_RECEIVER_HPP
#define ZOSC_RECEIVER_HPP

#include "ZoscBundle.hpp"
#include "ZoscMessage.hpp"
#include <asio.hpp>
#include <functional>
#include <iostream>
#include <thread>

/**
* @brief Class for receiving OSC messages over UDP
* @details This class encapsulates the logic for receiving OSC messages over UDP.
* It uses ASIO to handle the I/O operations. It also provides methods to set callbacks for received messages and bundles.
*
* @class ZoscReceiver
* @ingroup OSC Tools
**/
class ZoscReceiver {
  public:
	// Constructor & Destructor
	ZoscReceiver(uint16_t port);
	~ZoscReceiver();

	// Start listening for incoming messages
	void start();
	// Stop listening
	void stop();

	// Set callback for received messages
	void
	setMessageCallback(const std::function<void(const ZoscMessage &)> &callback);

	// Set callback for received bundles
	void setBundleCallback(const std::function<void(const ZoscBundle &)> &callback);

  private:
	uint16_t _port;                      // Port to listen on
	asio::io_context _ioContext;         // ASIO I/O context
	asio::ip::udp::socket _socket;       // UDP socket for receiving data
	std::thread _ioThread;               // Thread for running I/O context
	bool _running;                       // Flag to track receiver state
	std::vector<uint8_t> _receiveBuffer; // Buffer for receiving data

	// User-defined callbacks
	std::function<void(const ZoscMessage &)> _messageCallback;
	std::function<void(const ZoscBundle &)> _bundleCallback;

	void receive(); // Internal method to handle receiving data
	void processData(const std::vector<uint8_t> &data); // Parse incoming data
};

#endif // ZOSC_RECEIVER_HPP
