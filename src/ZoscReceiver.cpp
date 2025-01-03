/**
 * @defgroup zosc_network Network OSC Components
 * @{
 */

#include "../inc/ZoscReceiver.hpp"

/* ************************************************************************** */
/*                                Constructor                                 */
/* ************************************************************************** */

/// @brief Constructor to initialize the UDP receiver
/// @param port The port to listen for OSC messages on
ZoscReceiver::ZoscReceiver(uint16_t port)
	: _port(port),
	  _socket(_ioContext,
			  boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
	  _running(false), _receiveBuffer(BUFFER_SIZE) {
}

/* ************************************************************************** */
/*                                Destructor                                  */
/* ************************************************************************** */

/// @brief Destructor ensures the receiver stops cleanly
ZoscReceiver::~ZoscReceiver() {
	stop();
}

/* ************************************************************************** */
/*                                  Methods                                   */
/* ************************************************************************** */

/// @brief Start the receiver
void ZoscReceiver::start() {
	if (_running)
		return;
	_running = true;

	// Start receiving data
	receive();

	// Run the I/O context in multiple threads
	size_t threadCount = std::thread::hardware_concurrency();
	for (size_t i = 0; i < threadCount; ++i) {
		_ioThreads.emplace_back([this]() { _ioContext.run(); });
	}
}

/// @brief Stop the receiver
void ZoscReceiver::stop() {
	if (!_running)
		return;
	_running = false;

	_ioContext.stop();
	for (auto &thread : _ioThreads) {
		if (thread.joinable())
			thread.join();
	}
	_ioThreads.clear();

	std::cout << "Receiver stopped" << std::endl;
}

/// @brief Set callback for received messages
/// @param callback The function to call when a message is received
/// @details The callback receives the message, sender's IP address, and port
void ZoscReceiver::setMessageCallback(
    const std::function<void(const ZoscMessage &, const std::string &, uint16_t)> &callback) {
    _messageCallback = callback;
}

/// @brief Set callback for received bundles
/// @param callback The function to call when a bundle is received
/// @details The callback receives the bundle, sender's IP address, and port
void ZoscReceiver::setBundleCallback(
    const std::function<void(const ZoscBundle &, const std::string &, uint16_t)> &callback) {
    _bundleCallback = callback;
}

/* ************************************************************************** */
/*                           Internal Methods                                 */
/* ************************************************************************** */

/// @brief Begin asynchronous receive
void ZoscReceiver::receive() {
	auto receiveBuffer =
		std::make_shared<std::vector<uint8_t>>(BUFFER_SIZE); // Unique buffer
	auto senderEndpoint = std::make_shared<boost::asio::ip::udp::endpoint>();

	_socket.async_receive_from(
		boost::asio::buffer(*receiveBuffer),
		*senderEndpoint,
		[this, receiveBuffer, senderEndpoint](std::error_code ec,
											  std::size_t bytesReceived) {
			if (ec) {
				fprintf(stderr, "Error: %s\n", ec.message().c_str());
				return;
			}
			// Extract sender's IP address and port
			_senderAddress = senderEndpoint->address().to_string();
			_senderPort = senderEndpoint->port();
#ifdef DEBUG
			std::cout << "Buffer address: " << receiveBuffer.get()
					  << ", size: " << bytesReceived << std::endl;
#endif
			if (bytesReceived > receiveBuffer->size()) {
				std::cerr << "Buffer overflow detected. Bytes received: "
						  << bytesReceived
						  << ", buffer size: " << receiveBuffer->size()
						  << std::endl;
				return;
			}

			std::vector<uint8_t> data(receiveBuffer->begin(),
									  receiveBuffer->begin() + bytesReceived);
			processData(data);

			if (_running)
				receive(); // Continue receiving
		});
}

/// @brief Process received data
/// @param data The raw data received
void ZoscReceiver::processData(const std::vector<uint8_t> &data) {
	try {
		// Try to deserialize as a message
		ZoscMessage message =
			ZoscMessage::deserialize(std::string(data.begin(), data.end()));

		// If successful, invoke the message callback
		std::lock_guard<std::mutex> lock(_callbackMutex);

		if (_messageCallback)
			_messageCallback(message, _senderAddress, _senderPort);
	} catch (...) {
		// Try to deserialize as a bundle
		try {
			ZoscBundle bundle =
				ZoscBundle::deserialize(std::string(data.begin(), data.end()));

			// If successful, invoke the bundle callback
			std::lock_guard<std::mutex> lock(_callbackMutex);
			if (_bundleCallback)
				_bundleCallback(bundle, _senderAddress, _senderPort);
		} catch (const std::exception &e) {
			std::cerr << "Failed to process data: " << e.what() << std::endl;
		}
	}
}
/** @} */
