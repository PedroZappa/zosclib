/**
 * @page osc_protocol OSC Protocol Implementation
 * Describes how ZOSC implements the OSC 1.0 specification:
 * - Message Format
 * - Bundle Structure
 * - Type Tags
 */
#ifndef ZOSC_HPP
#define ZOSC_HPP

#include "ZoscMessage.hpp"
#include "ZoscReceiver.hpp"
#include "ZoscBundle.hpp"
#include "ansi.h"
#include "debug.hpp"

/* ************************************************************************** */
/*                               Standard Libs                                */
/* ************************************************************************** */

#include <csignal>
#include <algorithm>
#include <arpa/inet.h>
#include <asio.hpp> // Socket mgmt
#include <chrono>
#include <cstdint>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>
#include <variant>
#include <vector>

#endif
