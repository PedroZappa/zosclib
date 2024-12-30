/**
 * @defgroup osc_core Core OSC Components
 * @{
 */

#include "../inc/Zosc.hpp"

/* ************************************************************************** */
/*                                ZoscTimeTag                                 */
/* ************************************************************************** */

/// @brief Construct a new ZoscTimeTag object
/// @param value The value of the ZoscTimeTag
ZoscBundle::ZoscTimeTag::ZoscTimeTag() : _value(0) {
}

ZoscBundle::ZoscTimeTag::ZoscTimeTag(uint64_t value) : _value(value) {
}

/* ************************************************************************** */
/*                                  Getters                                   */
/* ************************************************************************** */

uint64_t ZoscBundle::ZoscTimeTag::getValue() const {
	return (_value);
}

/// @brief Get the current time as a ZoscTimeTag (NTP/nano-second precision)
ZoscBundle::ZoscTimeTag ZoscBundle::ZoscTimeTag::now() {
	auto now = std::chrono::system_clock::now(); // Get current time
	// Get number of seconds since last UNIX epoch
	auto secs =
		std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch())
			.count();
	// Get microseconds since last second
	auto usecs = std::chrono::duration_cast<std::chrono::nanoseconds>(
					 now.time_since_epoch())
					 .count() %
		1'000'000'000;
	// Convert to NTP
	uint64_t ntpSecs = (static_cast<uint64_t>(secs) + NTP_DIFF);
	// Get NTP fraction
	uint64_t ntpFrac =
		(static_cast<uint64_t>(usecs * (1LL << 32)) / 1'000'000'000);

	return (ZoscTimeTag(ntpSecs << 32 | ntpFrac));
}

/* ************************************************************************** */
/*                                 ZoscBundle */
/* ************************************************************************** */

/// @brief Construct a new ZoscBundle object
ZoscBundle::ZoscBundle(void) : _timeTag(ZoscTimeTag::now()) {
}

/// @brief Construct a new ZoscBundle object
/// @param _timeTag The time tag of the OSC bundle
ZoscBundle::ZoscBundle(const ZoscTimeTag &timeTag) : _timeTag(timeTag) {
}

/* ************************************************************************** */
/*                                  Getters                                   */
/* ************************************************************************** */

/// @brief Get the time tag of the OSC bundle
const ZoscBundle::ZoscTimeTag &ZoscBundle::getTimeTag() const {
	return (_timeTag);
}

/// @brief Get the elements of the OSC bundle
/// @return The elements of the OSC bundle
const std::vector<std::variant<ZoscMessage, ZoscBundle>> &
ZoscBundle::getElements() const {
	return (_elements);
}

/* ************************************************************************** */
/*                                  Setters                                   */
/* ************************************************************************** */

/// @brief Set the time tag of the OSC bundle
/// @param _timeTag The time tag of the OSC bundle
void ZoscBundle::setTimeTag(const ZoscTimeTag &timeTag) {
	this->_timeTag = timeTag;
}

/// @brief Add a message to the OSC bundle
/// @param message The message to add
void ZoscBundle::addMessage(const ZoscMessage &message) {
	_elements.emplace_back(message);
}

/// @brief Add a bundle to the OSC bundle
/// @param bundle The bundle to add
void ZoscBundle::addBundle(const ZoscBundle &bundle) {
	_elements.emplace_back(bundle);
}

/* ************************************************************************** */
/*                                  Methods                                   */
/* ************************************************************************** */

/// @brief Serialize the OSC bundle
/// @return The serialized OSC bundle
std::string ZoscBundle::serialize() const {
	std::ostringstream oss;

	// Add the OSC bundle identifier
	oss << "#bundle";
	size_t padding = ((4 - (7 % 4)) % 4);
	oss.write("\0\0\0", padding);

	// Serialize the TimeTag
	uint64_t networkTimeTag =
		htonl(static_cast<uint32_t>(_timeTag.getValue() >> 32));
	uint32_t lowerBits =
		htonl(static_cast<uint32_t>(_timeTag.getValue() & 0xFFFFFFFF));
	oss.write(reinterpret_cast<const char *>(&networkTimeTag),
			  sizeof(networkTimeTag));
	oss.write(reinterpret_cast<const char *>(&lowerBits), sizeof(lowerBits));

	// Serialize elements
	for (const auto &element : _elements) {
		std::string serializedElement;
		if (std::holds_alternative<ZoscMessage>(element))
			serializedElement = std::get<ZoscMessage>(element).serialize();
		else if (std::holds_alternative<ZoscBundle>(element))
			serializedElement = std::get<ZoscBundle>(element).serialize();

		// Write size of the element (aligned to 4 bytes)
		uint32_t size = htonl(static_cast<uint32_t>(serializedElement.size()));
		oss.write(reinterpret_cast<const char *>(&size), sizeof(size));
		oss.write(serializedElement.data(), serializedElement.size());
	}

	return (oss.str());
}

ZoscBundle ZoscBundle::deserialize(const std::string &data) {
	if ((data.size() < 16) || (data.substr(0, 7) != "#bundle"))
		throw std::runtime_error("Malformed OSC bundle: Missing #bundle "
								 "identifier.");

	ZoscBundle bundle;
	size_t pos = 8; // Skip "#bundle" and align to 8 bytes

	// Extract TimeTag
	uint32_t upperBits, lowerBits;
	memcpy(&upperBits, data.data() + pos, sizeof(upperBits));
	memcpy(&lowerBits, data.data() + pos + sizeof(upperBits), sizeof(lowerBits));
	uint64_t timeTag =
		(static_cast<uint64_t>(ntohl(upperBits)) << 32) | ntohl(lowerBits);
	bundle.setTimeTag(ZoscTimeTag(timeTag));
	pos += 8;

	// Extract elements
	while (pos < data.size()) {
		if ((pos + sizeof(uint32_t)) > data.size())
			throw std::runtime_error("Malformed OSC bundle: Missing element "
									 "size.");
		// Measure element size
		uint32_t size;
		memcpy(&size, (data.data() + pos), sizeof(size));
		size = ntohl(size);
		pos += sizeof(size);
		if ((pos + size) > data.size())
			throw std::runtime_error("Malformed OSC bundle: Element size "
									 "exceeds available data.");
		// Deserialize element
		std::string elementData = data.substr(pos, size);
		pos += size;
		// Align to 4 bytes
		pos = (pos + 3) & ~3;

		// Determine element type
		if (elementData.substr(0, 7) == "#bundle") {
			bundle.addBundle(ZoscBundle::deserialize(elementData));
		} else {
			bundle.addMessage(ZoscMessage::deserialize(elementData));
		}
	}

	return (bundle);
}
/** @} */
