#ifndef _MemoryBlock_hpp
#define _MemoryBlock_hpp

#include "James.hpp"
#include "Serializable.hpp"

namespace james
{
    namespace core
    {
        /**
         * @class MemoryBlock
         * 
         * MemoryBlock is an implementation for all classes that use
         * banked or non-banked memory.
         */
        class MemoryBlock : public Serializable
        {
        public:
            /**
             * Constructor of Memory.
             * 
             * Initializes all values in all banks with zero.
             * 
             * @param banks The amount banks.
             * @param size The size of each bank.
             * 
             * @see Reset()
             */
            MemoryBlock(int banks, int size);
            
            /**
             * Destructor of MemoryBlock.
             */
            ~MemoryBlock() = default;
            
            /**
             * Reads a byte.
             * 
             * @param bank The bank from which the byte is read.
             * @param address The address from which the byte is read.
             * @return The byte which has been read.
             */
            int ReadByteFromBank(int bank, int address);
            
            /**
             * Writes a byte.
             * 
             * @param bank The bank to which the byte is written.
             * @param address The address to which the byte is written.
             * @param value The byte which is written
             */
            void WriteByteToBank(int bank, int address, int value);
            
            virtual void Serialize(std::ostream &);
            virtual void Deserialize(std::istream &);
            
            /**
             * Resets all values in all banks to zero. This method may
             * be implemented by child classes to execute further
             * code that is necessary to actually reset the memory
             * module.
             */
            virtual void Reset();
            
        private:
            /** 
             * The amount of banks.
             */
            int _banks;
            
            /**
             * The size of each bank.
             */
            int _size;
            
            /**
             * The two dimensional array in which all bytes of all banks
             * are stored.
             */
            DynamicArray2<int> _data;
        };
    }
}

#endif
