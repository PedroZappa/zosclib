#ifndef ZOSC_DEBUG_HPP
#define ZOSC_DEBUG_HPP

#include "ansi.h"
#include <iostream>
#include <sstream>
#include <string>

/* ************************************************************************** */
/*                                   Enums                                    */
/* ************************************************************************** */

typedef enum e_debug {
	ERROR,
	FSTART,
	FEND,
	SUCCESS,
	SHOW_MSG,
} t_debug;

/* ************************************************************************** */
/*                                 Structures                                 */
/* ************************************************************************** */

typedef struct s_debug_msg {
	const char *msg;
	int dbg_stat;
	int len;
	const char *msg_header;
	const char *color;
} t_debug_msg;

/* ************************************************************************** */
/*                                   Macros                                   */
/* ************************************************************************** */

/// @brief Macro to print debug messages
#define _DEBUG(status, msg) debug(typeid(*this).name(), __func__, status, msg)

/* ************************************************************************** */
/*                            Function Prototypes                             */
/* ************************************************************************** */

void debug(const std::string &className,
		   const std::string &funcName,
		   int status,
		   const std::string &customMsg);

/* ************************************************************************** */
/*                                 Templates                                  */
/* ************************************************************************** */

/// @brief Template to print containers
/// @tparam C The container type
/// @param funcName The name of the function
/// @param contName The name of the container
/// @param cont The container
template <typename C>
void showContainer(const std::string &funcName,
				   const std::string &contName,
				   const C &cont) {
	// Use constant SHOW_MSG (which should correspond to INFO status in debug_msg)
	debug(typeid(cont).name(), funcName, SHOW_MSG, "Printing cont: " + contName);

	std::ostringstream output;
	typename C::const_iterator it;

	for (it = cont.begin(); it != cont.end(); ++it) {
		output << *it;
		typename C::const_iterator next = it;
		++next;
		if (next != cont.end())
			output << "\n";
	}

	std::cerr << output.str() << std::endl;
}

#endif
