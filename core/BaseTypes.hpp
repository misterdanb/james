#ifndef _BaseTypes_hpp_
#define _BaseTypes_hpp

#include <array>
#include <vector>
#include <queue>

namespace gbc
{
	namespace core
	{
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
		
		template<class T>
		using Queue = std::deque<T>;
		
		template<class T, class U>
		using Pair = std::pair<T, U>;
		
		typedef unsigned char byte;
	}
}

#endif
