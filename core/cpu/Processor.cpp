#include "Processor.hpp"

gbc::core::cpu::Processor::Processor()
	: _counter(0), _state(), _bus(NULL)
{
}

gbc::core::cpu::Processor::~Processor()
{
}

void gbc::core::cpu::Processor::SetMemoryBus(IMemoryBus *bus)
{
	_bus = bus;
}

void gbc::core::cpu::Processor::Step()
{
	if (S_TICKS > 0)
	{
		S_TICKS--;
	}
	else
	{
		ExecuteInterrupt();
		ExecuteInstruction();
	}
}

void gbc::core::cpu::Processor::ExecuteInstruction()
{
	if (!S_HALTED)
	{
		FETCH_INSTRUCTION();
		CB_FETCH_INSTRUCTION();
		
		/*std::cout << ToHex(GET_OP_CODE());
		std::cout << ToHex(GET_OP_LOW());
		std::cout << ToHex(GET_OP_HIGH());
		std::cin.get();*/
		
		std::ostringstream oss;
		
		oss << "COUNTER=" << ToDec(_counter) << ", "
		    << "OP_CODE=" << ToHex(GET_OP_CODE()) << ", "
		    << "OP_LOW=" << ToHex(GET_OP_LOW()) << ", "
		    << "OP_HIGH=" << ToHex(GET_OP_HIGH()) << ", "
		    << "A=" << ToHex(S_A) << ", "
		    << "F=" << ToHex(S_F) << ", "
		    << "B=" << ToHex(S_B) << ", "
		    << "C=" << ToHex(S_C) << ", "
		    << "D=" << ToHex(S_D) << ", "
		    << "E=" << ToHex(S_E) << ", "
		    << "H=" << ToHex(S_H) << ", "
		    << "L=" << ToHex(S_L) << ", "
		    << "(HL)=" << ToHex(READ(JOIN_BYTES(S_H, S_L))) << ", "
		    << "PC=" << ToHex(S_PC) << ", "
		    << "SP=" << ToHex(S_SP) << ", "
		    << "POP_LOW=" << ToHex(READ(S_SP)) << ", "
		    << "POP_HIGH=" << ToHex(READ(S_SP + 1));
		
		CPU_LOG(oss.str());
		
		_counter++;
		
		switch (GET_OP_CODE())
		{
			case 0x00: NOP(); break;
			case 0x01: LD_RR_NN(&S_B, &S_C); break;
			case 0x02: LD_AA_R(&S_B, &S_C, &S_A); break;
			case 0x03: INC_RR(&S_B, &S_C); break;
			case 0x04: INC_R(&S_B); break;
			case 0x05: DEC_R(&S_B); break;
			case 0x06: LD_R_N(&S_B); break;
			case 0x07: RLCA(); break;
			case 0x08: LD_AA_SP(&GET_OP_HIGH(), &GET_OP_LOW()); break;
			case 0x09: ADD_RR_RR(&S_H, &S_L, &S_B, &S_C); break;
			case 0x0A: LD_R_AA(&S_A, &S_B, &S_C); break;
			case 0x0B: DEC_RR(&S_B, &S_C); break;
			case 0x0C: INC_R(&S_C); break;
			case 0x0D: DEC_R(&S_C); break;
			case 0x0E: LD_R_N(&S_C); break;
			case 0x0F: RRCA(); break;
			case 0x10: STOP(); break;
			case 0x11: LD_RR_NN(&S_D, &S_E); break;
			case 0x12: LD_AA_R(&S_D, &S_E, &S_A); break;
			case 0x13: INC_RR(&S_D, &S_E); break;
			case 0x14: INC_R(&S_D); break;
			case 0x15: DEC_R(&S_D); break;
			case 0x16: LD_R_N(&S_D); break;
			case 0x17: RLA(); break;
			case 0x18: JR_N(); break;
			case 0x19: ADD_RR_RR(&S_H, &S_L, &S_D, &S_E); break;
			case 0x1A: LD_R_AA(&S_A, &S_D, &S_E); break;
			case 0x1B: DEC_RR(&S_D, &S_E); break;
			case 0x1C: INC_R(&S_E); break;
			case 0x1D: DEC_R(&S_E); break;
			case 0x1E: LD_R_N(&S_E); break;
			case 0x1F: RRA(); break;
			case 0x20: JR_N_IF(!GET_ZFLAG()); break;
			case 0x21: LD_RR_NN(&S_H, &S_L); break;
			case 0x22: LD_AA_R_INC(&S_H, &S_L, &S_A); break;
			case 0x23: INC_RR(&S_H, &S_L); break;
			case 0x24: INC_R(&S_H); break;
			case 0x25: DEC_R(&S_H); break;
			case 0x26: LD_R_N(&S_H); break;
			case 0x27: DAA(); break;
			case 0x28: JR_N_IF(GET_ZFLAG()); break;
			case 0x29: ADD_RR_RR(&S_H, &S_L, &S_H, &S_L); break;
			case 0x2A: LD_R_AA_INC(&S_A, &S_H, &S_L); break;
			case 0x2B: DEC_RR(&S_H, &S_L); break;
			case 0x2C: INC_R(&S_L); break;
			case 0x2D: DEC_R(&S_L); break;
			case 0x2E: LD_R_N(&S_L); break;
			case 0x2F: CPL(); break;
			case 0x30: JR_N_IF(!GET_CFLAG()); break;
			case 0x31: LD_SP_NN(); break;
			case 0x32: LD_AA_R_DEC(&S_H, &S_L, &S_A); break;
			case 0x33: INC_SP(); break;
			case 0x34: INC_AA(&S_H, &S_L); break;
			case 0x35: DEC_AA(&S_H, &S_L); break;
			case 0x36: LD_AA_N(&S_H, &S_L); break;
			case 0x37: SCF(); break;
			case 0x38: JR_N_IF(GET_CFLAG()); break;
			case 0x39: ADD_RR_SP(&S_H, &S_L); break;
			case 0x3A: LD_R_AA_DEC(&S_A, &S_H, &S_L); break;
			case 0x3B: DEC_SP(); break;
			case 0x3C: INC_R(&S_A); break;
			case 0x3D: DEC_R(&S_A); break;
			case 0x3E: LD_R_N(&S_A); break;
			case 0x3F: CCF(); break;
			case 0x40: LD_R_R(&S_B, &S_B); break;
			case 0x41: LD_R_R(&S_B, &S_C); break;
			case 0x42: LD_R_R(&S_B, &S_D); break;
			case 0x43: LD_R_R(&S_B, &S_E); break;
			case 0x44: LD_R_R(&S_B, &S_H); break;
			case 0x45: LD_R_R(&S_B, &S_L); break;
			case 0x46: LD_R_AA(&S_B, &S_H, &S_L); break;
			case 0x47: LD_R_R(&S_B, &S_A); break;
			case 0x48: LD_R_R(&S_C, &S_B); break;
			case 0x49: LD_R_R(&S_C, &S_C); break;
			case 0x4A: LD_R_R(&S_C, &S_D); break;
			case 0x4B: LD_R_R(&S_C, &S_E); break;
			case 0x4C: LD_R_R(&S_C, &S_H); break;
			case 0x4D: LD_R_R(&S_C, &S_L); break;
			case 0x4E: LD_R_AA(&S_C, &S_H, &S_L); break;
			case 0x4F: LD_R_R(&S_C, &S_A); break;
			case 0x50: LD_R_R(&S_D, &S_B); break;
			case 0x51: LD_R_R(&S_D, &S_C); break;
			case 0x52: LD_R_R(&S_D, &S_D); break;
			case 0x53: LD_R_R(&S_D, &S_E); break;
			case 0x54: LD_R_R(&S_D, &S_H); break;
			case 0x55: LD_R_R(&S_D, &S_L); break;
			case 0x56: LD_R_AA(&S_D, &S_H, &S_L); break;
			case 0x57: LD_R_R(&S_D, &S_A); break;
			case 0x58: LD_R_R(&S_E, &S_B); break;
			case 0x59: LD_R_R(&S_E, &S_C); break;
			case 0x5A: LD_R_R(&S_E, &S_D); break;
			case 0x5B: LD_R_R(&S_E, &S_E); break;
			case 0x5C: LD_R_R(&S_E, &S_H); break;
			case 0x5D: LD_R_R(&S_E, &S_L); break;
			case 0x5E: LD_R_AA(&S_E, &S_H, &S_L); break;
			case 0x5F: LD_R_R(&S_E, &S_A); break;
			case 0x60: LD_R_R(&S_H, &S_B); break;
			case 0x61: LD_R_R(&S_H, &S_C); break;
			case 0x62: LD_R_R(&S_H, &S_D); break;
			case 0x63: LD_R_R(&S_H, &S_E); break;
			case 0x64: LD_R_R(&S_H, &S_H); break;
			case 0x65: LD_R_R(&S_H, &S_L); break;
			case 0x66: LD_R_AA(&S_H, &S_H, &S_L); break;
			case 0x67: LD_R_R(&S_H, &S_A); break;
			case 0x68: LD_R_R(&S_L, &S_B); break;
			case 0x69: LD_R_R(&S_L, &S_C); break;
			case 0x6A: LD_R_R(&S_L, &S_D); break;
			case 0x6B: LD_R_R(&S_L, &S_E); break;
			case 0x6C: LD_R_R(&S_L, &S_H); break;
			case 0x6D: LD_R_R(&S_L, &S_L); break;
			case 0x6E: LD_R_AA(&S_L, &S_H, &S_L); break;
			case 0x6F: LD_R_R(&S_L, &S_A); break;
			case 0x70: LD_AA_R(&S_H, &S_L, &S_B); break;
			case 0x71: LD_AA_R(&S_H, &S_L, &S_C); break;
			case 0x72: LD_AA_R(&S_H, &S_L, &S_D); break;
			case 0x73: LD_AA_R(&S_H, &S_L, &S_E); break;
			case 0x74: LD_AA_R(&S_H, &S_L, &S_H); break;
			case 0x75: LD_AA_R(&S_H, &S_L, &S_L); break;
			case 0x76: HALT(); break;
			case 0x77: LD_AA_R(&S_H, &S_L, &S_A); break;
			case 0x78: LD_R_R(&S_A, &S_B); break;
			case 0x79: LD_R_R(&S_A, &S_C); break;
			case 0x7A: LD_R_R(&S_A, &S_D); break;
			case 0x7B: LD_R_R(&S_A, &S_E); break;
			case 0x7C: LD_R_R(&S_A, &S_H); break;
			case 0x7D: LD_R_R(&S_A, &S_L); break;
			case 0x7E: LD_R_AA(&S_A, &S_H, &S_L); break;
			case 0x7F: LD_R_R(&S_A, &S_A); break;
			case 0x80: ADD_R_R(&S_A, &S_B); break;
			case 0x81: ADD_R_R(&S_A, &S_C); break;
			case 0x82: ADD_R_R(&S_A, &S_D); break;
			case 0x83: ADD_R_R(&S_A, &S_E); break;
			case 0x84: ADD_R_R(&S_A, &S_H); break;
			case 0x85: ADD_R_R(&S_A, &S_L); break;
			case 0x86: ADD_R_AA(&S_A, &S_H, &S_L); break;
			case 0x87: ADD_R_R(&S_A, &S_A); break;
			case 0x88: ADC_R_R(&S_A, &S_B); break;
			case 0x89: ADC_R_R(&S_A, &S_C); break;
			case 0x8A: ADC_R_R(&S_A, &S_D); break;
			case 0x8B: ADC_R_R(&S_A, &S_E); break;
			case 0x8C: ADC_R_R(&S_A, &S_H); break;
			case 0x8D: ADC_R_R(&S_A, &S_L); break;
			case 0x8E: ADC_R_AA(&S_A, &S_H, &S_L); break;
			case 0x8F: ADC_R_R(&S_A, &S_A); break;
			case 0x90: SUB_R_R(&S_A, &S_B); break;
			case 0x91: SUB_R_R(&S_A, &S_C); break;
			case 0x92: SUB_R_R(&S_A, &S_D); break;
			case 0x93: SUB_R_R(&S_A, &S_E); break;
			case 0x94: SUB_R_R(&S_A, &S_H); break;
			case 0x95: SUB_R_R(&S_A, &S_L); break;
			case 0x96: SUB_R_AA(&S_A, &S_H, &S_L); break;
			case 0x97: SUB_R_R(&S_A, &S_A); break;
			case 0x98: SBC_R_R(&S_A, &S_B); break;
			case 0x99: SBC_R_R(&S_A, &S_C); break;
			case 0x9A: SBC_R_R(&S_A, &S_D); break;
			case 0x9B: SBC_R_R(&S_A, &S_E); break;
			case 0x9C: SBC_R_R(&S_A, &S_H); break;
			case 0x9D: SBC_R_R(&S_A, &S_L); break;
			case 0x9E: SBC_R_AA(&S_A, &S_H, &S_L); break;
			case 0x9F: SBC_R_R(&S_A, &S_A); break;
			case 0xA0: AND_R_R(&S_A, &S_B); break;
			case 0xA1: AND_R_R(&S_A, &S_C); break;
			case 0xA2: AND_R_R(&S_A, &S_D); break;
			case 0xA3: AND_R_R(&S_A, &S_E); break;
			case 0xA4: AND_R_R(&S_A, &S_H); break;
			case 0xA5: AND_R_R(&S_A, &S_L); break;
			case 0xA6: AND_R_AA(&S_A, &S_H, &S_L); break;
			case 0xA7: AND_R_R(&S_A, &S_A); break;
			case 0xA8: XOR_R_R(&S_A, &S_B); break;
			case 0xA9: XOR_R_R(&S_A, &S_C); break;
			case 0xAA: XOR_R_R(&S_A, &S_D); break;
			case 0xAB: XOR_R_R(&S_A, &S_E); break;
			case 0xAC: XOR_R_R(&S_A, &S_H); break;
			case 0xAD: XOR_R_R(&S_A, &S_L); break;
			case 0xAE: XOR_R_AA(&S_A, &S_H, &S_L); break;
			case 0xAF: XOR_R_R(&S_A, &S_A); break;
			case 0xB0: OR_R_R(&S_A, &S_B); break;
			case 0xB1: OR_R_R(&S_A, &S_C); break;
			case 0xB2: OR_R_R(&S_A, &S_D); break;
			case 0xB3: OR_R_R(&S_A, &S_E); break;
			case 0xB4: OR_R_R(&S_A, &S_H); break;
			case 0xB5: OR_R_R(&S_A, &S_L); break;
			case 0xB6: OR_R_AA(&S_A, &S_H, &S_L); break;
			case 0xB7: OR_R_R(&S_A, &S_A); break;
			case 0xB8: CP_R_R(&S_A, &S_B); break;
			case 0xB9: CP_R_R(&S_A, &S_C); break;
			case 0xBA: CP_R_R(&S_A, &S_D); break;
			case 0xBB: CP_R_R(&S_A, &S_E); break;
			case 0xBC: CP_R_R(&S_A, &S_H); break;
			case 0xBD: CP_R_R(&S_A, &S_L); break;
			case 0xBE: CP_R_AA(&S_A, &S_H, &S_L); break;
			case 0xBF: CP_R_R(&S_A, &S_A); break;
			case 0xC0: RET_IF(!GET_ZFLAG()); break;
			case 0xC1: POP_RR(&S_B, &S_C); break;
			case 0xC2: JP_NN_IF(!GET_ZFLAG()); break;
			case 0xC3: JP_NN(); break;
			case 0xC4: CALL_NN_IF(!GET_ZFLAG()); break;
			case 0xC5: PUSH_RR(&S_B, &S_C); break;
			case 0xC6: ADD_R_N(&S_A); break;
			case 0xC7: RST(0x00); break;
			case 0xC8: RET_IF(GET_ZFLAG()); break;
			case 0xC9: RET(); break;
			case 0xCA: JP_NN_IF(GET_ZFLAG()); break;
			case 0xCB:
				switch (CB_GET_OP_CODE())
				{
					case 0x00: RLC_R(&S_B); break;
					case 0x01: RLC_R(&S_C); break;
					case 0x02: RLC_R(&S_D); break;
					case 0x03: RLC_R(&S_E); break;
					case 0x04: RLC_R(&S_H); break;
					case 0x05: RLC_R(&S_L); break;
					case 0x06: RLC_AA(&S_H, &S_L); break;
					case 0x07: RLC_R(&S_A); break;
					case 0x08: RRC_R(&S_B); break;
					case 0x09: RRC_R(&S_C); break;
					case 0x0A: RRC_R(&S_D); break;
					case 0x0B: RRC_R(&S_E); break;
					case 0x0C: RRC_R(&S_H); break;
					case 0x0D: RRC_R(&S_L); break;
					case 0x0E: RRC_AA(&S_H, &S_L); break;
					case 0x0F: RRC_R(&S_A); break;
					case 0x10: RL_R(&S_B); break;
					case 0x11: RL_R(&S_C); break;
					case 0x12: RL_R(&S_D); break;
					case 0x13: RL_R(&S_E); break;
					case 0x14: RL_R(&S_H); break;
					case 0x15: RL_R(&S_L); break;
					case 0x16: RL_AA(&S_H, &S_L); break;
					case 0x17: RL_R(&S_A); break;
					case 0x18: RR_R(&S_B); break;
					case 0x19: RR_R(&S_C); break;
					case 0x1A: RR_R(&S_D); break;
					case 0x1B: RR_R(&S_E); break;
					case 0x1C: RR_R(&S_H); break;
					case 0x1D: RR_R(&S_L); break;
					case 0x1E: RR_AA(&S_H, &S_L); break;
					case 0x1F: RR_R(&S_A); break;
					case 0x20: SLA_R(&S_B); break;
					case 0x21: SLA_R(&S_C); break;
					case 0x22: SLA_R(&S_D); break;
					case 0x23: SLA_R(&S_E); break;
					case 0x24: SLA_R(&S_H); break;
					case 0x25: SLA_R(&S_L); break;
					case 0x26: SLA_AA(&S_H, &S_L); break;
					case 0x27: SLA_R(&S_A); break;
					case 0x28: SRA_R(&S_B); break;
					case 0x29: SRA_R(&S_C); break;
					case 0x2A: SRA_R(&S_D); break;
					case 0x2B: SRA_R(&S_E); break;
					case 0x2C: SRA_R(&S_H); break;
					case 0x2D: SRA_R(&S_L); break;
					case 0x2E: SRA_AA(&S_H, &S_L); break;
					case 0x2F: SRA_R(&S_A); break;
					case 0x30: SWAP_R(&S_B); break;
					case 0x31: SWAP_R(&S_C); break;
					case 0x32: SWAP_R(&S_D); break;
					case 0x33: SWAP_R(&S_E); break;
					case 0x34: SWAP_R(&S_H); break;
					case 0x35: SWAP_R(&S_L); break;
					case 0x36: SWAP_AA(&S_H, &S_L); break;
					case 0x37: SWAP_R(&S_A); break;
					case 0x38: SRL_R(&S_B); break;
					case 0x39: SRL_R(&S_C); break;
					case 0x3A: SRL_R(&S_D); break;
					case 0x3B: SRL_R(&S_E); break;
					case 0x3C: SRL_R(&S_H); break;
					case 0x3D: SRL_R(&S_L); break;
					case 0x3E: SRL_AA(&S_H, &S_L); break;
					case 0x3F: SRL_R(&S_A); break;
					case 0x40: BIT_X_R(0, &S_B); break;
					case 0x41: BIT_X_R(0, &S_C); break;
					case 0x42: BIT_X_R(0, &S_D); break;
					case 0x43: BIT_X_R(0, &S_E); break;
					case 0x44: BIT_X_R(0, &S_H); break;
					case 0x45: BIT_X_R(0, &S_L); break;
					case 0x46: BIT_X_AA(0, &S_H, &S_L); break;
					case 0x47: BIT_X_R(0, &S_A); break;
					case 0x48: BIT_X_R(1, &S_B); break;
					case 0x49: BIT_X_R(1, &S_C); break;
					case 0x4A: BIT_X_R(1, &S_D); break;
					case 0x4B: BIT_X_R(1, &S_E); break;
					case 0x4C: BIT_X_R(1, &S_H); break;
					case 0x4D: BIT_X_R(1, &S_L); break;
					case 0x4E: BIT_X_AA(1, &S_H, &S_L); break;
					case 0x4F: BIT_X_R(1, &S_A); break;
					case 0x50: BIT_X_R(2, &S_B); break;
					case 0x51: BIT_X_R(2, &S_C); break;
					case 0x52: BIT_X_R(2, &S_D); break;
					case 0x53: BIT_X_R(2, &S_E); break;
					case 0x54: BIT_X_R(2, &S_H); break;
					case 0x55: BIT_X_R(2, &S_L); break;
					case 0x56: BIT_X_AA(2, &S_H, &S_L); break;
					case 0x57: BIT_X_R(2, &S_A); break;
					case 0x58: BIT_X_R(3, &S_B); break;
					case 0x59: BIT_X_R(3, &S_C); break;
					case 0x5A: BIT_X_R(3, &S_D); break;
					case 0x5B: BIT_X_R(3, &S_E); break;
					case 0x5C: BIT_X_R(3, &S_H); break;
					case 0x5D: BIT_X_R(3, &S_L); break;
					case 0x5E: BIT_X_AA(3, &S_H, &S_L); break;
					case 0x5F: BIT_X_R(3, &S_A); break;
					case 0x60: BIT_X_R(4, &S_B); break;
					case 0x61: BIT_X_R(4, &S_C); break;
					case 0x62: BIT_X_R(4, &S_D); break;
					case 0x63: BIT_X_R(4, &S_E); break;
					case 0x64: BIT_X_R(4, &S_H); break;
					case 0x65: BIT_X_R(4, &S_L); break;
					case 0x66: BIT_X_AA(4, &S_H, &S_L); break;
					case 0x67: BIT_X_R(4, &S_A); break;
					case 0x68: BIT_X_R(5, &S_B); break;
					case 0x69: BIT_X_R(5, &S_C); break;
					case 0x6A: BIT_X_R(5, &S_D); break;
					case 0x6B: BIT_X_R(5, &S_E); break;
					case 0x6C: BIT_X_R(5, &S_H); break;
					case 0x6D: BIT_X_R(5, &S_L); break;
					case 0x6E: BIT_X_AA(5, &S_H, &S_L); break;
					case 0x6F: BIT_X_R(5, &S_A); break;
					case 0x70: BIT_X_R(6, &S_B); break;
					case 0x71: BIT_X_R(6, &S_C); break;
					case 0x72: BIT_X_R(6, &S_D); break;
					case 0x73: BIT_X_R(6, &S_E); break;
					case 0x74: BIT_X_R(6, &S_H); break;
					case 0x75: BIT_X_R(6, &S_L); break;
					case 0x76: BIT_X_AA(6, &S_H, &S_L); break;
					case 0x77: BIT_X_R(6, &S_A); break;
					case 0x78: BIT_X_R(7, &S_B); break;
					case 0x79: BIT_X_R(7, &S_C); break;
					case 0x7A: BIT_X_R(7, &S_D); break;
					case 0x7B: BIT_X_R(7, &S_E); break;
					case 0x7C: BIT_X_R(7, &S_H); break;
					case 0x7D: BIT_X_R(7, &S_L); break;
					case 0x7E: BIT_X_AA(7, &S_H, &S_L); break;
					case 0x7F: BIT_X_R(7, &S_A); break;
					case 0x80: RES_X_R(0, &S_B); break;
					case 0x81: RES_X_R(0, &S_C); break;
					case 0x82: RES_X_R(0, &S_D); break;
					case 0x83: RES_X_R(0, &S_E); break;
					case 0x84: RES_X_R(0, &S_H); break;
					case 0x85: RES_X_R(0, &S_L); break;
					case 0x86: RES_X_AA(0, &S_H, &S_L); break;
					case 0x87: RES_X_R(0, &S_A); break;
					case 0x88: RES_X_R(1, &S_B); break;
					case 0x89: RES_X_R(1, &S_C); break;
					case 0x8A: RES_X_R(1, &S_D); break;
					case 0x8B: RES_X_R(1, &S_E); break;
					case 0x8C: RES_X_R(1, &S_H); break;
					case 0x8D: RES_X_R(1, &S_L); break;
					case 0x8E: RES_X_AA(1, &S_H, &S_L); break;
					case 0x8F: RES_X_R(1, &S_A); break;
					case 0x90: RES_X_R(2, &S_B); break;
					case 0x91: RES_X_R(2, &S_C); break;
					case 0x92: RES_X_R(2, &S_D); break;
					case 0x93: RES_X_R(2, &S_E); break;
					case 0x94: RES_X_R(2, &S_H); break;
					case 0x95: RES_X_R(2, &S_L); break;
					case 0x96: RES_X_AA(2, &S_H, &S_L); break;
					case 0x97: RES_X_R(2, &S_A); break;
					case 0x98: RES_X_R(3, &S_B); break;
					case 0x99: RES_X_R(3, &S_C); break;
					case 0x9A: RES_X_R(3, &S_D); break;
					case 0x9B: RES_X_R(3, &S_E); break;
					case 0x9C: RES_X_R(3, &S_H); break;
					case 0x9D: RES_X_R(3, &S_L); break;
					case 0x9E: RES_X_AA(3, &S_H, &S_L); break;
					case 0x9F: RES_X_R(3, &S_A); break;
					case 0xA0: RES_X_R(4, &S_B); break;
					case 0xA1: RES_X_R(4, &S_C); break;
					case 0xA2: RES_X_R(4, &S_D); break;
					case 0xA3: RES_X_R(4, &S_E); break;
					case 0xA4: RES_X_R(4, &S_H); break;
					case 0xA5: RES_X_R(4, &S_L); break;
					case 0xA6: RES_X_AA(4, &S_H, &S_L); break;
					case 0xA7: RES_X_R(4, &S_A); break;
					case 0xA8: RES_X_R(5, &S_B); break;
					case 0xA9: RES_X_R(5, &S_C); break;
					case 0xAA: RES_X_R(5, &S_D); break;
					case 0xAB: RES_X_R(5, &S_E); break;
					case 0xAC: RES_X_R(5, &S_H); break;
					case 0xAD: RES_X_R(5, &S_L); break;
					case 0xAE: RES_X_AA(5, &S_H, &S_L); break;
					case 0xAF: RES_X_R(5, &S_A); break;
					case 0xB0: RES_X_R(6, &S_B); break;
					case 0xB1: RES_X_R(6, &S_C); break;
					case 0xB2: RES_X_R(6, &S_D); break;
					case 0xB3: RES_X_R(6, &S_E); break;
					case 0xB4: RES_X_R(6, &S_H); break;
					case 0xB5: RES_X_R(6, &S_L); break;
					case 0xB6: RES_X_AA(6, &S_H, &S_L); break;
					case 0xB7: RES_X_R(6, &S_A); break;
					case 0xB8: RES_X_R(7, &S_B); break;
					case 0xB9: RES_X_R(7, &S_C); break;
					case 0xBA: RES_X_R(7, &S_D); break;
					case 0xBB: RES_X_R(7, &S_E); break;
					case 0xBC: RES_X_R(7, &S_H); break;
					case 0xBD: RES_X_R(7, &S_L); break;
					case 0xBE: RES_X_AA(7, &S_H, &S_L); break;
					case 0xBF: RES_X_R(7, &S_A); break;
					case 0xC0: SET_X_R(0, &S_B); break;
					case 0xC1: SET_X_R(0, &S_C); break;
					case 0xC2: SET_X_R(0, &S_D); break;
					case 0xC3: SET_X_R(0, &S_E); break;
					case 0xC4: SET_X_R(0, &S_H); break;
					case 0xC5: SET_X_R(0, &S_L); break;
					case 0xC6: SET_X_AA(0, &S_H, &S_L); break;
					case 0xC7: SET_X_R(0, &S_A); break;
					case 0xC8: SET_X_R(1, &S_B); break;
					case 0xC9: SET_X_R(1, &S_C); break;
					case 0xCA: SET_X_R(1, &S_D); break;
					case 0xCB: SET_X_R(1, &S_E); break;
					case 0xCC: SET_X_R(1, &S_H); break;
					case 0xCD: SET_X_R(1, &S_L); break;
					case 0xCE: SET_X_AA(1, &S_H, &S_L); break;
					case 0xCF: SET_X_R(1, &S_A); break;
					case 0xD0: SET_X_R(2, &S_B); break;
					case 0xD1: SET_X_R(2, &S_C); break;
					case 0xD2: SET_X_R(2, &S_D); break;
					case 0xD3: SET_X_R(2, &S_E); break;
					case 0xD4: SET_X_R(2, &S_H); break;
					case 0xD5: SET_X_R(2, &S_L); break;
					case 0xD6: SET_X_AA(2, &S_H, &S_L); break;
					case 0xD7: SET_X_R(2, &S_A); break;
					case 0xD8: SET_X_R(3, &S_B); break;
					case 0xD9: SET_X_R(3, &S_C); break;
					case 0xDA: SET_X_R(3, &S_D); break;
					case 0xDB: SET_X_R(3, &S_E); break;
					case 0xDC: SET_X_R(3, &S_H); break;
					case 0xDD: SET_X_R(3, &S_L); break;
					case 0xDE: SET_X_AA(3, &S_H, &S_L); break;
					case 0xDF: SET_X_R(3, &S_A); break;
					case 0xE0: SET_X_R(4, &S_B); break;
					case 0xE1: SET_X_R(4, &S_C); break;
					case 0xE2: SET_X_R(4, &S_D); break;
					case 0xE3: SET_X_R(4, &S_E); break;
					case 0xE4: SET_X_R(4, &S_H); break;
					case 0xE5: SET_X_R(4, &S_L); break;
					case 0xE6: SET_X_AA(4, &S_H, &S_L); break;
					case 0xE7: SET_X_R(4, &S_A); break;
					case 0xE8: SET_X_R(5, &S_B); break;
					case 0xE9: SET_X_R(5, &S_C); break;
					case 0xEA: SET_X_R(5, &S_D); break;
					case 0xEB: SET_X_R(5, &S_E); break;
					case 0xEC: SET_X_R(5, &S_H); break;
					case 0xED: SET_X_R(5, &S_L); break;
					case 0xEE: SET_X_AA(5, &S_H, &S_L); break;
					case 0xEF: SET_X_R(5, &S_A); break;
					case 0xF0: SET_X_R(6, &S_B); break;
					case 0xF1: SET_X_R(6, &S_C); break;
					case 0xF2: SET_X_R(6, &S_D); break;
					case 0xF3: SET_X_R(6, &S_E); break;
					case 0xF4: SET_X_R(6, &S_H); break;
					case 0xF5: SET_X_R(6, &S_L); break;
					case 0xF6: SET_X_AA(6, &S_H, &S_L); break;
					case 0xF7: SET_X_R(6, &S_A); break;
					case 0xF8: SET_X_R(7, &S_B); break;
					case 0xF9: SET_X_R(7, &S_C); break;
					case 0xFA: SET_X_R(7, &S_D); break;
					case 0xFB: SET_X_R(7, &S_E); break;
					case 0xFC: SET_X_R(7, &S_H); break;
					case 0xFD: SET_X_R(7, &S_L); break;
					case 0xFE: SET_X_AA(7, &S_H, &S_L); break;
					case 0xFF: SET_X_R(7, &S_A); break;
					default: break;
				}
				
				break;
			
			case 0xCC: CALL_NN_IF(GET_ZFLAG()); break;
			case 0xCD: CALL_NN(); break;
			case 0xCE: ADC_R_N(&S_A); break;
			case 0xCF: RST(0x08); break;
			case 0xD0: RET_IF(!GET_CFLAG()); break;
			case 0xD1: POP_RR(&S_D, &S_E); break;
			case 0xD2: JP_NN_IF(!GET_CFLAG()); break;
			case 0xD3: NOP(); /* NOT USED */ break;
			case 0xD4: CALL_NN_IF(!GET_CFLAG()); break;
			case 0xD5: PUSH_RR(&S_D, &S_E); break;
			case 0xD6: SUB_R_N(&S_A); break;
			case 0xD7: RST(0x10); break;
			case 0xD8: RET_IF(GET_CFLAG()); break;
			case 0xD9: RETI(); break;
			case 0xDA: JP_NN_IF(GET_CFLAG()); break;
			case 0xDB: NOP(); /* NOT USED */ break;
			case 0xDC: CALL_NN_IF(GET_CFLAG()); break;
			case 0xDD: NOP(); /* NOT USED */ break;
			case 0xDE: SBC_R_N(&S_A); break;
			case 0xDF: RST(0x18); break;
			case 0xE0: LDH_A_R(&GET_OP_LOW(), &S_A); break;
			case 0xE1: POP_RR(&S_H, &S_L); break;
			case 0xE2: LD_A_R(&S_C, &S_A); break;
			case 0xE3: NOP(); /* NOT USED */ break;
			case 0xE4: NOP(); /* NOT USED */ break;
			case 0xE5: PUSH_RR(&S_H, &S_L); break;
			case 0xE6: AND_R_N(&S_A); break;
			case 0xE7: RST(0x20); break;
			case 0xE8: ADD_SP_N(); break;
			case 0xE9: JP_RR(&S_H, &S_L); break;
			case 0xEA: LD_AA_R(&opHigh, &GET_OP_LOW(), &S_A); break;
			case 0xEB: NOP(); /* NOT USED */ break;
			case 0xEC: NOP(); /* NOT USED */ break;
			case 0xED: NOP(); /* NOT USED */ break;
			case 0xEE: XOR_R_N(&S_A); break;
			case 0xEF: RST(0x28); break;
			case 0xF0: LDH_R_A(&S_A, &GET_OP_LOW()); break;
			case 0xF1: POP_AF(); break;
			case 0xF2: LD_R_A(&S_A, &S_C); break;
			case 0xF3: DI(); break;
			case 0xF4: NOP(); /* NOT USED */ break;
			case 0xF5: PUSH_AF(); break;
			case 0xF6: OR_R_N(&S_A); break;
			case 0xF7: RST(0x30); break;
			case 0xF8: LD_HL_SP_N(); break;
			case 0xF9: LD_SP_HL(); break;
			case 0xFA: LD_R_AA(&S_A, &GET_OP_HIGH(), &GET_OP_LOW()); break;
			case 0xFB: EI(); break;
			case 0xFC: NOP(); /* NOT USED */ break;
			case 0xFD: NOP(); /* NOT USED */ break;
			case 0xFE: CP_R_N(&S_A); break;
			case 0xFF: RST(0x38); break;
			default: break;
		}
	}
}

