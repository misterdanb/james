#ifndef _LookUpTables_hpp_
#define _LookUpTables_hpp_

#include "../gbc.hpp"

namespace gbc
{
	namespace core
	{
		namespace cpu
		{
			class LookUpTables
			{
			public:
				static const int AMOUNT_OF_INSTUCTION_TABLES = 2;
				static const int INSTRUCTION_TABLE_SIZE = 0x0100;
			
			public:
				static const Array2<int, AMOUNT_OF_INSTUCTION_TABLES, INSTRUCTION_TABLE_SIZE> INSTRUCTION_CYCLES;
				static const Array2<int, AMOUNT_OF_INSTUCTION_TABLES, INSTRUCTION_TABLE_SIZE> ADDITIONAL_INSTRUCTION_CYCLES;
				static const Array2<int, AMOUNT_OF_INSTUCTION_TABLES, INSTRUCTION_TABLE_SIZE> INSTRUCTION_LENGTHS;
			};
		}
	}
}

#endif
