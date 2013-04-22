#ifndef _LookUpTables_hpp_
#define _LookUpTables_hpp_

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
				static const int INSTRUCTION_CYCLES[AMOUNT_OF_INSTUCTION_TABLES][INSTRUCTION_TABLE_SIZE];
				static const int ADDITIONAL_INSTRUCTION_CYCLES[AMOUNT_OF_INSTUCTION_TABLES][INSTRUCTION_TABLE_SIZE];
				static const int INSTRUCTION_LENGTHS[AMOUNT_OF_INSTUCTION_TABLES][INSTRUCTION_TABLE_SIZE];
			};
		}
	}
}

#endif
