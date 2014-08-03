#include "LookUpTables.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cpu;

const Array2<int, LookUpTables::AMOUNT_OF_INSTUCTION_TABLES, LookUpTables::INSTRUCTION_TABLE_SIZE> LookUpTables::INSTRUCTION_CYCLES =
{
	{
		{
			// DEFAULT
			/*  X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF        XX */
			4,	12,	8,	8,	4,	4,	8,	4,	20,	8,	8,	8,	4,	4,	8,	4,
			0,	12,	8,	8,	4,	4,	8,	4,	12,	8,	8,	8,	4,	4,	8,	4,
			8,	12,	8,	8,	4,	4,	8,	4,	8,	8,	8,	8,	4,	4,	8,	4,
			8,	12,	8,	8,	12,	12,	12,	4,	8,	8,	8,	8,	4,	4,	8,	4,
			4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
			4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
			4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
			8,	8,	8,	8,	8,	8,	0,	8,	4,	4,	4,	4,	4,	4,	8,	4,
			4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
			4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
			4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
			4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
			8,	12,	12,	16,	12,	16,	8,	16,	8,	16,	12,	0,	12,	24,	8,	16,
			8,	12,	12,	0,	12,	16,	8,	16,	8,	16,	12,	0,	12,	0,	8,	16,
			12,	12,	8,	0,	0,	16,	8,	16,	16,	4,	16,	0,	0,	0,	8,	16,
			12,	12,	8,	4,	0,	16,	8,	16,	12,	8,	16,	4,	0,	0,	8,	16
		},
		{
			// CB
			/*  X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF        XX */
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	12,	8,	8,	8,	8,	8,	8,	8,	12,	8,
			8,	8,	8,	8,	8,	8,	12,	8,	8,	8,	8,	8,	8,	8,	12,	8,
			8,	8,	8,	8,	8,	8,	12,	8,	8,	8,	8,	8,	8,	8,	12,	8,
			8,	8,	8,	8,	8,	8,	12,	8,	8,	8,	8,	8,	8,	8,	12,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
			8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8
		}
	}
};

const Array2<int, LookUpTables::AMOUNT_OF_INSTUCTION_TABLES, LookUpTables::INSTRUCTION_TABLE_SIZE> LookUpTables::ADDITIONAL_INSTRUCTION_CYCLES =
{
	{
		{
			// DEFAULT
			/*  X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF        XX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 0X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 1X */
			4,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,    /* 2X */
			4,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,    /* 3X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 4X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 5X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 6X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 7X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 8X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 9X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* AX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* BX */
			12,  0,  4,  0, 12,  0,  0,  0,  12,  0,  4,  0, 12,  0,  0,  0,    /* CX */
			12,  0,  4,  0, 12,  0,  0,  0,  12,  0,  4,  0, 12,  0,  0,  0,    /* DX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* EX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0     /* FX */
		},
		{
			// CB
			/*  X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF        XX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 0X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 1X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 2X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 3X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 4X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 5X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 6X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 7X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 8X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 9X */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* AX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* BX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* CX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* DX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* EX */
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0     /* FX */
		}
	}
};

const Array2<int, LookUpTables::AMOUNT_OF_INSTUCTION_TABLES, LookUpTables::INSTRUCTION_TABLE_SIZE> LookUpTables::INSTRUCTION_LENGTHS =
{
	{
		{
			// DEFAULT
			/*  X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF        XX */
			1,  3,  1,  1,  1,  1,  2,  1,  3,  1,  1,  1,  1,  1,  2,  1,    /* 0X */
			2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,    /* 1X */
			2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,    /* 2X */
			2,  3,  1,  1,  1,  1,  2,  1,  2,  1,  1,  1,  1,  1,  2,  1,    /* 3X */
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,    /* 4X */
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,    /* 5X */
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,    /* 6X */
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,    /* 7X */
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,    /* 8X */
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,    /* 9X */
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,    /* AX */
			1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,    /* BX */
			1,  1,  3,  3,  3,  1,  2,  1,  1,  1,  3,  0,  3,  3,  2,  1,    /* CX */
			1,  1,  3,  0,  3,  1,  2,  1,  1,  1,  3,  0,  3,  0,  2,  1,    /* DX */
			2,  1,  1,  0,  0,  1,  2,  1,  2,  1,  3,  0,  0,  0,  2,  1,    /* EX */
			2,  1,  1,  1,  0,  1,  2,  1,  2,  1,  3,  1,  0,  0,  2,  1     /* FX */
		},
		{
			// CB
			/*  X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF        XX */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 0X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 1X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 2X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 3X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 4X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 5X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 6X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 7X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 8X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* 9X */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* AX */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* BX */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* CX */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* DX */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,    /* EX */
			2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2     /* FX */
		}
	}
};
