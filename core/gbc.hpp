#ifndef _GBC_hpp_
#define _GBC_hpp_

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <type_traits>

//#define DEBUG

#define LOG_LEVEL_2

#ifdef DEBUG
	#define LOG(str) Log(std::string("L1> ") + str)
	#define ERROR(str) Log(std::string("E1> ") + str)
	
	#if defined(LOG_LEVEL_2) || defined(LOG_LEVEL_3)
		#define LOG_L2(str) Log(std::string("L2> ") + str)
		#define ERROR_L2(str) Log(std::string("E2> ") + str)
	#else
		#define LOG_L2(str)
		#define ERROR_L2(str)
	#endif
	
	#ifdef LOG_LEVEL_3
		#define CPU_LOG(str) LogToFile("cpu.log", str)
		#define LOG_L3(str) Log(std::string("L3> ") + str)
		#define ERROR_L3(str) Log(std::string("E3> ") + str)
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
	// templates
	template<class T, int size>
	using Array = std::array<T, size>;
	
	template<class T, int width, int height>
	using Array2 = std::array<std::array<T, height>, width>;
	
	template<class T, int width, int height, int depth>
	using Array3 = std::array<std::array<std::array<T, depth>, height>, width>;
	
	template<class T>
	using DynamicArray = std::vector<T>;
	
	template<class T>
	using DynamicArray2 = std::vector<std::vector<T>>;
	
	template<class T>
	using DynamicArray3 = std::vector<std::vector<std::vector<T>>>;
	
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
	
	template<unsigned long T>
	std::string ToString(Array<int, T> array)
	{
		std::string string;
		
		for (int &i : array)
		{
			string += (char) i;
		}
		
		return string;
	}
	
	std::string ToHex(int);
	std::string ToDec(int);
	
	std::string ToUpper(std::string);
}

#endif
