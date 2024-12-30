/// @file main.cpp
/// @brief Main entry point
/// @Author: Zedro

#include "../inc/zosc.hpp"
#include <algorithm>
#include <iostream>
#include <ostream>

int main(void) {
	ZoscMessage message("/test");
	message.addArgument(42);
	message.addArgument(3.14f);
	message.addArgument("Yo, whirl!");
	message.addArgument(123456789);
	message.addArgument(std::vector<uint8_t>{1, 2, 3, 4});

	const std::vector<OscArg> &args = message.getArgs();
	for (const auto &arg : args)
		std::cout << arg << std::endl;
		
	std::cout << "Done Looping" <<  std::endl;

	std::string serialData = message.serialize();
	std::cout << serialData << std::endl;

	ZoscMessage deSerial = deSerial.deserialize(serialData);
	std::cout << deSerial.getAddress() << std::endl;
	const std::vector<OscArg> &deSerialArgs = deSerial.getArgs();
	for (const auto &arg : deSerialArgs)
		std::cout << arg << std::endl;
	std::cout << deSerialArgs.size() << std::endl;

	return (0);
}

// int main() {
// 	// Create a message
// 	ZoscMessage message("/example");
// 	message.addArgument(42);
// 	message.addArgument(3.14f);
//
// 	// Create a bundle
// 	ZoscBundle bundle;
// 	bundle.addMessage(message);
//
// 	// Nested bundle
// 	ZoscBundle nestedBundle;
// 	nestedBundle.addMessage(message);
// 	bundle.addBundle(nestedBundle);
//
// 	// Serialize the bundle
// 	std::string serialized = bundle.serialize();
// 	std::cout << "Serialized Bundle: " << serialized << std::endl;
//
// 	// Deserialize the bundle
// 	// ZoscBundle deserializedBundle = ZoscBundle::deserialize(serialized);
// 	// std::cout << "Deserialized Bundle TimeTag: " << deserializedBundle.getTimeTag().getValue() << std::endl;
//
// 	return 0;
// }
