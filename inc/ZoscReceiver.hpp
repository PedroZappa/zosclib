#ifndef ZOSC_RECEIVER_HPP
#define ZOSC_RECEIVER_HPP

#include "ZoscBundle.hpp"
#include "ZoscMessage.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <thread>

#define BUFFER_SIZE 2048

/**
 * @brief Class for receiving OSC messages over UDP
 * @details This class encapsulates the logic for receiving OSC messages over
 *UDP. It uses ASIO to handle the I/O operations. It also provides methods to
 *set callbacks for received messages and bundles.
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

	// Set callbacks for received messages & bundles
	void setMessageCallback(
		const std::function<void(const ZoscMessage &, const std::string &, uint16_t)>
			&callback);
	void setBundleCallback(
		const std::function<void(const ZoscBundle &, const std::string &, uint16_t)>
			&callback);

  private:
	uint16_t _port;                       // Port to listen on
	boost::asio::io_context _ioContext;   // ASIO I/O context
	boost::asio::ip::udp::socket _socket; // UDP socket for receiving data
	std::vector<std::thread> _ioThreads;  // Threads for running I/O context
	bool _running;                        // Flag to track receiver state
	alignas(8) std::vector<uint8_t> _receiveBuffer; // Buffer for receiving data
	std::string _senderAddress; // Stores the sender's IP address
	uint16_t _senderPort;       // Stores the sender's port

	// User-defined callbacks
	std::mutex _callbackMutex;
	std::function<void(const ZoscMessage &, const std::string &, uint16_t)> _messageCallback;
    std::function<void(const ZoscBundle &, const std::string &, uint16_t)> _bundleCallback;

	void receive(); // Internal method to handle receiving data
	void processData(const std::vector<uint8_t> &data); // Parse incoming data
};

#endif // ZOSC_RECEIVER_HPP
