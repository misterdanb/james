#ifndef _Serializable_hpp_
#define _Serializable_hpp_

#include "James.hpp"

namespace james
{
	namespace core
	{
		class Serializable
		{
		public:
			friend std::ostream &operator<<(std::ostream &, Serializable &);
			friend std::istream &operator>>(std::istream &, Serializable &);
			
		public:
			virtual void Serialize(std::ostream &) = GBC_NULL;
			virtual void Deserialize(std::istream &) = GBC_NULL;
		};
	}
}

#endif
