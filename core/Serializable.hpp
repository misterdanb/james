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
			friend std::ostream &operator<<(std::ostream &os, Serializable &serializable)
			{
				serializable.Serialize(os);

				return os;
			}

			friend std::istream &operator>>(std::istream &is, Serializable &serializable)
			{
				serializable.Deserialize(is);

				return is;
			}
			
			virtual void Serialize(std::ostream &) = 0;
			virtual void Deserialize(std::istream &) = 0;
		};
	}
}

#endif
