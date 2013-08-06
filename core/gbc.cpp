#include "gbc.hpp"

namespace gbc
{
	void Log(std::string log)
	{
		std::cout << log << std::endl;
	}

	void LogToFile(std::string file, std::string log)
	{
		std::ofstream filestream(file, std::ios_base::app | std::ios_base::out);	
		
		filestream << log << std::endl;
		
		filestream.close();
	}

	std::string ToHex(int decimal)
	{
		std::ostringstream oss;
		
		oss << std::hex << decimal;
		
		return ToUpper(oss.str());
	}
	
	std::string ToFixedHex(int decimal, int length)
	{
		std::ostringstream oss;
		
		oss.width(length);
		oss.fill('0');
		
		oss << std::fixed << std::hex << decimal;
		
		return ToUpper(oss.str());
	}

	std::string ToDec(int decimal)
	{
		std::ostringstream oss;
		
		oss << std::dec << decimal;
		
		return oss.str();
	}
	
	std::string ToFixedDec(int decimal, int length)
	{
		std::ostringstream oss;
		
		oss.width(length);
		oss.fill('0');
		
		oss << std::fixed << std::dec << decimal;
		
		return ToUpper(oss.str());
	}

	std::string ToUpper(std::string string)
	{
		std::string result = string;
		
		for (unsigned int i = 0; i < string.size(); i++)
		{
			result[i] = (char) std::toupper(string[i]);
//			int temp = std::toupper(string[i]);
//			std::string tmp = std::to_string(temp);
//			result[i] = *(tmp.c_str());
		}
		
		return result;
	}
}
