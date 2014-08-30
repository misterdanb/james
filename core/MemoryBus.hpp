#ifndef _MemoryBus_hpp_
#define _MemoryBus_hpp_

#include "James.hpp"

using namespace james;
using namespace james::core;

namespace james
{
	namespace core
	{
		/**
		 * @class MemoryBus
		 *
		 * MemoryBus is an interface to all classes that represent an
		 * addressable part of the memory.
		 */
		class MemoryBus
		{
		  public:
			/**
			 * Contructor of MemoryBus.
			 */
			MemoryBus() = default;

			/**
			 * Destructor of MemoryBus.
			 */
			~MemoryBus() = default;

			/**
			 * Reads a byte from this part of the memory bus.
			 *
			 * @param address The address from which the byte is read.
			 * @return The byte which has been read.
			 *
			 * @see WriteByte
			 */
			virtual j8 ReadByte (j16 address) = 0;

			/**
			 * Writes a byte to this part of the memory bus.
			 *
			 * @param address The address to which the byte is written.
			 * @param value The byte which is written.
			 *
			 * @see ReadByte
			 */
			virtual void WriteByte (j16 address, j8 value) = 0;

			/**
			 * Reads a word from this part of the memory bus.
			 *
			 * @param address The address from which the word is read.
			 * @return The word which has been read.
			 *
			 * @see WriteByte
			 */
			j16 ReadWord (j16 address);

			/**
			 * Writes a word to this part of the memory bus.
			 *
			 * @param address The address to which the word is written.
			 * @param value The word which is written.
			 *
			 * @see ReadByte
			 */
			void WriteWord (j16 address, j16 value);
		};
	}
}

#endif
