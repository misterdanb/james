#ifndef _gbc_h_
#define _gbc_h_

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

const int GBC_FALSE = 0;
const int GBC_TRUE = 1;

//#define DEBUG

namespace gbc
{
	void Log(std::string);
}

/* constexpr const char *NotImplemented()
{
	return "Not implmenented.";
}

constexpr char *AddressOutOfRange(int address)
{
	std::stringstream ss;
	
	ss << "Address out of range (0x";
	ss << std::hex << address;
	ss << std::string(")");
	
	return std::const_cast<char *>(ss.str().c_str());
}*/

#endif
