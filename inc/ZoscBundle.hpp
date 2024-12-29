#ifndef ZOSC_BUNDLE_HPP
#define ZOSC_BUNDLE_HPP

#include "ZoscMessage.hpp"
#include <cstdint>
#include <variant>
#include <vector>

class OscBundle {
  public:
	// Nested TimeTag class
	class OscTimeTag {
	  public:
		OscTimeTag();
		explicit OscTimeTag(uint64_t value);
		static OscTimeTag now();
		uint64_t getValue() const;

	  private:
		uint64_t value; // 64-bit NTP timestamp
	};

	// Constructors
	OscBundle();
	explicit OscBundle(const OscTimeTag &timeTag);

	// Getters and Setters
	const OscTimeTag &getTimeTag() const;
	void setTimeTag(const OscTimeTag &timeTag);

	void addMessage(const ZoscMessage &message);
	void addBundle(const OscBundle &bundle);

	const std::vector<std::variant<ZoscMessage, OscBundle>> &getElements() const;

	// Serialization and Deserialization
	std::string serialize() const;
	static OscBundle deserialize(const std::string &data);

  private:
	OscTimeTag timeTag; // TimeTag for scheduling
	std::vector<std::variant<ZoscMessage, OscBundle>>
		elements; // Nested messages and bundles
};

#endif // ZOSC_BUNDLE_HPP
