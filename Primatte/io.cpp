#include "io.h"
#include <iostream>

void Error(const std::string& msg)
{
    std::cerr << "ERROR: " << msg << "\n";
}

void Warning(const std::string& msg)
{
    std::cout << "Warning: " << msg << "\n";
}

void Inform(const std::string& msg)
{
    std::cout << ":- " << msg << "\n";
}
