#include "../inc/ZoscBundle.hpp"
#include <chrono>

/* ************************************************************************** */
/*                                ZoscTimeTag                                 */
/* ************************************************************************** */

/// @brief Construct a new ZoscTimeTag object
/// @param value The value of the ZoscTimeTag
ZoscBundle::ZoscTimeTag::ZoscTimeTag() : value(0) {
}

ZoscBundle::ZoscTimeTag::ZoscTimeTag(uint64_t value) : value(value) {
}

/* ************************************************************************** */
/*                                  Getters                                   */
/* ************************************************************************** */

uint64_t ZoscBundle::ZoscTimeTag::getValue() const {
	return (value);
}

/// @brief Get the current time as a ZoscTimeTag (NTP/nano-second precision)
ZoscBundle::ZoscTimeTag ZoscBundle::ZoscTimeTag::now() {
	auto now = std::chrono::system_clock::now(); // Get current time
	// Get number of seconds since last UNIX epoch
	auto secs =
		std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch())
			.count();
	// Get microseconds since last second
	auto usecs = std::chrono::duration_cast<std::chrono::microseconds>(
					 now.time_since_epoch())
					 .count() %
		1'000'000'000;
	// Convert to NTP
	uint64_t ntpSecs = (static_cast<uint64_t>(secs) + NTP_DIFF);
	// Get NTP fraction
	uint64_t ntpFrac = (static_cast<uint64_t>(usecs) / 1'000'000'000);

	return (ZoscTimeTag(ntpSecs << 32 | ntpFrac));
}
