#ifndef _MemoryBlock_hpp
#define _MemoryBlock_hpp

#include "James.hpp"
#include "MemoryBus.hpp"
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
        class MemoryBlock : public MemoryBus, Serializable
        {
        public:
            /**
             * Constructor of Memory.
             * 
             * Initializes all values in all banks with zero.
             * 
             * @param amountOfBanks The amount banks.
             * @param size The size of each bank.
             * 
             * @see Reset()
             */
            MemoryBlock(int amountOfBanks, int bankSize);
            
            /**
             * Destructor of MemoryBlock.
             */
            ~MemoryBlock() = default;
            
            /**
             * Reads a byte.
             * 
             * @param bankNumber The bank from which the byte is read.
             * @param address The address from which the byte is read.
             * @return The byte which has been read.
             */
            int ReadByteFromBank(int bankNumber, int address) const;
            
            /**
             * Writes a byte.
             * 
             * @param bankNumber The bank to which the byte is written.
             * @param address The address to which the byte is written.
             * @param value The byte which is written
             */
            void WriteByteToBank(int bankNumber, int address, int value);
            
						virtual int ReadByte(int address);
						virtual void WriteByte(int address, int value);

            virtual void Serialize(std::ostream &);
            virtual void Deserialize(std::istream &);
            
            /**
             * Resets all values in all banks to zero. This method may
             * be implemented by child classes to execute further
             * code that is necessary to actually reset the memory
             * module.
             */
            virtual void Reset();

						/**
						 * Modifies the active bank selection.
						 *
						 * @param bankNumber New active bank number.
						 */
						void SetActiveBank(int bankNumber);

						/**
						 * Returns the active bank selection.
						 *
						 * @return The active bank number.
						 */
						int GetActiveBank() const;
            
        private:
            /** 
             * The amount of banks.
             */
            int _amountOfBanks;
            
            /**
             * The size of each bank.
             */
            int _bankSize;

						/**
						 * The currently active bank to read from.
						 */
						int _activeBankNumber;
            
            /**
             * The two dimensional array in which all bytes of all banks
             * are stored.
             */
            DynamicArray2<int> _banks;
        };
    }
}

#endif
