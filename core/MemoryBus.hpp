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
             * Reads a byte from this part of the memory bus.
             * 
             * @param address The address from which the byte is read.
             * @return The byte which has been read.
             */
            virtual int ReadByte(int address) = 0;
            
            /**
             * Writes a byte to this part of the memory bus.
             * 
             * @param address The address to which the byte is written.
             * @param value The byte which is written.
             */
            virtual void WriteByte(int address, int value) = 0;
        };
    }
}

#endif
