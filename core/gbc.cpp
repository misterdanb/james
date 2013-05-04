#include "gbc.hpp"

void gbc::Log(std::string log)
{
	std::cout << log << std::endl;
}

void gbc::LogToFile(std::string file, std::string log)
{
	std::ofstream filestream(file, std::ios_base::app | std::ios_base::out);	
	
	filestream << log << std::endl;
	
	filestream.close();
}

std::string gbc::ToHex(int decimal)
{
	std::ostringstream oss;
	
	oss << std::hex << decimal;
	
	return ToUpper(oss.str());
}

std::string gbc::ToDec(int decimal)
{
	std::ostringstream oss;
	
	oss << std::dec << decimal;
	
	return oss.str();
}

std::string gbc::ToUpper(std::string string)
{
	std::string result = string;
	
	for (int i = 0; i < string.size(); i++)
	{
		result[i] = std::toupper(string[i]);
	}
	
	return result;
}