void gbc::core::cpu::Processor::ExecuteInterrupt()
{
	if (S_INTERRUPTS_ENABLED || S_HALTED)
	{
		if ((GET_BIT(READ(0xFFFF), 0) && GET_BIT(READ(0xFF0F), 0)) ||
		    (GET_BIT(READ(0xFFFF), 1) && GET_BIT(READ(0xFF0F), 1)) ||
		    (GET_BIT(READ(0xFFFF), 2) && GET_BIT(READ(0xFF0F), 2)) ||
		    (GET_BIT(READ(0xFFFF), 3) && GET_BIT(READ(0xFF0F), 3)) ||
		    (GET_BIT(READ(0xFFFF), 4) && GET_BIT(READ(0xFF0F), 4)))
		{int washalted = S_HALTED;
			S_INTERRUPTS_ENABLED = GBC_FALSE;
			S_STOPPED = GBC_FALSE;
			S_HALTED = GBC_FALSE;
			
			S_SP -= 2;
			
			WRITE(S_SP, S_PC & 0xFF);
			WRITE(S_SP + 1, (S_PC >> 8) & 0xFF);
			
			if (GET_BIT(READ(0xFFFF), 0) && GET_BIT(READ(0xFF0F), 0))
			{
				S_PC = 0x0040;
			}
			else if (GET_BIT(READ(0xFFFF), 1) && GET_BIT(READ(0xFF0F), 1))
			{
				S_PC = 0x0048;
			}
			else if (GET_BIT(READ(0xFFFF), 2) && GET_BIT(READ(0xFF0F), 2))
			{
				S_PC = 0x0050;
			}
			else if (GET_BIT(READ(0xFFFF), 3) && GET_BIT(READ(0xFF0F), 3))
			{
				S_PC = 0x0058;
			}
			else if (GET_BIT(READ(0xFFFF), 4) && GET_BIT(READ(0xFF0F), 4))
			{
				S_PC = 0x0060;
			}
			
			if (GET_BIT(READ(0xFFFF), 0) && GET_BIT(READ(0xFF0F), 0))
			{
				int result = READ(0xFF0F);
				result = SET_BIT(result, 0, GBC_FALSE);
				WRITE(0xFF0F, result);
			}
			else if (GET_BIT(READ(0xFFFF), 1) && GET_BIT(READ(0xFF0F), 1))
			{
				int result = READ(0xFF0F);
				result = SET_BIT(result, 1, GBC_FALSE);
				WRITE(0xFF0F, result);
			}
			else if (GET_BIT(READ(0xFFFF), 2) && GET_BIT(READ(0xFF0F), 2))
			{
				int result = READ(0xFF0F);
				result = SET_BIT(result, 2, GBC_FALSE);
				WRITE(0xFF0F, result);
			}
			else if (GET_BIT(READ(0xFFFF), 3) && GET_BIT(READ(0xFF0F), 3))
			{
				int result = READ(0xFF0F);
				result = SET_BIT(result, 3, GBC_FALSE);
				WRITE(0xFF0F, result);
			}
			else if (GET_BIT(READ(0xFFFF), 4) && GET_BIT(READ(0xFF0F), 4))
			{
				int result = READ(0xFF0F);
				result = SET_BIT(result, 4, GBC_FALSE);
				WRITE(0xFF0F, result);
			}
			
			S_TICKS += 16;
		}
	}
}

