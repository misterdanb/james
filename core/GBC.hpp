#ifndef _gbc_h_
#define _gbc_h_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <type_traits>
#include <stdarg.h>

//#define DEBUG

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
	// consts
	const int GBC_FALSE = 0;
	const int GBC_TRUE = 1;
	
	// const expressions
	template<typename T>
	constexpr typename std::underlying_type<T>::type GetEnumValue(T value)
	{
		return static_cast<typename std::underlying_type<T>::type>(value);
	}
	
	// helper methods
	void Log(std::string);
	void LogToFile(std::string, std::string);
	
	std::string ToHex(int);
	std::string ToDec(int);
	
	std::string ToUpper(std::string);
	
	std::string ToString(int[], int);
}

#endif
