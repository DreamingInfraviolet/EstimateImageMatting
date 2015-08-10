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

/** Converts the input to a hexadecimal string. */
template <class T>
std::string ToHexString(const T t)
{
    std::ostringstream ss;
    ss << std::hex << t;
    return ss.str();
}