void gbc::core::cpu::Processor::PowerUp()
{
	S_A = 0x01;
	//S_A = 0x11; //test
	S_F = 0xB0;
	S_B = 0x00;
	S_C = 0x13;
	S_D = 0x00;
	S_E = 0xD8;
	S_H = 0x01;
	S_L = 0x4D;
	S_PC = 0x0100;
	S_SP = 0xFFFE;
	
	S_INTERRUPTS_ENABLED = GBC_TRUE;
	S_HALTED = GBC_FALSE;
	S_STOPPED = GBC_FALSE;
	
	WRITE(0xFF05, 0x00); // TIMA
	WRITE(0xFF06, 0x00); // TMA
	WRITE(0xFF07, 0x00); // TAC
	WRITE(0xFF10, 0x80); // NR10
	WRITE(0xFF11, 0xBF); // NR11
	WRITE(0xFF12, 0xF3); // NR12
	WRITE(0xFF14, 0xBF); // NR14
	WRITE(0xFF16, 0x3F); // NR21
	WRITE(0xFF17, 0x00); // NR22
	WRITE(0xFF19, 0xBF); // NR24
	WRITE(0xFF1A, 0x7F); // NR30
	WRITE(0xFF1B, 0xFF); // NR31
	WRITE(0xFF1C, 0x9F); // NR32
	WRITE(0xFF1E, 0xBF); // NR33
	WRITE(0xFF20, 0xFF); // NR41
	WRITE(0xFF21, 0x00); // NR42
	WRITE(0xFF22, 0x00); // NR43
	WRITE(0xFF23, 0xBF); // NR30
	WRITE(0xFF24, 0x77); // NR50
	WRITE(0xFF25, 0xF3); // NR51
	WRITE(0xFF26, 0xF1); // NR52
	WRITE(0xFF40, 0x91); // LCDC
	WRITE(0xFF42, 0x00); // SCY
	WRITE(0xFF43, 0x00); // SCX
	WRITE(0xFF45, 0x00); // LYC
	WRITE(0xFF47, 0xFC); // BGP
	WRITE(0xFF48, 0xFF); // OBP0
	WRITE(0xFF49, 0xFF); // OBP1
	WRITE(0xFF4A, 0x00); // WY
	WRITE(0xFF4B, 0x00); // WX
	WRITE(0xFFFF, 0x00); // IE
}

