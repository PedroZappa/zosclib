#include "../inc/zosc.hpp"

const t_debug_msg dbg_msg[6] = {{" INFO", SHOW_MSG, 5, "(ii) ", BBLU},
								{" START", FSTART, 6, "(>>) ", GRN},
								{" END", FEND, 4, "(<<) ", MAG},
								{" ERROR", ERROR, 6, "(xx) ", RED},
								{" SUCCESS", SUCCESS, 8, "(🖔🖔)", BGRN},
								{NULL, 0, 0, NULL, NULL}};

/// @brief Print a debug message to standard error
/// @param className The name of the class
/// @param funcName The name of the function
/// @param status The status of the message
/// @param customMsg A custom message
void debug(const std::string &className,
		   const std::string &funcName,
		   int status,
		   const std::string &customMsg) {
	// Find the appropriate debug message entry
	const t_debug_msg *msg_entry = NULL;
	for (int i = 0; dbg_msg[i].msg != NULL; i++) {
		if (dbg_msg[i].dbg_stat == status) {
			msg_entry = &dbg_msg[i];
			break;
		}
	}
	// Check if status is valid
	if (!msg_entry) {
		std::cerr << RED "Invalid status." NC << std::endl;
		return;
	}
	// Build the message using ostringstream
	std::ostringstream msg;
	msg << msg_entry->color      // Color code
		<< msg_entry->msg_header // Header like "(ii) "
		<< "[" << className << "::" << funcName
		<< "] "            // Class and function name
		<< msg_entry->msg; // Status message
	if (!customMsg.empty()) {
		msg << " -> " << customMsg;
	}
	// Print to standard error and reset color
	std::cerr << msg.str() << NC << std::endl;
}
