#ifndef ZOSC_BUNDLE_HPP
#define ZOSC_BUNDLE_HPP

#include "ZoscMessage.hpp"
#include <cstdint>
#include <variant>
#include <vector>

#define NTP_DIFF 2208988800

/**
 * @brief Encapsulates an OSC bundle
 * @details This class represents an OSC bundle, composed of a time tag and a
 * vector of messages and/or bundles.
 *
 * @class ZoscBundle
 * @ingroup OSC Data Types
 * **/
class ZoscBundle {
  public:
	/**
	 * @class OscTimeTag
	 * @brief NTP-compatible timestamp for OSC bundles
	 * @details 64-bit fixed-point time tag, upper 32 bits for seconds since 1900
	 */
	class ZoscTimeTag {
	  public:
		ZoscTimeTag();
		explicit ZoscTimeTag(uint64_t value);

		uint64_t getValue() const;
		static ZoscTimeTag now();

	  private:
		uint64_t _value; // 64-bit NTP timestamp
	};

	// Constructors
	ZoscBundle();
	explicit ZoscBundle(const ZoscTimeTag &timeTag);

	// Getters
	const ZoscTimeTag &getTimeTag() const;
	const std::vector<std::variant<ZoscMessage, ZoscBundle>> &getElements() const;

	// Setters
	void setTimeTag(const ZoscTimeTag &timeTag);
	void addMessage(const ZoscMessage &message);
	void addBundle(const ZoscBundle &bundle);

	// Serialization and Deserialization
	std::string serialize() const;
	static ZoscBundle deserialize(const std::string &data);

  private:
	ZoscTimeTag _timeTag; // TimeTag for scheduling
	std::vector<std::variant<ZoscMessage, ZoscBundle>>
		_elements; // Nested messages and bundles
};

#endif // ZOSC_BUNDLE_HPP