void gbc::core::cpu::Processor::SetState(gbc::core::cpu::State state)
{
	_state = state;
}

gbc::core::cpu::State gbc::core::cpu::Processor::GetState()
{
	return _state;
}

int gbc::core::cpu::Processor::GetCounter()
{
	return _counter;
}

// default instruction set

void gbc::core::cpu::Processor::NOP()
{
	FETCH_OP_CODE();
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::INC_RR(int *r1, int *r2)
{
	FETCH_OP_CODE();
	(*r2)++;
	if ((*r2) > 0xFF)
	{
		(*r1)++;
		(*r1) &= 0xFF;
		(*r2) &= 0xFF;
	}
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::INC_R(int *r)
{
	FETCH_OP_CODE();
	SET_HFLAG(((*r) & 0x0F) == 0x0F);
	(*r)++;
	(*r) &= 0xFF;
	SET_ZFLAG((*r) == 0x00);
	SET_NFLAG(GBC_FALSE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::INC_AA(int *a1, int *a2)
{
	FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	SET_HFLAG((memory & 0x0F) == 0x0F);
	memory++;
	memory &= 0xFF;
	SET_ZFLAG(memory == 0x00);
	SET_NFLAG(GBC_FALSE);
	WRITE(address, memory);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::INC_SP()
{
	FETCH_OP_CODE();
	S_SP++;
	S_SP &= 0xFFFF;
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::DEC_AA(int *a1, int *a2)
{
	FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	SET_HFLAG((memory & 0x0F) == 0x00);
	memory--;
	memory &= 0xFF;
	SET_ZFLAG(memory == 0x00);
	SET_NFLAG(GBC_TRUE);
	WRITE(address, memory);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::DEC_RR(int *r1, int *r2)
{
	FETCH_OP_CODE();
	(*r2)--;
	if ((*r2) < 0x00)
	{
		(*r1)--;
		(*r1) &= 0xFF;
		(*r2) &= 0xFF;
	}
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::DEC_R(int *r)
{
	FETCH_OP_CODE();
	SET_HFLAG(((*r) & 0x0F) == 0x00);
	(*r)--;
	(*r) &= 0xFF;
	SET_ZFLAG((*r) == 0x00);
	SET_NFLAG(GBC_TRUE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::DEC_SP()
{
	FETCH_OP_CODE();
	S_SP--;
	S_SP &= 0xFF;
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_RR_NN(int *r1, int *r2)
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	FETCH_OP_HIGH();
	(*r1) = GET_OP_HIGH();
	(*r2) = GET_OP_LOW();
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_SP_NN()
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	FETCH_OP_HIGH();
	S_SP = JOIN_BYTES(GET_OP_HIGH(), GET_OP_LOW());
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RLCA()
{
	FETCH_OP_CODE();
	SET_CFLAG(((S_A >> 7) & 0x01) != 0x00);
	S_A = ((S_A << 1) & 0xFF) | ((S_A >> 7) & 0x01);
	SET_ZFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_AA_SP(int *a1, int *a2)
{
	FETCH_OP_CODE();
	WRITE(JOIN_BYTES(*a1, *a2), GET_LOW(S_SP));
	WRITE((JOIN_BYTES(*a1, *a2) + 1) & 0xFFFF, GET_HIGH(S_SP));
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::ADD_RR_RR(int *r11, int *r12, int *r21, int *r22)
{
	FETCH_OP_CODE();
	int r1Full = JOIN_BYTES(*r11, *r12);
	int r2Full = JOIN_BYTES(*r21, *r22);
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG(((r1Full & 0x0FFF) + (r2Full & 0x0FFF)) > 0x0FFF);
	int result = (r1Full & 0xFFFF) + (r2Full & 0xFFFF);
	SET_CFLAG(result > 0xFFFF);
	(*r11) = GET_HIGH(result);
	(*r12) = GET_LOW(result);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::ADD_RR_SP(int *r1, int *r2)
{
	FETCH_OP_CODE();
	int rFull = JOIN_BYTES(*r1, *r2);
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG((rFull & 0x0FFF) + (S_SP & 0x0FFF)) > 0x0FFF;
	int result = (rFull + S_SP) & 0xFFFF;
	SET_CFLAG(result > 0xFFFF);
	(*r1) = GET_HIGH(result);
	(*r2) = GET_LOW(result);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_R_AA(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	(*r) = READ(JOIN_BYTES(*a1, *a2));
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_R_N(int *r)
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	(*r) = GET_OP_LOW();
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_AA_N(int *a1, int *a2)
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	WRITE(JOIN_BYTES(*a1, *a2), GET_OP_LOW());
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RRCA()
{
	FETCH_OP_CODE();
	SET_CFLAG((S_A & 0x01) != 0x00);
	S_A = ((S_A >> 1) & 0xFF) | ((S_A & 0x01) << 7);
	SET_ZFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::STOP()
{
	FETCH_OP_CODE();
	S_STOPPED = GBC_TRUE;
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RLA()
{
	FETCH_OP_CODE();
	int old_cflag = GET_CFLAG();
	SET_CFLAG(((S_A >> 7) & 0x01) != 0x00);
	S_A = ((S_A << 1) & 0xFF) | (old_cflag ? 0x01 : 0x00);
	SET_ZFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::JR_N()
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	S_PC += GET_SIGNED_VALUE(GET_OP_LOW());
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RRA()
{
	FETCH_OP_CODE();
	int old_cflag = GET_CFLAG();
	SET_CFLAG((S_A & 0x01) != 0);
	S_A = ((S_A >> 1) & 0x7F) | (old_cflag ? 0x80 : 0x00);
	SET_ZFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::JR_N_IF(int c)
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	if (c)
	{
		S_PC += GET_SIGNED_VALUE(GET_OP_LOW());
		UPDATE_ADDITIONAL_TICKS();
	}
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_AA_R_INC(int *a1, int *a2, int *r)
{
	FETCH_OP_CODE();
	WRITE(JOIN_BYTES(*a1, *a2), *r);
	(*a2)++;
	if ((*a2) > 0xFF)
	{
		(*a1)++;
		(*a1) &= 0xFF;
		(*a2) &= 0xFF;
	}
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_AA_R_DEC(int *a1, int *a2, int *r)
{
	FETCH_OP_CODE();
	WRITE(JOIN_BYTES(*a1, *a2), (*r));
	(*a2)--;
	if ((*a2) < 0x00)
	{
		(*a1)--;
		(*a1) &= 0xFF;
		(*a2) &= 0xFF;
	}
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_R_AA_INC(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	(*r) = READ(JOIN_BYTES(*a1, *a2));
	(*a2)++;
	if ((*a2) > 0xFF)
	{
		(*a1)++;
		(*a1) &= 0xFF;
		(*a2) &= 0xFF;
	}
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_R_AA_DEC(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	(*r) = READ(JOIN_BYTES(*a1, *a2));
	(*a2)--;
	if ((*a2) < 0x00)
	{
		(*a1)--;
		(*a1) &= 0xFF;
		(*a2) &= 0xFF;
	}
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::DAA()
{
	FETCH_OP_CODE();
	
	int highNibble = S_A >> 4;
    int lowNibble = S_A & 0x0F;
    
    bool newCarry = GBC_TRUE;
    
	if (GET_NFLAG())
    {
		if (GET_CFLAG())
		{
			if (GET_HFLAG())
			{
				S_A += 0x9A;
			}
			else
			{
				S_A += 0xA0;
			}
		}
		else
		{
			newCarry = GBC_FALSE;
			
			if (GET_HFLAG())
			{
				S_A += 0xFA;
			}
			else
			{
				S_A += 0x00;
			}
		}
	}
	else if (GET_CFLAG())
	{
		if (GET_HFLAG() || lowNibble > 9)
		{
			S_A += 0x66;
		}
		else
		{
			S_A += 0x60;
		}
	}
	else if (GET_HFLAG())
	{
		if (highNibble > 9)
		{
			S_A += 0x66;
		}
		else
		{
			S_A += 0x06;
			newCarry = GBC_FALSE;
		}
	}
	else if (lowNibble > 9)
	{
		if (highNibble < 9)
		{
			newCarry = GBC_FALSE;
			S_A += 0x06;
		}
		else
		{
			S_A += 0x66;
		}
	}
	else if (highNibble > 9)
	{
		S_A += 0x60;
	}
	else
	{
		newCarry = GBC_FALSE;
	}
	
	S_A &= 0xFF;
	
	SET_CFLAG(newCarry);
	SET_ZFLAG(S_A == 0);
	
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::CPL()
{
	FETCH_OP_CODE();
	SET_CFLAG(S_A == 0);
	S_A ^= 0xFF;
	S_A &= 0xFF;
	SET_NFLAG(GBC_TRUE);
	SET_HFLAG(GBC_TRUE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SCF()
{
	FETCH_OP_CODE();
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_CFLAG(GBC_TRUE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::CCF()
{
	FETCH_OP_CODE();
	SET_HFLAG(GET_CFLAG());
	SET_CFLAG(!GET_CFLAG());
	SET_NFLAG(GBC_FALSE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_R_R(int *r1, int *r2)
{
	FETCH_OP_CODE();
	(*r1) = (*r2);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::HALT()
{
	FETCH_OP_CODE();
	S_HALTED = GBC_TRUE;
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::ADD_R_R(int *r1, int *r2)
{
	FETCH_OP_CODE();
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG((((*r1) & 0x0F) + ((*r2) & 0x0F)) > 0x0F);
	(*r1) += (*r2);
	SET_CFLAG((*r1) > 0xFF);
	(*r1) &= 0xFF;
	SET_ZFLAG((*r1) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::ADD_R_AA(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	int memory = READ(JOIN_BYTES(*a1, *a2));
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG((((*r) & 0x0F) + (memory & 0x0F)) > 0x0F);
	(*r) += memory;
	SET_CFLAG((*r) > 0xFF);
	(*r) &= 0xFF;
	SET_ZFLAG((*r) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::ADC_R_R(int *r1, int *r2)
{
	FETCH_OP_CODE();
	int old_cflag = GET_CFLAG();
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG((((*r1) & 0x0F) + ((*r2) & 0x0F) + (old_cflag ? 0x01 : 0x00)) > 0x0F);
	(*r1) += (*r2) + (old_cflag ? 0x01 : 0x00);
	SET_CFLAG((*r1) > 0xFF);
	(*r1) &= 0xFF;
	SET_ZFLAG((*r1) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::ADC_R_AA(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	int memory = READ(JOIN_BYTES(*a1, *a2));
	int old_cflag = GET_CFLAG();
	SET_NFLAG(GBC_FALSE);
	SET_HFLAG((((*r) & 0x0F) + (memory & 0x0F) + (old_cflag ? 0x01 : 0x00)) > 0x0F);
	(*r) += memory + (old_cflag ? 0x01 : 0x00);
	SET_CFLAG((*r) > 0xFF);
	(*r) &= 0xFF;
	SET_ZFLAG((*r) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SUB_R_R(int *r1, int *r2)
{
	FETCH_OP_CODE();
	SET_NFLAG(GBC_TRUE);
	SET_HFLAG((((*r1) & 0x0F) - ((*r2) & 0x0F)) < 0x00);
	(*r1) -= (*r2);
	SET_CFLAG((*r1) < 0x00);
	(*r1) &= 0xFF;
	SET_ZFLAG((*r1) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SUB_R_AA(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	int memory = READ(JOIN_BYTES(*a1, *a2));
	SET_NFLAG(GBC_TRUE);
	SET_HFLAG((((*r) & 0x0F) - (memory & 0x0F)) < 0x00);
	(*r) -= memory;
	SET_CFLAG((*r) < 0x00);
	(*r) &= 0xFF;
	SET_ZFLAG((*r) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SBC_R_R(int *r1, int *r2)
{
	FETCH_OP_CODE();
	int old_cflag = GET_CFLAG();
	SET_NFLAG(GBC_TRUE);
	SET_HFLAG((((*r1) & 0x0F) - (((*r2) & 0x0F) + (old_cflag ? 0x01 : 0x00))) < 0x00);
	(*r1) -= ((*r2) + (old_cflag ? 0x01 : 0x00));
	SET_CFLAG((*r1) < 0x00);
	(*r1) &= 0xFF;
	SET_ZFLAG((*r1) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SBC_R_AA(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	int memory = READ(JOIN_BYTES(*a1, *a2));
	int old_cflag = GET_CFLAG();
	SET_NFLAG(GBC_TRUE);
	SET_HFLAG((((*r) & 0x0F) - ((memory & 0x0F) + (old_cflag ? 0x01 : 0x00))) < 0x00);
	(*r) -= (memory + (old_cflag ? 0x01 : 0x00));
	SET_CFLAG((*r) < 0x00);
	(*r) &= 0xFF;
	SET_ZFLAG((*r) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::AND_R_R(int *r1, int *r2)
{
	FETCH_OP_CODE();
	(*r1) &= (*r2);
	SET_CFLAG(GBC_FALSE);
	SET_HFLAG(GBC_TRUE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r1) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::AND_R_AA(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	int memory = READ(JOIN_BYTES(*a1, *a2));
	(*r) &= memory;
	SET_CFLAG(GBC_FALSE);
	SET_HFLAG(GBC_TRUE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::XOR_R_R(int *r1, int *r2)
{
	FETCH_OP_CODE();
	(*r1) ^= (*r2);
	SET_CFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r1) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::XOR_R_AA(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	int memory = READ(JOIN_BYTES(*a1, *a2));
	(*r) ^= memory;
	SET_CFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::OR_R_R(int *r1, int *r2)
{
	FETCH_OP_CODE();
	(*r1) |= (*r2);
	SET_CFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r1) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::OR_R_AA(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	int memory = READ(JOIN_BYTES(*a1, *a2));
	(*r) |= memory;
	SET_CFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::CP_R_R(int *r1, int *r2)
{
	FETCH_OP_CODE();
	SET_CFLAG((*r2) > (*r1));
	SET_HFLAG(((*r2) & 0x0F) > ((*r1) & 0x0F));
	SET_NFLAG(GBC_TRUE);
	SET_ZFLAG((*r2) == (*r1));
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::CP_R_AA(int *r, int *a1, int *a2)
{
	FETCH_OP_CODE();
	int memory = READ(JOIN_BYTES(*a1, *a2));
	SET_CFLAG(memory > (*r));
	SET_HFLAG((memory & 0x0F) > ((*r) & 0x0F));
	SET_NFLAG(GBC_TRUE);
	SET_ZFLAG(memory == (*r));
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::CALL_NN()
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	FETCH_OP_HIGH();
	S_SP -= 2;
	WRITE(S_SP + 1, GET_HIGH(S_PC + LENGTH));
	WRITE(S_SP, GET_LOW(S_PC + LENGTH));
	S_PC = JOIN_BYTES(GET_OP_HIGH(), GET_OP_LOW());
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::CALL_NN_IF(int c)
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	FETCH_OP_HIGH();
	if (c)
	{
		S_SP -= 2;
		WRITE(S_SP + 1, GET_HIGH(S_PC + LENGTH));
		WRITE(S_SP, GET_LOW(S_PC + LENGTH));
		S_PC = JOIN_BYTES(GET_OP_HIGH(), GET_OP_LOW());
		UPDATE_ADDITIONAL_TICKS();
	}
	else
	{
		UPDATE_PC();
	}
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RET()
{
	FETCH_OP_CODE();
	S_PC = JOIN_BYTES(READ(S_SP + 1), READ(S_SP));
	S_SP += 2;
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RET_IF(int c)
{
	FETCH_OP_CODE();
	if (c)
	{
		S_PC = JOIN_BYTES(READ(S_SP + 1), READ(S_SP));
		S_SP += 2;
		UPDATE_ADDITIONAL_TICKS();
	}
	else
	{
		UPDATE_PC();
	}
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RETI()
{
	FETCH_OP_CODE();
	S_INTERRUPTS_ENABLED = GBC_TRUE;
	S_HALTED = GBC_FALSE;
	S_PC = (READ(S_SP + 1) << 8) | READ(S_SP);
	S_SP += 2;
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::JP_NN()
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	FETCH_OP_HIGH();
	S_PC = JOIN_BYTES(GET_OP_HIGH(), GET_OP_LOW());
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::JP_NN_IF(int c)
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	FETCH_OP_HIGH();
	if (c)
	{
		S_PC = JOIN_BYTES(GET_OP_HIGH(), GET_OP_LOW());
		UPDATE_ADDITIONAL_TICKS();
	}
	else
	{
		UPDATE_PC();
	}
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::JP_RR(int *r1, int *r2)
{
	FETCH_OP_CODE();
	S_PC = JOIN_BYTES(*r1, *r2);
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::POP_RR(int *r1, int *r2)
{
	FETCH_OP_CODE();
	(*r1) = READ(S_SP + 1);
	(*r2) = READ(S_SP);
	S_SP += 2;
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::POP_AF()
{
	FETCH_OP_CODE();
	S_A = READ(S_SP + 1);
	S_F = READ(S_SP) & 0xF0;
	S_SP += 2;
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::PUSH_RR(int *r1, int *r2)
{
	FETCH_OP_CODE();
	S_SP -= 2;
	WRITE(S_SP + 1, *r1);
	WRITE(S_SP, *r2);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::PUSH_AF()
{
	FETCH_OP_CODE();
	S_SP -= 2;
	WRITE(S_SP + 1, S_A);
	WRITE(S_SP, S_F & 0xF0);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::ADD_R_N(int *r)
{
	FETCH_OP_LOW();
	ADD_R_R(r, &GET_OP_LOW());
}

void gbc::core::cpu::Processor::ADC_R_N(int *r)
{
	FETCH_OP_LOW();
	ADC_R_R(r, &GET_OP_LOW());
}

void gbc::core::cpu::Processor::SUB_R_N(int *r)
{
	FETCH_OP_LOW();
	SUB_R_R(r, &GET_OP_LOW());
}

void gbc::core::cpu::Processor::SBC_R_N(int *r)
{
	FETCH_OP_LOW();
	SBC_R_R(r, &GET_OP_LOW());
}

void gbc::core::cpu::Processor::AND_R_N(int *r)
{
	FETCH_OP_LOW();
	AND_R_R(r, &GET_OP_LOW());
}

void gbc::core::cpu::Processor::XOR_R_N(int *r)
{
	FETCH_OP_LOW();
	XOR_R_R(r, &GET_OP_LOW());
}

void gbc::core::cpu::Processor::OR_R_N(int *r)
{
	FETCH_OP_LOW();
	OR_R_R(r, &GET_OP_LOW());
}

void gbc::core::cpu::Processor::CP_R_N(int *r)
{
	FETCH_OP_LOW();
	CP_R_R(r, &GET_OP_LOW());
}

void gbc::core::cpu::Processor::RST(int a)
{
	FETCH_OP_CODE();
	S_SP -= 2;
	WRITE(S_SP + 1, GET_HIGH(S_PC + LENGTH));
	WRITE(S_SP, GET_LOW(S_PC + LENGTH));
	S_PC = a;
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LDH_A_R(int *a, int *r)
{
	LD_A_R(a, r);
}

void gbc::core::cpu::Processor::LDH_R_A(int *r, int *a)
{
	LD_R_A(r, a);
}

void gbc::core::cpu::Processor::LD_A_R(int *a, int *r)
{
	FETCH_OP_CODE();
	WRITE(JOIN_BYTES(0xFF, *a), *r);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_R_A(int *r, int *a)
{
	FETCH_OP_CODE();
	(*r) = READ(JOIN_BYTES(0xFF, *a));
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_AA_R(int *a1, int *a2, int *r)
{
	FETCH_OP_CODE();
	WRITE(JOIN_BYTES(*a1, *a2), *r);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::ADD_SP_N()
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	S_SP += GET_SIGNED_VALUE(GET_OP_LOW());
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(GBC_FALSE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_HL_SP_N()
{
	FETCH_OP_CODE();
	FETCH_OP_LOW();
	int result = (S_SP + GET_SIGNED_VALUE(GET_OP_LOW())) & 0xFFFF;
	S_H = GET_HIGH(result);
	S_L = GET_LOW(result);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(GBC_FALSE);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::LD_SP_HL()
{
	FETCH_OP_CODE();
	S_SP = JOIN_BYTES(S_H, S_L);
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::DI()
{
	FETCH_OP_CODE();
	S_INTERRUPTS_ENABLED = GBC_FALSE;
	UPDATE_PC();
	UPDATE_TICKS();
}

void gbc::core::cpu::Processor::EI()
{
	FETCH_OP_CODE();
	S_INTERRUPTS_ENABLED = GBC_TRUE;
	UPDATE_PC();
	UPDATE_TICKS();
}

// cb instruction set

void gbc::core::cpu::Processor::RLC_R(int *r)
{
	CB_FETCH_OP_CODE();
	SET_CFLAG(GET_BIT(*r, 7));
	(*r) = (((*r) << 1) & 0xFF) | (GET_BIT((*r), 7) ? 0x01 : 0x00);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RLC_AA(int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	SET_CFLAG(GET_BIT(memory, 7));
	memory = ((memory << 1) & 0xFF) | (GET_BIT(memory, 7) ? 0x01 : 0x00);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(memory == 0x00);
	WRITE(address, memory);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RRC_R(int *r)
{
	CB_FETCH_OP_CODE();
	SET_CFLAG(GET_BIT(*r, 0));
	(*r) = (((*r) >> 1) & 0xFF) | (GET_BIT(*r, 0) ? 0x80 : 0x00);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RRC_AA(int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	SET_CFLAG(GET_BIT(memory, 0));
	memory = ((memory >> 1) & 0xFF) | (GET_BIT(memory, 0) ? 0x80 : 0x00);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(memory == 0x00);
	WRITE(address, memory);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RL_R(int *r)
{
	CB_FETCH_OP_CODE();
	int old_cflag = GET_CFLAG();
	SET_CFLAG(GET_BIT((*r), 7));
	(*r) = (((*r) << 1) & 0xFF) | (old_cflag ? 0x01 : 0x00);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RL_AA(int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	int old_cflag = GET_CFLAG();
	SET_CFLAG(GET_BIT(memory, 7));
	memory = ((memory << 1) & 0xFF) | (old_cflag ? 0x01 : 0x00);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(memory == 0x00);
	WRITE(address, memory);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RR_R(int *r)
{
	CB_FETCH_OP_CODE();
	int old_cflag = GET_CFLAG();
	SET_CFLAG(GET_BIT((*r), 0));
	(*r) = (((*r) >> 1) & 0xFF) | (old_cflag ? 0x80 : 0x00);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RR_AA(int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	int old_cflag = GET_CFLAG();
	SET_CFLAG(GET_BIT(memory, 0));
	memory = ((memory >> 1) & 0xFF) | (old_cflag ? 0x80 : 0x00);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(memory == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SLA_R(int *r)
{
	CB_FETCH_OP_CODE();
	SET_CFLAG(GET_BIT(*r, 7));
	(*r) = ((*r) << 1) & 0xFF;
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SLA_AA(int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	SET_CFLAG(GET_BIT(memory, 7));
	memory = (memory << 1) & 0xFF;
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(memory == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SRA_R(int *r)
{
	CB_FETCH_OP_CODE();
	SET_CFLAG(GET_BIT((*r), 0));
	(*r) = ((*r) & 0x80) | (((*r) >> 1) & 0xFF);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SRA_AA(int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	SET_CFLAG(GET_BIT(memory, 0));
	memory = (memory & 0x80) | ((memory >> 1) & 0xFF);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(memory == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SWAP_R(int *r)
{
	CB_FETCH_OP_CODE();
	(*r) = (((*r) << 4) | ((*r) >> 4)) & 0xFF;
	SET_CFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SWAP_AA(int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	memory = ((memory << 4) | (memory >> 4)) & 0xFF;
	SET_CFLAG(GBC_FALSE);
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(memory == 0x00);
	WRITE(address, memory);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SRL_R(int *r)
{
	CB_FETCH_OP_CODE();
	SET_CFLAG(GET_BIT(*r, 0));
	(*r) = ((*r) >> 1) & 0xFF;
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG((*r) == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SRL_AA(int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	SET_CFLAG(GET_BIT(memory, 0));
	memory = (memory >> 1) & 0xFF;
	SET_HFLAG(GBC_FALSE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(memory == 0x00);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::BIT_X_R(int x, int *r)
{
	CB_FETCH_OP_CODE();
	SET_HFLAG(GBC_TRUE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(!GET_BIT(*r, x));
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::BIT_X_AA(int x, int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	SET_HFLAG(GBC_TRUE);
	SET_NFLAG(GBC_FALSE);
	SET_ZFLAG(!GET_BIT(memory, x));
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RES_X_R(int x, int *r)
{
	CB_FETCH_OP_CODE();
	(*r) = SET_BIT(*r, x, GBC_FALSE);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::RES_X_AA(int x, int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	memory = SET_BIT(memory, x, GBC_FALSE);
	WRITE(address, memory);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SET_X_R(int x, int *r)
{
	CB_FETCH_OP_CODE();
	(*r) = SET_BIT(*r, x, GBC_TRUE);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}

void gbc::core::cpu::Processor::SET_X_AA(int x, int *a1, int *a2)
{
	CB_FETCH_OP_CODE();
	int address = JOIN_BYTES(*a1, *a2);
	int memory = READ(address);
	memory = SET_BIT(memory, x, GBC_TRUE);
	WRITE(address, memory);
	CB_UPDATE_PC();
	CB_UPDATE_TICKS();
}
