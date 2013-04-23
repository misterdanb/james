#ifndef _GBC_hpp_
#define _GBC_hpp_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <type_traits>
#include <stdarg.h>

#define DEBUG

#define LOG_LEVEL_2

#ifdef DEBUG
	#define LOG(str) gbc::Log(std::string("L1> ") + str)
	#define ERROR(str) gbc::Log(std::string("E1> ") + str)
	
	#if defined(LOG_LEVEL_2) || defined(LOG_LEVEL_3)
		#define LOG_L2(str) gbc::Log(std::string("L2> ") + str)
		#define ERROR_L2(str) gbc::Log(std::string("E2> ") + str)
	#else
		#define LOG_L2(str)
		#define ERROR_L2(str)
	#endif
	
	#ifdef LOG_LEVEL_3
		#define CPU_LOG(str) gbc::LogToFile("cpu.log", str)
		#define LOG_L3(str) gbc::Log(std::string("L3> ") + str)
		#define ERROR_L3(str) gbc::Log(std::string("E3> ") + str)
	#else
		#define CPU_LOG(str)
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

#define GBC_NULL 0

namespace gbc
{
	// consts
	const int GBC_FALSE = 0;
	const int GBC_TRUE = 1;
	
	// const expressions
	constexpr int GetBit(int x, int n)
	{
		return (x >> n) & 0x01;
	}
	
	constexpr int SetBit(int x, int n, int value)
	{
		return (x & (~(0x01 << n))) | (value ? (0x01 << n) : 0x00);
	}
	
	constexpr int GetHigh(int x)
	{
		return (x >> 8) & 0xFF;
	}
	
	constexpr int GetLow(int x)
	{
		return x & 0xFF;
	}
	
	constexpr int JoinBytes(int high, int low)
	{
		return ((high & 0xFF) << 8) | (low & 0xFF);
	}
	
	constexpr int GetSignedValue(int x)
	{
		return (x & 0x7F) - (x & 0x80);
	}
	
	constexpr int GetAbsoluteValue(int x)
	{
		return (x < 0) ? (x * (-1)) : x;
	}
	
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
