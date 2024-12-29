#include "../inc/ZoscMessage.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <sstream>

/* ************************************************************************** */
/*                                Constructors                                */
/* ************************************************************************** */

/// @brief Construct a new ZoscMessage object
/// @param address The address of the OSC message
ZoscMessage::ZoscMessage(const std::string &address) : _address(address) {
}

/* ************************************************************************** */
/*                                  Getters                                   */
/* ************************************************************************** */

/// @brief Get the address of the OSC message
/// @return The address of the OSC message
const std::string &ZoscMessage::getAddress() const {
	return (_address);
}

/// @brief Get the arguments of the OSC message
/// @return The arguments of the OSC message
const std::vector<OscArg> &ZoscMessage::getArgs() const {
	return (_arguments);
}

/* ************************************************************************** */
/*                                  Setters                                   */
/* ************************************************************************** */

/// @brief Set the address of the OSC message
/// @param address The address of the OSC message
void ZoscMessage::setAddress(const std::string &address) {
	_address = address;
}

/// @brief Add an argument to the OSC message
/// @param arg The argument to add
void ZoscMessage::addArgument(const OscArg &arg) {
	_arguments.push_back(arg);
}

/* ************************************************************************** */
/*                                  Methods                                   */
/* ************************************************************************** */

/// @brief Serialize the OSC message into a string
/// @return The serialized OSC message
std::string ZoscMessage::serialize() const {
	std::ostringstream oss; // Output stream

	// Serialize the address
	oss << _address;
	// Align memory with '\0' padding
	size_t padding = ((4 - (_address.size() % 4)) % 4);
	oss.write("\0\0\0", padding);

	// Serialize the type tag string
	oss.put(',');
	for (const auto &arg : _arguments) {
		if (std::holds_alternative<int32_t>(arg)) {
			oss.put('i'); // Integer type tag
		} else if (std::holds_alternative<float>(arg)) {
			oss.put('f'); // Float type tag
		} else if (std::holds_alternative<std::string>(arg)) {
			oss.put('s'); // String type tag
		} else if (std::holds_alternative<std::vector<uint8_t>>(arg)) {
			oss.put('b'); // Blob type tag
		} else if (std::holds_alternative<uint64_t>(arg)) {
			oss.put('t'); // Time tag type
		}
	}
	// Align memory with '\0' padding
	padding = ((4 - ((_arguments.size() + 1) % 4)) % 4);
	oss.write("\0\0\0", padding);

	// Serialize the arguments
	for (const auto &arg : _arguments) {
		// Serialize int
		if (std::holds_alternative<int32_t>(arg)) {
			int32_t value = htonl(std::get<int32_t>(arg));
			oss.write(reinterpret_cast<const char *>(&value), sizeof(value));
			// Serialize float arg
		} else if (std::holds_alternative<float>(arg)) {
			float value = std::get<float>(arg);
			uint32_t networkValue;
			memcpy(&networkValue, &value, sizeof(networkValue));
			networkValue = htonl(networkValue);
			oss.write(reinterpret_cast<const char *>(&networkValue),
					  sizeof(networkValue));
			// Serialize string arg
		} else if (std::holds_alternative<std::string>(arg)) {
			const std::string &value = std::get<std::string>(arg);
			oss << value;
			// Align memory with '\0' padding
			padding = ((4 - (value.size() % 4)) % 4);
			oss.write("\0\0\0", padding);
			// Serialize blob arg
		} else if (std::holds_alternative<std::vector<uint8_t>>(arg)) {
			const auto &value = std::get<std::vector<uint8_t>>(arg);
			int32_t size = htonl(static_cast<int32_t>(value.size()));
			oss.write(reinterpret_cast<const char *>(&size), sizeof(size));
			oss.write(reinterpret_cast<const char *>(value.data()), value.size());
			// Align memory with '\0' padding
			padding = ((4 - (value.size() % 4)) % 4);
			oss.write("\0\0\0", padding);
		}
	}

	return (oss.str());
}

/// @brief Deserialize an OSC message from a string
/// @throw std::runtime_error if the OSC message is malformed
/// @return The deserialized OSC message
ZoscMessage ZoscMessage::deserialize(const std::string &data) {
	ZoscMessage message;
	size_t pos = 0;

	// Extract the address
	size_t addressEnd = data.find('\0', pos);
	if (addressEnd == std::string::npos)
		throw std::runtime_error("Malformed OSC message: Missing null "
								 "terminator for address.");
	message._address = data.substr(pos, (addressEnd - pos));
	pos = ((addressEnd + 4) & ~3); // Align to 4 bytes

	// Extract the type tag string
	if (data[pos] != ',')
		throw std::runtime_error("Malformed OSC message: Missing type tag "
								 "string.");
	size_t typeTagEnd = data.find('\0', pos);
	std::string typeTags = data.substr((pos + 1), (typeTagEnd - pos - 1));
	pos = ((typeTagEnd + 4) & ~3); // Align to 4 bytes

	// Extract the arguments
	for (char type : typeTags) {
		if (type == 'i') {
			// Extract Integer
			if (pos + sizeof(int32_t) > data.size())
				throw std::runtime_error("Malformed OSC message: Not enough "
										 "data for integer argument.");
			int32_t value;
			memcpy(&value, (data.data() + pos), sizeof(value));
			message._arguments.emplace_back(ntohl(value));
			pos += sizeof(value);
		} else if (type == 'f') {
			// Extract Float
			if (pos + sizeof(float) > data.size())
				throw std::runtime_error("Malformed OSC message: Not enough "
										 "data for float argument.");
			uint32_t networkValue;
			memcpy(&networkValue, (data.data() + pos), sizeof(networkValue));
			networkValue = ntohl(networkValue);
			float value;
			memcpy(&value, &networkValue, sizeof(value));
			message._arguments.emplace_back(value);
			pos += sizeof(networkValue);
		} else if (type == 's') {
			// Extract String
			size_t stringEnd = data.find('\0', pos);
			if (stringEnd == std::string::npos)
				throw std::runtime_error("Malformed OSC message: Missing null "
										 "terminator for string argument.");
			message._arguments.emplace_back(data.substr(pos, (stringEnd - pos)));
			pos = (stringEnd + 4) & ~3; // Align to 4 bytes
		} else if (type == 'b') {
			// Extract Blob
			if (pos + sizeof(int32_t) > data.size())
				throw std::runtime_error("Malformed OSC message: Not enough "
										 "data for blob size.");
			int32_t size;
			memcpy(&size, (data.data() + pos), sizeof(size));
			size = ntohl(size);
			pos += sizeof(size);

			if (pos + size > data.size())
				throw std::runtime_error("Malformed OSC message: Blob size "
										 "exceeds available data.");
			message._arguments.emplace_back(std::vector<uint8_t>(
				(data.begin() + pos), (data.begin() + pos + size)));
			pos += size;
			pos = ((pos + 3) & ~3); // Align to 4 bytes
		}
	}

	return (message);
}
