#ifndef _gbc_h_
#define _gbc_h_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdarg.h>

const int GBC_FALSE = 0;
const int GBC_TRUE = 1;

#define DEBUG

#define LOG_LEVEL_2

#ifdef DEBUG
	#define LOG(str) gbc::Log(std::string("L1> ") + str)
	#define ERROR(str) gbc::Log(std::string("E1> ") + str)
	#define CPU_LOG(str) gbc::LogToFile("cpu.log", str)
	
	#if defined(LOG_LEVEL_2) || defined(LOG_LEVEL_3)
		#define LOG_L2(str) gbc::Log(std::string("L2> ") + str)
		#define ERROR_L2(str) gbc::Log(std::string("E2> ") + str)
	#else
		#define LOG_L2(str)
		#define ERROR_L2(str)
	#endif
	
	#ifdef LOG_LEVEL_3
		#define LOG_L3(str) gbc::Log(std::string("L3> ") + str)
		#define ERROR_L3(str) gbc::Log(std::string("E3> ") + str)
	#else
		#define LOG_L3(str)
		#define ERROR_L3(str)
	#endif
#else
	#define LOG(str)
	#define ERROR(str)
	#define CPU_LOG(str)
	
	#define LOG_L2(str)
	#define ERROR_L2(str)
	
	#define LOG_L3(str)
	#define ERROR_L3(str)
#endif

namespace gbc
{
	void Log(std::string);
	void LogToFile(std::string, std::string);
	
	std::string ToHex(int);
	std::string ToDec(int);
	
	std::string ToUpper(std::string);
	
	std::string ToString(int[], int);
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
