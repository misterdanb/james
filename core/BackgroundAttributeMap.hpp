#ifndef _BackgroundAttributeMap_hpp_
#define _BackgroundAttributeMap_hpp_

#include "James.hpp"
#include "BackgroundAttribute.hpp"

namespace james
{
	namespace core
	{
		class BackgroundAttributeMap : public ByteMap<32, 32>
		{
		public:
			static const int WIDTH = 32;
			static const int HEIGHT = 32;
			
			static const int DATA_SIZE = WIDTH * HEIGHT;
			
		public:
			BackgroundAttributeMap();
			BackgroundAttributeMap(Array<int, DATA_SIZE>);
			~BackgroundAttributeMap();
			
			void SetData(Array<int, DATA_SIZE>);
			void SetBackgroundAttribute(int, int, BackgroundAttribute);
			
			Array<int, DATA_SIZE> GetData();
			BackgroundAttribute GetBackgroundAttribute(int, int);
			
		public:
			using ByteMap<WIDTH, HEIGHT>::_matrixElements;
		};
	}
}

#endif
