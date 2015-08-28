#pragma once
#include <string>
#include <sstream>

/**
* A series of function to help with basic input and output.
* */

/** Prints the given error message */
void Error(const std::string& msg);

/** Prints the given warning message. */
void Warning(const std::string& msg);

/** Prints the given information message. */
void Inform(const std::string& msg);

/** Converts the input to a string. */
template <class T>
std::string ToString(const T t)
{
    std::ostringstream ss;
    ss << t;
    return ss.str();
}

/* Timer macros. Usage:
 * '''
 * START_TIMER(myFunkyTimer);
 * doHeavyCode();
 * END_TIMER(myFunkyTimer);
 * '''
 */

#include <chrono>

#define START_TIMER(t) auto t = std::chrono::system_clock::now();

#define END_TIMER(t) {Inform("Stopping timer: "#t + std::string(" : ") + \
    ToString(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - t).count()));}
