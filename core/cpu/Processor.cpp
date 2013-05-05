#include "Processor.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cpu;

Processor::Processor()
	: _counter(0), _state(), _bus(NULL)
{
}

Processor::~Processor()
{
}

void Processor::SetMemoryBus(IMemoryBus *bus)
{
	_bus = bus;
}

void Processor::Step()
{
	if (_state.ticks > 0)
	{
		_state.ticks--;
	}
	else
	{
		ExecuteInterrupt();
		ExecuteInstruction();
	}
}

void Processor::ExecuteInstruction()
{
	if (!_state.halted)
	{
		FetchInstruction(InstructionTable::DEFAULT);
		
		/*std::ostringstream oss;
		
		oss << "COUNTER=" << ToDec(_counter) << ", "
		    << "OP_CODE=" << ToHex(GET_OP_CODE()) << ", "
		    << "OP_LOW=" << ToHex(GET_OP_LOW()) << ", "
		    << "OP_HIGH=" << ToHex(GetOpHigh(InstructionTable::DEFAULT)) << ", "
		    << "A=" << ToHex(_state.a) << ", "
		    << "F=" << ToHex(_state.f) << ", "
		    << "B=" << ToHex(_state.b) << ", "
		    << "C=" << ToHex(_state.c) << ", "
		    << "D=" << ToHex(_state.d) << ", "
		    << "E=" << ToHex(_state.e) << ", "
		    << "H=" << ToHex(_state.h) << ", "
		    << "L=" << ToHex(_state.l) << ", "
		    << "(HL)=" << ToHex(_bus->ReadByte(JoinBytes(_state.h, _state.l))) << ", "
		    << "PC=" << ToHex(_state.pc) << ", "
		    << "SP=" << ToHex(_state.sp) << ", "
		    << "POP_LOW=" << ToHex(_bus->ReadByte(_state.sp)) << ", "
		    << "POP_HIGH=" << ToHex(_bus->ReadByte(_state.sp + 1));
		
		CPU_LOG(oss.str());*/
		
		_counter++;
		
		switch (GetOpCode(InstructionTable::DEFAULT))
		{
			case 0x00: NOP(); break;
			case 0x01: LD_RR_NN(_state.b, _state.c); break;
			case 0x02: LD_AA_R(_state.b, _state.c, _state.a); break;
			case 0x03: INC_RR(_state.b, _state.c); break;
			case 0x04: INC_R(_state.b); break;
			case 0x05: DEC_R(_state.b); break;
			case 0x06: LD_R_N(_state.b); break;
			case 0x07: RLCA(); break;
			case 0x08: LD_AA_SP(GetOpHigh(InstructionTable::DEFAULT), GetOpLow(InstructionTable::DEFAULT)); break;
			case 0x09: ADD_RR_RR(_state.h, _state.l, _state.b, _state.c); break;
			case 0x0A: LD_R_AA(_state.a, _state.b, _state.c); break;
			case 0x0B: DEC_RR(_state.b, _state.c); break;
			case 0x0C: INC_R(_state.c); break;
			case 0x0D: DEC_R(_state.c); break;
			case 0x0E: LD_R_N(_state.c); break;
			case 0x0F: RRCA(); break;
			case 0x10: STOP(); break;
			case 0x11: LD_RR_NN(_state.d, _state.e); break;
			case 0x12: LD_AA_R(_state.d, _state.e, _state.a); break;
			case 0x13: INC_RR(_state.d, _state.e); break;
			case 0x14: INC_R(_state.d); break;
			case 0x15: DEC_R(_state.d); break;
			case 0x16: LD_R_N(_state.d); break;
			case 0x17: RLA(); break;
			case 0x18: JR_N(); break;
			case 0x19: ADD_RR_RR(_state.h, _state.l, _state.d, _state.e); break;
			case 0x1A: LD_R_AA(_state.a, _state.d, _state.e); break;
			case 0x1B: DEC_RR(_state.d, _state.e); break;
			case 0x1C: INC_R(_state.e); break;
			case 0x1D: DEC_R(_state.e); break;
			case 0x1E: LD_R_N(_state.e); break;
			case 0x1F: RRA(); break;
			case 0x20: JR_N_IF(!GetZFlag()); break;
			case 0x21: LD_RR_NN(_state.h, _state.l); break;
			case 0x22: LD_AA_R_INC(_state.h, _state.l, _state.a); break;
			case 0x23: INC_RR(_state.h, _state.l); break;
			case 0x24: INC_R(_state.h); break;
			case 0x25: DEC_R(_state.h); break;
			case 0x26: LD_R_N(_state.h); break;
			case 0x27: DAA(); break;
			case 0x28: JR_N_IF(GetZFlag()); break;
			case 0x29: ADD_RR_RR(_state.h, _state.l, _state.h, _state.l); break;
			case 0x2A: LD_R_AA_INC(_state.a, _state.h, _state.l); break;
			case 0x2B: DEC_RR(_state.h, _state.l); break;
			case 0x2C: INC_R(_state.l); break;
			case 0x2D: DEC_R(_state.l); break;
			case 0x2E: LD_R_N(_state.l); break;
			case 0x2F: CPL(); break;
			case 0x30: JR_N_IF(!GetCFlag()); break;
			case 0x31: LD_SP_NN(); break;
			case 0x32: LD_AA_R_DEC(_state.h, _state.l, _state.a); break;
			case 0x33: INC_SP(); break;
			case 0x34: INC_AA(_state.h, _state.l); break;
			case 0x35: DEC_AA(_state.h, _state.l); break;
			case 0x36: LD_AA_N(_state.h, _state.l); break;
			case 0x37: SCF(); break;
			case 0x38: JR_N_IF(GetCFlag()); break;
			case 0x39: ADD_RR_SP(_state.h, _state.l); break;
			case 0x3A: LD_R_AA_DEC(_state.a, _state.h, _state.l); break;
			case 0x3B: DEC_SP(); break;
			case 0x3C: INC_R(_state.a); break;
			case 0x3D: DEC_R(_state.a); break;
			case 0x3E: LD_R_N(_state.a); break;
			case 0x3F: CCF(); break;
			case 0x40: LD_R_R(_state.b, _state.b); break;
			case 0x41: LD_R_R(_state.b, _state.c); break;
			case 0x42: LD_R_R(_state.b, _state.d); break;
			case 0x43: LD_R_R(_state.b, _state.e); break;
			case 0x44: LD_R_R(_state.b, _state.h); break;
			case 0x45: LD_R_R(_state.b, _state.l); break;
			case 0x46: LD_R_AA(_state.b, _state.h, _state.l); break;
			case 0x47: LD_R_R(_state.b, _state.a); break;
			case 0x48: LD_R_R(_state.c, _state.b); break;
			case 0x49: LD_R_R(_state.c, _state.c); break;
			case 0x4A: LD_R_R(_state.c, _state.d); break;
			case 0x4B: LD_R_R(_state.c, _state.e); break;
			case 0x4C: LD_R_R(_state.c, _state.h); break;
			case 0x4D: LD_R_R(_state.c, _state.l); break;
			case 0x4E: LD_R_AA(_state.c, _state.h, _state.l); break;
			case 0x4F: LD_R_R(_state.c, _state.a); break;
			case 0x50: LD_R_R(_state.d, _state.b); break;
			case 0x51: LD_R_R(_state.d, _state.c); break;
			case 0x52: LD_R_R(_state.d, _state.d); break;
			case 0x53: LD_R_R(_state.d, _state.e); break;
			case 0x54: LD_R_R(_state.d, _state.h); break;
			case 0x55: LD_R_R(_state.d, _state.l); break;
			case 0x56: LD_R_AA(_state.d, _state.h, _state.l); break;
			case 0x57: LD_R_R(_state.d, _state.a); break;
			case 0x58: LD_R_R(_state.e, _state.b); break;
			case 0x59: LD_R_R(_state.e, _state.c); break;
			case 0x5A: LD_R_R(_state.e, _state.d); break;
			case 0x5B: LD_R_R(_state.e, _state.e); break;
			case 0x5C: LD_R_R(_state.e, _state.h); break;
			case 0x5D: LD_R_R(_state.e, _state.l); break;
			case 0x5E: LD_R_AA(_state.e, _state.h, _state.l); break;
			case 0x5F: LD_R_R(_state.e, _state.a); break;
			case 0x60: LD_R_R(_state.h, _state.b); break;
			case 0x61: LD_R_R(_state.h, _state.c); break;
			case 0x62: LD_R_R(_state.h, _state.d); break;
			case 0x63: LD_R_R(_state.h, _state.e); break;
			case 0x64: LD_R_R(_state.h, _state.h); break;
			case 0x65: LD_R_R(_state.h, _state.l); break;
			case 0x66: LD_R_AA(_state.h, _state.h, _state.l); break;
			case 0x67: LD_R_R(_state.h, _state.a); break;
			case 0x68: LD_R_R(_state.l, _state.b); break;
			case 0x69: LD_R_R(_state.l, _state.c); break;
			case 0x6A: LD_R_R(_state.l, _state.d); break;
			case 0x6B: LD_R_R(_state.l, _state.e); break;
			case 0x6C: LD_R_R(_state.l, _state.h); break;
			case 0x6D: LD_R_R(_state.l, _state.l); break;
			case 0x6E: LD_R_AA(_state.l, _state.h, _state.l); break;
			case 0x6F: LD_R_R(_state.l, _state.a); break;
			case 0x70: LD_AA_R(_state.h, _state.l, _state.b); break;
			case 0x71: LD_AA_R(_state.h, _state.l, _state.c); break;
			case 0x72: LD_AA_R(_state.h, _state.l, _state.d); break;
			case 0x73: LD_AA_R(_state.h, _state.l, _state.e); break;
			case 0x74: LD_AA_R(_state.h, _state.l, _state.h); break;
			case 0x75: LD_AA_R(_state.h, _state.l, _state.l); break;
			case 0x76: HALT(); break;
			case 0x77: LD_AA_R(_state.h, _state.l, _state.a); break;
			case 0x78: LD_R_R(_state.a, _state.b); break;
			case 0x79: LD_R_R(_state.a, _state.c); break;
			case 0x7A: LD_R_R(_state.a, _state.d); break;
			case 0x7B: LD_R_R(_state.a, _state.e); break;
			case 0x7C: LD_R_R(_state.a, _state.h); break;
			case 0x7D: LD_R_R(_state.a, _state.l); break;
			case 0x7E: LD_R_AA(_state.a, _state.h, _state.l); break;
			case 0x7F: LD_R_R(_state.a, _state.a); break;
			case 0x80: ADD_R_R(_state.a, _state.b); break;
			case 0x81: ADD_R_R(_state.a, _state.c); break;
			case 0x82: ADD_R_R(_state.a, _state.d); break;
			case 0x83: ADD_R_R(_state.a, _state.e); break;
			case 0x84: ADD_R_R(_state.a, _state.h); break;
			case 0x85: ADD_R_R(_state.a, _state.l); break;
			case 0x86: ADD_R_AA(_state.a, _state.h, _state.l); break;
			case 0x87: ADD_R_R(_state.a, _state.a); break;
			case 0x88: ADC_R_R(_state.a, _state.b); break;
			case 0x89: ADC_R_R(_state.a, _state.c); break;
			case 0x8A: ADC_R_R(_state.a, _state.d); break;
			case 0x8B: ADC_R_R(_state.a, _state.e); break;
			case 0x8C: ADC_R_R(_state.a, _state.h); break;
			case 0x8D: ADC_R_R(_state.a, _state.l); break;
			case 0x8E: ADC_R_AA(_state.a, _state.h, _state.l); break;
			case 0x8F: ADC_R_R(_state.a, _state.a); break;
			case 0x90: SUB_R_R(_state.a, _state.b); break;
			case 0x91: SUB_R_R(_state.a, _state.c); break;
			case 0x92: SUB_R_R(_state.a, _state.d); break;
			case 0x93: SUB_R_R(_state.a, _state.e); break;
			case 0x94: SUB_R_R(_state.a, _state.h); break;
			case 0x95: SUB_R_R(_state.a, _state.l); break;
			case 0x96: SUB_R_AA(_state.a, _state.h, _state.l); break;
			case 0x97: SUB_R_R(_state.a, _state.a); break;
			case 0x98: SBC_R_R(_state.a, _state.b); break;
			case 0x99: SBC_R_R(_state.a, _state.c); break;
			case 0x9A: SBC_R_R(_state.a, _state.d); break;
			case 0x9B: SBC_R_R(_state.a, _state.e); break;
			case 0x9C: SBC_R_R(_state.a, _state.h); break;
			case 0x9D: SBC_R_R(_state.a, _state.l); break;
			case 0x9E: SBC_R_AA(_state.a, _state.h, _state.l); break;
			case 0x9F: SBC_R_R(_state.a, _state.a); break;
			case 0xA0: AND_R_R(_state.a, _state.b); break;
			case 0xA1: AND_R_R(_state.a, _state.c); break;
			case 0xA2: AND_R_R(_state.a, _state.d); break;
			case 0xA3: AND_R_R(_state.a, _state.e); break;
			case 0xA4: AND_R_R(_state.a, _state.h); break;
			case 0xA5: AND_R_R(_state.a, _state.l); break;
			case 0xA6: AND_R_AA(_state.a, _state.h, _state.l); break;
			case 0xA7: AND_R_R(_state.a, _state.a); break;
			case 0xA8: XOR_R_R(_state.a, _state.b); break;
			case 0xA9: XOR_R_R(_state.a, _state.c); break;
			case 0xAA: XOR_R_R(_state.a, _state.d); break;
			case 0xAB: XOR_R_R(_state.a, _state.e); break;
			case 0xAC: XOR_R_R(_state.a, _state.h); break;
			case 0xAD: XOR_R_R(_state.a, _state.l); break;
			case 0xAE: XOR_R_AA(_state.a, _state.h, _state.l); break;
			case 0xAF: XOR_R_R(_state.a, _state.a); break;
			case 0xB0: OR_R_R(_state.a, _state.b); break;
			case 0xB1: OR_R_R(_state.a, _state.c); break;
			case 0xB2: OR_R_R(_state.a, _state.d); break;
			case 0xB3: OR_R_R(_state.a, _state.e); break;
			case 0xB4: OR_R_R(_state.a, _state.h); break;
			case 0xB5: OR_R_R(_state.a, _state.l); break;
			case 0xB6: OR_R_AA(_state.a, _state.h, _state.l); break;
			case 0xB7: OR_R_R(_state.a, _state.a); break;
			case 0xB8: CP_R_R(_state.a, _state.b); break;
			case 0xB9: CP_R_R(_state.a, _state.c); break;
			case 0xBA: CP_R_R(_state.a, _state.d); break;
			case 0xBB: CP_R_R(_state.a, _state.e); break;
			case 0xBC: CP_R_R(_state.a, _state.h); break;
			case 0xBD: CP_R_R(_state.a, _state.l); break;
			case 0xBE: CP_R_AA(_state.a, _state.h, _state.l); break;
			case 0xBF: CP_R_R(_state.a, _state.a); break;
			case 0xC0: RET_IF(!GetZFlag()); break;
			case 0xC1: POP_RR(_state.b, _state.c); break;
			case 0xC2: JP_NN_IF(!GetZFlag()); break;
			case 0xC3: JP_NN(); break;
			case 0xC4: CALL_NN_IF(!GetZFlag()); break;
			case 0xC5: PUSH_RR(_state.b, _state.c); break;
			case 0xC6: ADD_R_N(_state.a); break;
			case 0xC7: RST(0x00); break;
			case 0xC8: RET_IF(GetZFlag()); break;
			case 0xC9: RET(); break;
			case 0xCA: JP_NN_IF(GetZFlag()); break;
			case 0xCB:
				FetchInstruction(InstructionTable::CB);
				
				switch (GetOpCode(InstructionTable::CB))
				{
					case 0x00: RLC_R(_state.b); break;
					case 0x01: RLC_R(_state.c); break;
					case 0x02: RLC_R(_state.d); break;
					case 0x03: RLC_R(_state.e); break;
					case 0x04: RLC_R(_state.h); break;
					case 0x05: RLC_R(_state.l); break;
					case 0x06: RLC_AA(_state.h, _state.l); break;
					case 0x07: RLC_R(_state.a); break;
					case 0x08: RRC_R(_state.b); break;
					case 0x09: RRC_R(_state.c); break;
					case 0x0A: RRC_R(_state.d); break;
					case 0x0B: RRC_R(_state.e); break;
					case 0x0C: RRC_R(_state.h); break;
					case 0x0D: RRC_R(_state.l); break;
					case 0x0E: RRC_AA(_state.h, _state.l); break;
					case 0x0F: RRC_R(_state.a); break;
					case 0x10: RL_R(_state.b); break;
					case 0x11: RL_R(_state.c); break;
					case 0x12: RL_R(_state.d); break;
					case 0x13: RL_R(_state.e); break;
					case 0x14: RL_R(_state.h); break;
					case 0x15: RL_R(_state.l); break;
					case 0x16: RL_AA(_state.h, _state.l); break;
					case 0x17: RL_R(_state.a); break;
					case 0x18: RR_R(_state.b); break;
					case 0x19: RR_R(_state.c); break;
					case 0x1A: RR_R(_state.d); break;
					case 0x1B: RR_R(_state.e); break;
					case 0x1C: RR_R(_state.h); break;
					case 0x1D: RR_R(_state.l); break;
					case 0x1E: RR_AA(_state.h, _state.l); break;
					case 0x1F: RR_R(_state.a); break;
					case 0x20: SLA_R(_state.b); break;
					case 0x21: SLA_R(_state.c); break;
					case 0x22: SLA_R(_state.d); break;
					case 0x23: SLA_R(_state.e); break;
					case 0x24: SLA_R(_state.h); break;
					case 0x25: SLA_R(_state.l); break;
					case 0x26: SLA_AA(_state.h, _state.l); break;
					case 0x27: SLA_R(_state.a); break;
					case 0x28: SRA_R(_state.b); break;
					case 0x29: SRA_R(_state.c); break;
					case 0x2A: SRA_R(_state.d); break;
					case 0x2B: SRA_R(_state.e); break;
					case 0x2C: SRA_R(_state.h); break;
					case 0x2D: SRA_R(_state.l); break;
					case 0x2E: SRA_AA(_state.h, _state.l); break;
					case 0x2F: SRA_R(_state.a); break;
					case 0x30: SWAP_R(_state.b); break;
					case 0x31: SWAP_R(_state.c); break;
					case 0x32: SWAP_R(_state.d); break;
					case 0x33: SWAP_R(_state.e); break;
					case 0x34: SWAP_R(_state.h); break;
					case 0x35: SWAP_R(_state.l); break;
					case 0x36: SWAP_AA(_state.h, _state.l); break;
					case 0x37: SWAP_R(_state.a); break;
					case 0x38: SRL_R(_state.b); break;
					case 0x39: SRL_R(_state.c); break;
					case 0x3A: SRL_R(_state.d); break;
					case 0x3B: SRL_R(_state.e); break;
					case 0x3C: SRL_R(_state.h); break;
					case 0x3D: SRL_R(_state.l); break;
					case 0x3E: SRL_AA(_state.h, _state.l); break;
					case 0x3F: SRL_R(_state.a); break;
					case 0x40: BIT_X_R(0, _state.b); break;
					case 0x41: BIT_X_R(0, _state.c); break;
					case 0x42: BIT_X_R(0, _state.d); break;
					case 0x43: BIT_X_R(0, _state.e); break;
					case 0x44: BIT_X_R(0, _state.h); break;
					case 0x45: BIT_X_R(0, _state.l); break;
					case 0x46: BIT_X_AA(0, _state.h, _state.l); break;
					case 0x47: BIT_X_R(0, _state.a); break;
					case 0x48: BIT_X_R(1, _state.b); break;
					case 0x49: BIT_X_R(1, _state.c); break;
					case 0x4A: BIT_X_R(1, _state.d); break;
					case 0x4B: BIT_X_R(1, _state.e); break;
					case 0x4C: BIT_X_R(1, _state.h); break;
					case 0x4D: BIT_X_R(1, _state.l); break;
					case 0x4E: BIT_X_AA(1, _state.h, _state.l); break;
					case 0x4F: BIT_X_R(1, _state.a); break;
					case 0x50: BIT_X_R(2, _state.b); break;
					case 0x51: BIT_X_R(2, _state.c); break;
					case 0x52: BIT_X_R(2, _state.d); break;
					case 0x53: BIT_X_R(2, _state.e); break;
					case 0x54: BIT_X_R(2, _state.h); break;
					case 0x55: BIT_X_R(2, _state.l); break;
					case 0x56: BIT_X_AA(2, _state.h, _state.l); break;
					case 0x57: BIT_X_R(2, _state.a); break;
					case 0x58: BIT_X_R(3, _state.b); break;
					case 0x59: BIT_X_R(3, _state.c); break;
					case 0x5A: BIT_X_R(3, _state.d); break;
					case 0x5B: BIT_X_R(3, _state.e); break;
					case 0x5C: BIT_X_R(3, _state.h); break;
					case 0x5D: BIT_X_R(3, _state.l); break;
					case 0x5E: BIT_X_AA(3, _state.h, _state.l); break;
					case 0x5F: BIT_X_R(3, _state.a); break;
					case 0x60: BIT_X_R(4, _state.b); break;
					case 0x61: BIT_X_R(4, _state.c); break;
					case 0x62: BIT_X_R(4, _state.d); break;
					case 0x63: BIT_X_R(4, _state.e); break;
					case 0x64: BIT_X_R(4, _state.h); break;
					case 0x65: BIT_X_R(4, _state.l); break;
					case 0x66: BIT_X_AA(4, _state.h, _state.l); break;
					case 0x67: BIT_X_R(4, _state.a); break;
					case 0x68: BIT_X_R(5, _state.b); break;
					case 0x69: BIT_X_R(5, _state.c); break;
					case 0x6A: BIT_X_R(5, _state.d); break;
					case 0x6B: BIT_X_R(5, _state.e); break;
					case 0x6C: BIT_X_R(5, _state.h); break;
					case 0x6D: BIT_X_R(5, _state.l); break;
					case 0x6E: BIT_X_AA(5, _state.h, _state.l); break;
					case 0x6F: BIT_X_R(5, _state.a); break;
					case 0x70: BIT_X_R(6, _state.b); break;
					case 0x71: BIT_X_R(6, _state.c); break;
					case 0x72: BIT_X_R(6, _state.d); break;
					case 0x73: BIT_X_R(6, _state.e); break;
					case 0x74: BIT_X_R(6, _state.h); break;
					case 0x75: BIT_X_R(6, _state.l); break;
					case 0x76: BIT_X_AA(6, _state.h, _state.l); break;
					case 0x77: BIT_X_R(6, _state.a); break;
					case 0x78: BIT_X_R(7, _state.b); break;
					case 0x79: BIT_X_R(7, _state.c); break;
					case 0x7A: BIT_X_R(7, _state.d); break;
					case 0x7B: BIT_X_R(7, _state.e); break;
					case 0x7C: BIT_X_R(7, _state.h); break;
					case 0x7D: BIT_X_R(7, _state.l); break;
					case 0x7E: BIT_X_AA(7, _state.h, _state.l); break;
					case 0x7F: BIT_X_R(7, _state.a); break;
					case 0x80: RES_X_R(0, _state.b); break;
					case 0x81: RES_X_R(0, _state.c); break;
					case 0x82: RES_X_R(0, _state.d); break;
					case 0x83: RES_X_R(0, _state.e); break;
					case 0x84: RES_X_R(0, _state.h); break;
					case 0x85: RES_X_R(0, _state.l); break;
					case 0x86: RES_X_AA(0, _state.h, _state.l); break;
					case 0x87: RES_X_R(0, _state.a); break;
					case 0x88: RES_X_R(1, _state.b); break;
					case 0x89: RES_X_R(1, _state.c); break;
					case 0x8A: RES_X_R(1, _state.d); break;
					case 0x8B: RES_X_R(1, _state.e); break;
					case 0x8C: RES_X_R(1, _state.h); break;
					case 0x8D: RES_X_R(1, _state.l); break;
					case 0x8E: RES_X_AA(1, _state.h, _state.l); break;
					case 0x8F: RES_X_R(1, _state.a); break;
					case 0x90: RES_X_R(2, _state.b); break;
					case 0x91: RES_X_R(2, _state.c); break;
					case 0x92: RES_X_R(2, _state.d); break;
					case 0x93: RES_X_R(2, _state.e); break;
					case 0x94: RES_X_R(2, _state.h); break;
					case 0x95: RES_X_R(2, _state.l); break;
					case 0x96: RES_X_AA(2, _state.h, _state.l); break;
					case 0x97: RES_X_R(2, _state.a); break;
					case 0x98: RES_X_R(3, _state.b); break;
					case 0x99: RES_X_R(3, _state.c); break;
					case 0x9A: RES_X_R(3, _state.d); break;
					case 0x9B: RES_X_R(3, _state.e); break;
					case 0x9C: RES_X_R(3, _state.h); break;
					case 0x9D: RES_X_R(3, _state.l); break;
					case 0x9E: RES_X_AA(3, _state.h, _state.l); break;
					case 0x9F: RES_X_R(3, _state.a); break;
					case 0xA0: RES_X_R(4, _state.b); break;
					case 0xA1: RES_X_R(4, _state.c); break;
					case 0xA2: RES_X_R(4, _state.d); break;
					case 0xA3: RES_X_R(4, _state.e); break;
					case 0xA4: RES_X_R(4, _state.h); break;
					case 0xA5: RES_X_R(4, _state.l); break;
					case 0xA6: RES_X_AA(4, _state.h, _state.l); break;
					case 0xA7: RES_X_R(4, _state.a); break;
					case 0xA8: RES_X_R(5, _state.b); break;
					case 0xA9: RES_X_R(5, _state.c); break;
					case 0xAA: RES_X_R(5, _state.d); break;
					case 0xAB: RES_X_R(5, _state.e); break;
					case 0xAC: RES_X_R(5, _state.h); break;
					case 0xAD: RES_X_R(5, _state.l); break;
					case 0xAE: RES_X_AA(5, _state.h, _state.l); break;
					case 0xAF: RES_X_R(5, _state.a); break;
					case 0xB0: RES_X_R(6, _state.b); break;
					case 0xB1: RES_X_R(6, _state.c); break;
					case 0xB2: RES_X_R(6, _state.d); break;
					case 0xB3: RES_X_R(6, _state.e); break;
					case 0xB4: RES_X_R(6, _state.h); break;
					case 0xB5: RES_X_R(6, _state.l); break;
					case 0xB6: RES_X_AA(6, _state.h, _state.l); break;
					case 0xB7: RES_X_R(6, _state.a); break;
					case 0xB8: RES_X_R(7, _state.b); break;
					case 0xB9: RES_X_R(7, _state.c); break;
					case 0xBA: RES_X_R(7, _state.d); break;
					case 0xBB: RES_X_R(7, _state.e); break;
					case 0xBC: RES_X_R(7, _state.h); break;
					case 0xBD: RES_X_R(7, _state.l); break;
					case 0xBE: RES_X_AA(7, _state.h, _state.l); break;
					case 0xBF: RES_X_R(7, _state.a); break;
					case 0xC0: SET_X_R(0, _state.b); break;
					case 0xC1: SET_X_R(0, _state.c); break;
					case 0xC2: SET_X_R(0, _state.d); break;
					case 0xC3: SET_X_R(0, _state.e); break;
					case 0xC4: SET_X_R(0, _state.h); break;
					case 0xC5: SET_X_R(0, _state.l); break;
					case 0xC6: SET_X_AA(0, _state.h, _state.l); break;
					case 0xC7: SET_X_R(0, _state.a); break;
					case 0xC8: SET_X_R(1, _state.b); break;
					case 0xC9: SET_X_R(1, _state.c); break;
					case 0xCA: SET_X_R(1, _state.d); break;
					case 0xCB: SET_X_R(1, _state.e); break;
					case 0xCC: SET_X_R(1, _state.h); break;
					case 0xCD: SET_X_R(1, _state.l); break;
					case 0xCE: SET_X_AA(1, _state.h, _state.l); break;
					case 0xCF: SET_X_R(1, _state.a); break;
					case 0xD0: SET_X_R(2, _state.b); break;
					case 0xD1: SET_X_R(2, _state.c); break;
					case 0xD2: SET_X_R(2, _state.d); break;
					case 0xD3: SET_X_R(2, _state.e); break;
					case 0xD4: SET_X_R(2, _state.h); break;
					case 0xD5: SET_X_R(2, _state.l); break;
					case 0xD6: SET_X_AA(2, _state.h, _state.l); break;
					case 0xD7: SET_X_R(2, _state.a); break;
					case 0xD8: SET_X_R(3, _state.b); break;
					case 0xD9: SET_X_R(3, _state.c); break;
					case 0xDA: SET_X_R(3, _state.d); break;
					case 0xDB: SET_X_R(3, _state.e); break;
					case 0xDC: SET_X_R(3, _state.h); break;
					case 0xDD: SET_X_R(3, _state.l); break;
					case 0xDE: SET_X_AA(3, _state.h, _state.l); break;
					case 0xDF: SET_X_R(3, _state.a); break;
					case 0xE0: SET_X_R(4, _state.b); break;
					case 0xE1: SET_X_R(4, _state.c); break;
					case 0xE2: SET_X_R(4, _state.d); break;
					case 0xE3: SET_X_R(4, _state.e); break;
					case 0xE4: SET_X_R(4, _state.h); break;
					case 0xE5: SET_X_R(4, _state.l); break;
					case 0xE6: SET_X_AA(4, _state.h, _state.l); break;
					case 0xE7: SET_X_R(4, _state.a); break;
					case 0xE8: SET_X_R(5, _state.b); break;
					case 0xE9: SET_X_R(5, _state.c); break;
					case 0xEA: SET_X_R(5, _state.d); break;
					case 0xEB: SET_X_R(5, _state.e); break;
					case 0xEC: SET_X_R(5, _state.h); break;
					case 0xED: SET_X_R(5, _state.l); break;
					case 0xEE: SET_X_AA(5, _state.h, _state.l); break;
					case 0xEF: SET_X_R(5, _state.a); break;
					case 0xF0: SET_X_R(6, _state.b); break;
					case 0xF1: SET_X_R(6, _state.c); break;
					case 0xF2: SET_X_R(6, _state.d); break;
					case 0xF3: SET_X_R(6, _state.e); break;
					case 0xF4: SET_X_R(6, _state.h); break;
					case 0xF5: SET_X_R(6, _state.l); break;
					case 0xF6: SET_X_AA(6, _state.h, _state.l); break;
					case 0xF7: SET_X_R(6, _state.a); break;
					case 0xF8: SET_X_R(7, _state.b); break;
					case 0xF9: SET_X_R(7, _state.c); break;
					case 0xFA: SET_X_R(7, _state.d); break;
					case 0xFB: SET_X_R(7, _state.e); break;
					case 0xFC: SET_X_R(7, _state.h); break;
					case 0xFD: SET_X_R(7, _state.l); break;
					case 0xFE: SET_X_AA(7, _state.h, _state.l); break;
					case 0xFF: SET_X_R(7, _state.a); break;
					default: break;
				}
				
				break;
			
			case 0xCC: CALL_NN_IF(GetZFlag()); break;
			case 0xCD: CALL_NN(); break;
			case 0xCE: ADC_R_N(_state.a); break;
			case 0xCF: RST(0x08); break;
			case 0xD0: RET_IF(!GetCFlag()); break;
			case 0xD1: POP_RR(_state.d, _state.e); break;
			case 0xD2: JP_NN_IF(!GetCFlag()); break;
			case 0xD3: NOP(); /* NOT USED */ break;
			case 0xD4: CALL_NN_IF(!GetCFlag()); break;
			case 0xD5: PUSH_RR(_state.d, _state.e); break;
			case 0xD6: SUB_R_N(_state.a); break;
			case 0xD7: RST(0x10); break;
			case 0xD8: RET_IF(GetCFlag()); break;
			case 0xD9: RETI(); break;
			case 0xDA: JP_NN_IF(GetCFlag()); break;
			case 0xDB: NOP(); /* NOT USED */ break;
			case 0xDC: CALL_NN_IF(GetCFlag()); break;
			case 0xDD: NOP(); /* NOT USED */ break;
			case 0xDE: SBC_R_N(_state.a); break;
			case 0xDF: RST(0x18); break;
			case 0xE0: LDH_A_R(GetOpLow(InstructionTable::DEFAULT), _state.a); break;
			case 0xE1: POP_RR(_state.h, _state.l); break;
			case 0xE2: LD_A_R(_state.c, _state.a); break;
			case 0xE3: NOP(); /* NOT USED */ break;
			case 0xE4: NOP(); /* NOT USED */ break;
			case 0xE5: PUSH_RR(_state.h, _state.l); break;
			case 0xE6: AND_R_N(_state.a); break;
			case 0xE7: RST(0x20); break;
			case 0xE8: ADD_SP_N(); break;
			case 0xE9: JP_RR(_state.h, _state.l); break;
			case 0xEA: LD_AA_R(GetOpHigh(InstructionTable::DEFAULT), GetOpLow(InstructionTable::DEFAULT), _state.a); break;
			case 0xEB: NOP(); /* NOT USED */ break;
			case 0xEC: NOP(); /* NOT USED */ break;
			case 0xED: NOP(); /* NOT USED */ break;
			case 0xEE: XOR_R_N(_state.a); break;
			case 0xEF: RST(0x28); break;
			case 0xF0: LDH_R_A(_state.a, GetOpLow(InstructionTable::DEFAULT)); break;
			case 0xF1: POP_AF(); break;
			case 0xF2: LD_R_A(_state.a, _state.c); break;
			case 0xF3: DI(); break;
			case 0xF4: NOP(); /* NOT USED */ break;
			case 0xF5: PUSH_AF(); break;
			case 0xF6: OR_R_N(_state.a); break;
			case 0xF7: RST(0x30); break;
			case 0xF8: LD_HL_SP_N(); break;
			case 0xF9: LD_SP_HL(); break;
			case 0xFA: LD_R_AA(_state.a, GetOpHigh(InstructionTable::DEFAULT), GetOpLow(InstructionTable::DEFAULT)); break;
			case 0xFB: EI(); break;
			case 0xFC: NOP(); /* NOT USED */ break;
			case 0xFD: NOP(); /* NOT USED */ break;
			case 0xFE: CP_R_N(_state.a); break;
			case 0xFF: RST(0x38); break;
			default: break;
		}
	}
}

void Processor::ExecuteInterrupt()
{
	if (_state.interruptsEnabled || _state.halted)
	{
		int verticalBlankInterruptToBePerformed = GetBit(_bus->ReadByte(INTERRUPT_ENABLE_ADDRESS),
		                                                                VERTICAL_BLANK_INTERRUPT_ENABLE_BIT) &&
		                                          GetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
		                                                                VERTICAL_BLANK_INTERRUPT_REQUEST_BIT);
		
		int lcdStatusInterruptToBePerformed = GetBit(_bus->ReadByte(INTERRUPT_ENABLE_ADDRESS),
		                                                            LCD_STATUS_INTERRUPT_ENABLE_BIT) &&
		                                      GetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
		                                                            LCD_STATUS_INTERRUPT_REQUEST_BIT);
		
		int timerInterruptToBePerformed = GetBit(_bus->ReadByte(INTERRUPT_ENABLE_ADDRESS),
		                                                        TIMER_INTERRUPT_ENABLE_BIT) &&
		                                  GetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
		                                                        TIMER_INTERRUPT_REQUEST_BIT);
		
		int serialInterruptToBePerformed = GetBit(_bus->ReadByte(INTERRUPT_ENABLE_ADDRESS),
		                                                         SERIAL_INTERRUPT_ENABLE_BIT) &&
		                                   GetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
		                                                         SERIAL_INTERRUPT_REQUEST_BIT);
		
		int joypadInterruptToBePerformed = GetBit(_bus->ReadByte(INTERRUPT_ENABLE_ADDRESS),
		                                                         JOYPAD_INTERRUPT_ENABLE_BIT) &&
		                                   GetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
		                                                         JOYPAD_INTERRUPT_REQUEST_BIT);
		
		if (verticalBlankInterruptToBePerformed ||
		    lcdStatusInterruptToBePerformed ||
		    timerInterruptToBePerformed ||
		    serialInterruptToBePerformed ||
		    joypadInterruptToBePerformed)
		{
			_state.sp -= 2;
			
			_bus->WriteByte(_state.sp, GetLow(_state.pc));
			_bus->WriteByte(_state.sp + 1, GetHigh(_state.pc));
			
			if (verticalBlankInterruptToBePerformed)
			{
				_state.pc = VERTICAL_BLANK_INTERRUPT_VECTOR;
				
				int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
				                                                        VERTICAL_BLANK_INTERRUPT_REQUEST_BIT,
				                                                        GBC_FALSE);
				
				_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
			}
			else if (lcdStatusInterruptToBePerformed)
			{
				_state.pc = LCD_STATUS_INTERRUPT_VECTOR;
				
				int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
				                                                        LCD_STATUS_INTERRUPT_REQUEST_BIT,
				                                                        GBC_FALSE);
				
				_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
			}
			else if (timerInterruptToBePerformed)
			{
				_state.pc = TIMER_INTERRUPT_VECTOR;
				
				int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
				                                                        TIMER_INTERRUPT_REQUEST_BIT,
				                                                        GBC_FALSE);
				
				_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
			}
			else if (serialInterruptToBePerformed)
			{
				_state.pc = SERIAL_INTERRUPT_VECTOR;
				
				int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
				                                                        SERIAL_INTERRUPT_REQUEST_BIT,
				                                                        GBC_FALSE);
				
				_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
			}
			else if (joypadInterruptToBePerformed)
			{
				_state.pc = JOYPAD_INTERRUPT_VECTOR;
				
				int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
				                                                        JOYPAD_INTERRUPT_REQUEST_BIT,
				                                                        GBC_FALSE);
				
				_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
			}
			
			_state.interruptsEnabled = GBC_FALSE;
			_state.stopped = GBC_FALSE;
			_state.halted = GBC_FALSE;
			
			_state.ticks += 16;
		}
	}
}

void Processor::PowerUp()
{
	_state.a = 0x01;
	//_state.a = 0x11; //test
	_state.f = 0xB0;
	_state.b = 0x00;
	_state.c = 0x13;
	_state.d = 0x00;
	_state.e = 0xD8;
	_state.h = 0x01;
	_state.l = 0x4D;
	_state.pc = 0x0100;
	_state.sp = 0xFFFE;
	
	_state.interruptsEnabled = GBC_TRUE;
	_state.halted = GBC_FALSE;
	_state.stopped = GBC_FALSE;
	
	_bus->WriteByte(0xFF05, 0x00); // TIMA
	_bus->WriteByte(0xFF06, 0x00); // TMA
	_bus->WriteByte(0xFF07, 0x00); // TAC
	_bus->WriteByte(0xFF10, 0x80); // NR10
	_bus->WriteByte(0xFF11, 0xBF); // NR11
	_bus->WriteByte(0xFF12, 0xF3); // NR12
	_bus->WriteByte(0xFF14, 0xBF); // NR14
	_bus->WriteByte(0xFF16, 0x3F); // NR21
	_bus->WriteByte(0xFF17, 0x00); // NR22
	_bus->WriteByte(0xFF19, 0xBF); // NR24
	_bus->WriteByte(0xFF1A, 0x7F); // NR30
	_bus->WriteByte(0xFF1B, 0xFF); // NR31
	_bus->WriteByte(0xFF1C, 0x9F); // NR32
	_bus->WriteByte(0xFF1E, 0xBF); // NR33
	_bus->WriteByte(0xFF20, 0xFF); // NR41
	_bus->WriteByte(0xFF21, 0x00); // NR42
	_bus->WriteByte(0xFF22, 0x00); // NR43
	_bus->WriteByte(0xFF23, 0xBF); // NR30
	_bus->WriteByte(0xFF24, 0x77); // NR50
	_bus->WriteByte(0xFF25, 0xF3); // NR51
	_bus->WriteByte(0xFF26, 0xF1); // NR52
	_bus->WriteByte(0xFF40, 0x91); // LCDC
	_bus->WriteByte(0xFF42, 0x00); // SCY
	_bus->WriteByte(0xFF43, 0x00); // SCX
	_bus->WriteByte(0xFF45, 0x00); // LYC
	_bus->WriteByte(0xFF47, 0xFC); // BGP
	_bus->WriteByte(0xFF48, 0xFF); // OBP0
	_bus->WriteByte(0xFF49, 0xFF); // OBP1
	_bus->WriteByte(0xFF4A, 0x00); // WY
	_bus->WriteByte(0xFF4B, 0x00); // WX
	_bus->WriteByte(0xFFFF, 0x00); // IE
}

void Processor::SignalVBlankInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        VERTICAL_BLANK_INTERRUPT_REQUEST_BIT,
	                                                        GBC_TRUE);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void Processor::SignalLCDStatusInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        LCD_STATUS_INTERRUPT_REQUEST_BIT,
	                                                        GBC_TRUE);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void Processor::SignalTimerInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        TIMER_INTERRUPT_REQUEST_BIT,
	                                                        GBC_TRUE);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void Processor::SignalSerialInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        SERIAL_INTERRUPT_REQUEST_BIT,
	                                                        GBC_TRUE);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void Processor::SignalJoypadInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        JOYPAD_INTERRUPT_REQUEST_BIT,
	                                                        GBC_TRUE);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void Processor::SetState(State state)
{
	_state = state;
}

State Processor::GetState()
{
	return _state;
}

int Processor::GetCounter()
{
	return _counter;
}

// helper methods
inline void Processor::FetchInstruction(InstructionTable instructionTable)
{
	if (instructionTable == InstructionTable::DEFAULT)
	{
		_currentOpCodes[0] = _bus->ReadByte(_state.pc);
		_currentOpLows[0] = _bus->ReadByte(_state.pc + 1);
		_currentOpHighs[0] = _bus->ReadByte(_state.pc + 2);
	}
	else
	{
		_currentOpCodes[1] = _bus->ReadByte(_state.pc + 1);
		_currentOpLows[1] = _bus->ReadByte(_state.pc + 2);
		_currentOpHighs[1] = _bus->ReadByte(_state.pc + 3);
	}
}

void Processor::UpdatePC(InstructionTable instructionTable)
{
	int tableNumber = GetEnumValue(instructionTable);
	
	_state.pc += LookUpTables::INSTRUCTION_LENGTHS[tableNumber][_currentOpCodes[tableNumber]];
}

void Processor::UpdateTicks(InstructionTable instructionTable)
{
	int tableNumber = GetEnumValue(instructionTable);
	
	_state.ticks += LookUpTables::INSTRUCTION_CYCLES[tableNumber][_currentOpCodes[tableNumber]];
}

void Processor::UpdateAdditionalTicks(InstructionTable instructionTable)
{
	int tableNumber = GetEnumValue(instructionTable);
	
	_state.ticks += LookUpTables::ADDITIONAL_INSTRUCTION_CYCLES[tableNumber][_currentOpCodes[tableNumber]];
}

void Processor::SetCFlag(int value)
{
	_state.f = SetBit(_state.f, CFLAG_BIT, value);
}

void Processor::SetHFlag(int value)
{
	_state.f = SetBit(_state.f, HFLAG_BIT, value);
}

void Processor::SetNFlag(int value)
{
	_state.f = SetBit(_state.f, NFLAG_BIT, value);
}

void Processor::SetZFlag(int value)
{
	_state.f = SetBit(_state.f, ZFLAG_BIT, value);
}

int Processor::GetCFlag()
{
	return GetBit(_state.f, CFLAG_BIT);
}

int Processor::GetHFlag()
{
	return GetBit(_state.f, HFLAG_BIT);
}

int Processor::GetNFlag()
{
	return GetBit(_state.f, NFLAG_BIT);
}

int Processor::GetZFlag()
{
	return GetBit(_state.f, ZFLAG_BIT);
}

int Processor::GetOpCode(InstructionTable instructionTable)
{
	return _currentOpCodes[GetEnumValue(instructionTable)];
}

int Processor::GetOpLow(InstructionTable instructionTable)
{
	return _currentOpLows[GetEnumValue(instructionTable)];
}

int Processor::GetOpHigh(InstructionTable instructionTable)
{
	return _currentOpHighs[GetEnumValue(instructionTable)];
}

int Processor::GetOpLength(InstructionTable instructionTable)
{
	int tableNumber = GetEnumValue(instructionTable);
	
	return LookUpTables::INSTRUCTION_LENGTHS[tableNumber][_currentOpCodes[tableNumber]];
}

// default instruction set

void Processor::NOP()
{
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::INC_RR(int &r1, int &r2)
{
	r2++;
	
	if (r2 > 0xFF)
	{
		r1++;
		r1 &= 0xFF;
		r2 &= 0xFF;
	}
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::INC_R(int &r)
{
	SetHFlag((r & 0x0F) == 0x0F);
	
	r++;
	r &= 0xFF;
	
	SetZFlag(r == 0x00);
	SetNFlag(GBC_FALSE);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::INC_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	SetHFlag((memory & 0x0F) == 0x0F);
	
	memory++;
	memory &= 0xFF;
	
	SetZFlag(memory == 0x00);
	SetNFlag(GBC_FALSE);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::INC_SP()
{
	_state.sp++;
	_state.sp &= 0xFFFF;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::DEC_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	SetHFlag((memory & 0x0F) == 0x00);
	
	memory--;
	memory &= 0xFF;
	
	SetZFlag(memory == 0x00);
	SetNFlag(GBC_TRUE);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::DEC_RR(int &r1, int &r2)
{
	r2--;
	
	if (r2 < 0x00)
	{
		r1--;
		r1 &= 0xFF;
		r2 &= 0xFF;
	}
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::DEC_R(int &r)
{
	SetHFlag((r & 0x0F) == 0x00);
	
	r--;
	r &= 0xFF;
	
	SetZFlag(r == 0x00);
	SetNFlag(GBC_TRUE);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::DEC_SP()
{
	_state.sp--;
	_state.sp &= 0xFFFF;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_RR_NN(int &r1, int &r2)
{
	r1 = GetOpHigh(InstructionTable::DEFAULT);
	r2 = GetOpLow(InstructionTable::DEFAULT);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_SP_NN()
{
	_state.sp = JoinBytes(GetOpHigh(InstructionTable::DEFAULT), GetOpLow(InstructionTable::DEFAULT));
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::RLCA()
{
	SetCFlag(GetBit(_state.a, 7));
	
	_state.a = ((_state.a << 1) & 0xFE) | (GetBit(_state.a, 7) ? 0x01 : 0x00);
	
	SetZFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_AA_SP(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	
	_bus->WriteByte(address, GetLow(_state.sp));
	_bus->WriteByte((address + 1) & 0xFFFF, GetHigh(_state.sp));
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::ADD_RR_RR(int &r11, int &r12, int r21, int r22)
{
	int r1Full = JoinBytes(r11, r12);
	int r2Full = JoinBytes(r21, r22);
	
	SetNFlag(GBC_FALSE);
	SetHFlag(((r1Full & 0x0FFF) + (r2Full & 0x0FFF)) > 0x0FFF);
	
	int result = (r1Full & 0xFFFF) + (r2Full & 0xFFFF);
	
	SetCFlag(result > 0xFFFF);
	
	r11 = GetHigh(result);
	r12 = GetLow(result);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::ADD_RR_SP(int &r1, int &r2)
{
	int rFull = JoinBytes(r1, r2);
	
	SetNFlag(GBC_FALSE);
	SetHFlag(((rFull & 0x0FFF) + (_state.sp & 0x0FFF)) > 0x0FFF);
	
	int result = (rFull + _state.sp);
	
	SetCFlag(result > 0xFFFF);
	
	result &= 0xFFFF;
	
	r1 = GetHigh(result);
	r2 = GetLow(result);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_R_AA(int &r, int a1, int a2)
{
	r = _bus->ReadByte(JoinBytes(a1, a2));
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_R_N(int &r)
{
	r = GetOpLow(InstructionTable::DEFAULT);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_AA_N(int a1, int a2)
{
	_bus->WriteByte(JoinBytes(a1, a2), GetOpLow(InstructionTable::DEFAULT));
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::RRCA()
{
	SetCFlag(GetBit(_state.a, 0));
	
	_state.a = ((_state.a >> 1) & 0x7F) | (GetBit(_state.a, 0) ? 0x80 : 0x00);
	
	SetZFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::STOP()
{
	_state.stopped = GBC_TRUE;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::RLA()
{
	int oldCFlag = GetCFlag();
	
	SetCFlag(((_state.a >> 7) & 0x01) != 0x00);
	
	_state.a = ((_state.a << 1) & 0xFF) | (oldCFlag ? 0x01 : 0x00);
	
	SetZFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::JR_N()
{
	_state.pc += GetSignedValue(GetOpLow(InstructionTable::DEFAULT));
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::RRA()
{
	int oldCFlag = GetCFlag();
	
	SetCFlag((_state.a & 0x01) != 0);
	
	_state.a = ((_state.a >> 1) & 0x7F) | (oldCFlag ? 0x80 : 0x00);
	
	SetZFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::JR_N_IF(int c)
{
	if (c)
	{
		_state.pc += GetSignedValue(GetOpLow(InstructionTable::DEFAULT));
		
		UpdateAdditionalTicks(InstructionTable::DEFAULT);
	}
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_AA_R_INC(int &a1, int &a2, int r)
{
	_bus->WriteByte(JoinBytes(a1, a2), r);
	
	a2++;
	
	if (a2 > 0xFF)
	{
		a1++;
		a1 &= 0xFF;
		a2 &= 0xFF;
	}
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_AA_R_DEC(int &a1, int &a2, int r)
{
	_bus->WriteByte(JoinBytes(a1, a2), r);
	
	a2--;
	
	if (a2 < 0x00)
	{
		a1--;
		a1 &= 0xFF;
		a2 &= 0xFF;
	}
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_R_AA_INC(int &r, int &a1, int &a2)
{
	r = _bus->ReadByte(JoinBytes(a1, a2));
	
	a2++;
	
	if (a2 > 0xFF)
	{
		a1++;
		a1 &= 0xFF;
		a2 &= 0xFF;
	}
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_R_AA_DEC(int &r, int &a1, int &a2)
{
	r = _bus->ReadByte(JoinBytes(a1, a2));
	
	a2--;
	
	if (a2 < 0x00)
	{
		a1--;
		a1 &= 0xFF;
		a2 &= 0xFF;
	}
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::DAA()
{
	if (!GetNFlag())
	{
		if (GetHFlag() || (_state.a & 0x0F) > 0x09)
		{
			_state.a += 0x06;
		}
		
		if (GetCFlag() || _state.a > 0x9F)
		{
			_state.a += 0x60;
		}
	}
	else
	{
		if (GetNFlag())
		{
			_state.a -= 0x06;
			_state.a &= 0xFF;
		}
		
		if (GetCFlag())
		{
			_state.a -= 0x60;
		}
	}
	
	SetHFlag(GBC_FALSE);
	SetZFlag(GBC_FALSE);
	
	if (_state.a > 0xFF)
	{
		SetCFlag(GBC_TRUE);
	}
	
	_state.a &= 0xFF;
	
	if (_state.a == 0x00)
	{
		SetZFlag(GBC_TRUE);
	}
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::CPL()
{
	_state.a = ~_state.a;
	_state.a &= 0xFF;
	
	SetNFlag(GBC_TRUE);
	SetHFlag(GBC_TRUE);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::SCF()
{
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetCFlag(GBC_TRUE);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::CCF()
{
	SetHFlag(GBC_FALSE);
	SetCFlag(!GetCFlag());
	SetNFlag(GBC_FALSE);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_R_R(int &r1, int r2)
{
	r1 = r2;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::HALT()
{
	_state.halted = GBC_TRUE;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::ADD_R_R(int &r1, int r2)
{
	SetNFlag(GBC_FALSE);
	SetHFlag(((r1 & 0x0F) + (r2 & 0x0F)) > 0x0F);
	
	r1 += r2;
	
	SetCFlag(r1 > 0xFF);
	
	r1 &= 0xFF;
	
	SetZFlag(r1 == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::ADD_R_AA(int &r, int a1, int a2)
{
	int memory = _bus->ReadByte(JoinBytes(a1, a2));
	
	SetNFlag(GBC_FALSE);
	SetHFlag(((r & 0x0F) + (memory & 0x0F)) > 0x0F);
	
	r += memory;
	
	SetCFlag(r > 0xFF);
	
	r &= 0xFF;
	
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::ADC_R_R(int &r1, int r2)
{
	int oldCFlag = GetCFlag();
	
	SetNFlag(GBC_FALSE);
	SetHFlag(((r1 & 0x0F) + (r2 & 0x0F) + (oldCFlag ? 0x01 : 0x00)) > 0x0F);
	
	r1 += r2 + (oldCFlag ? 0x01 : 0x00);
	
	SetCFlag(r1 > 0xFF);
	
	r1 &= 0xFF;
	
	SetZFlag(r1 == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::ADC_R_AA(int &r, int a1, int a2)
{
	int memory = _bus->ReadByte(JoinBytes(a1, a2));
	int oldCFlag = GetCFlag();
	
	SetNFlag(GBC_FALSE);
	SetHFlag(((r & 0x0F) + (memory & 0x0F) + (oldCFlag ? 0x01 : 0x00)) > 0x0F);
	
	r += memory + (oldCFlag ? 0x01 : 0x00);
	
	SetCFlag(r > 0xFF);
	
	r &= 0xFF;
	
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::SUB_R_R(int &r1, int r2)
{
	SetNFlag(GBC_TRUE);
	SetHFlag(((r1 & 0x0F) - (r2 & 0x0F)) < 0x00);
	
	r1-= r2;
	
	SetCFlag(r1 < 0x00);
	
	r1 &= 0xFF;
	
	SetZFlag(r1 == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::SUB_R_AA(int &r, int a1, int a2)
{
	int memory = _bus->ReadByte(JoinBytes(a1, a2));
	
	SetNFlag(GBC_TRUE);
	SetHFlag(((r & 0x0F) - (memory & 0x0F)) < 0x00);
	
	r -= memory;
	
	SetCFlag(r < 0x00);
	
	r &= 0xFF;
	
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::SBC_R_R(int &r1, int r2)
{
	int oldCFlag = GetCFlag();
	
	SetNFlag(GBC_TRUE);
	SetHFlag(((r1 & 0x0F) - ((r2 & 0x0F) + (oldCFlag ? 0x01 : 0x00))) < 0x00);
	
	r1 -= (r2 + (oldCFlag ? 0x01 : 0x00));
	
	SetCFlag(r1 < 0x00);
	
	r1 &= 0xFF;
	
	SetZFlag(r1 == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::SBC_R_AA(int &r, int a1, int a2)
{
	int memory = _bus->ReadByte(JoinBytes(a1, a2));
	int oldCFlag = GetCFlag();
	
	SetNFlag(GBC_TRUE);
	SetHFlag(((r & 0x0F) - ((memory & 0x0F) + (oldCFlag ? 0x01 : 0x00))) < 0x00);
	
	r -= (memory + (oldCFlag ? 0x01 : 0x00));
	
	SetCFlag(r < 0x00);
	
	r &= 0xFF;
	
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::AND_R_R(int &r1, int r2)
{
	r1 &= r2;
	
	SetCFlag(GBC_FALSE);
	SetHFlag(GBC_TRUE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r1 == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::AND_R_AA(int &r, int a1, int a2)
{
	int memory = _bus->ReadByte(JoinBytes(a1, a2));
	
	r &= memory;
	
	SetCFlag(GBC_FALSE);
	SetHFlag(GBC_TRUE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::XOR_R_R(int &r1, int r2)
{
	r1 ^= r2;
	
	SetCFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r1 == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::XOR_R_AA(int &r, int a1, int a2)
{
	int memory = _bus->ReadByte(JoinBytes(a1, a2));
	
	r ^= memory;
	
	SetCFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::OR_R_R(int &r1, int r2)
{
	r1 |= r2;
	
	SetCFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r1 == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::OR_R_AA(int &r, int a1, int a2)
{
	int memory = _bus->ReadByte(JoinBytes(a1, a2));
	
	r |= memory;
	
	SetCFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::CP_R_R(int &r1, int r2)
{
	SetCFlag(r2 > r1);
	SetHFlag((r2 & 0x0F) > (r1 & 0x0F));
	SetNFlag(GBC_TRUE);
	SetZFlag(r2 == r1);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::CP_R_AA(int &r, int a1, int a2)
{
	int memory = _bus->ReadByte(JoinBytes(a1, a2));
	
	SetCFlag(memory > r);
	SetHFlag((memory & 0x0F) > (r & 0x0F));
	SetNFlag(GBC_TRUE);
	SetZFlag(memory == r);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::CALL_NN()
{
	_state.sp -= 2;
	
	_bus->WriteByte(_state.sp + 1, GetHigh(_state.pc + GetOpLength(InstructionTable::DEFAULT)));
	_bus->WriteByte(_state.sp, GetLow(_state.pc + GetOpLength(InstructionTable::DEFAULT)));
	
	_state.pc = JoinBytes(GetOpHigh(InstructionTable::DEFAULT), GetOpLow(InstructionTable::DEFAULT));
	
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::CALL_NN_IF(int c)
{
	if (c)
	{
		_state.sp -= 2;
		
		_bus->WriteByte(_state.sp + 1, GetHigh(_state.pc + GetOpLength(InstructionTable::DEFAULT)));
		_bus->WriteByte(_state.sp, GetLow(_state.pc + GetOpLength(InstructionTable::DEFAULT)));
		
		_state.pc = JoinBytes(GetOpHigh(InstructionTable::DEFAULT), GetOpLow(InstructionTable::DEFAULT));
		
		UpdateAdditionalTicks(InstructionTable::DEFAULT);
	}
	else
	{
		UpdatePC(InstructionTable::DEFAULT);
	}
	
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::RET()
{
	_state.pc = JoinBytes(_bus->ReadByte(_state.sp + 1), _bus->ReadByte(_state.sp));
	
	_state.sp += 2;
	
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::RET_IF(int c)
{
	if (c)
	{
		_state.pc = JoinBytes(_bus->ReadByte(_state.sp + 1), _bus->ReadByte(_state.sp));
		
		_state.sp += 2;
		
		UpdateAdditionalTicks(InstructionTable::DEFAULT);
	}
	else
	{
		UpdatePC(InstructionTable::DEFAULT);
	}
	
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::RETI()
{
	_state.interruptsEnabled = GBC_TRUE;
	_state.halted = GBC_FALSE;
	
	_state.pc = (_bus->ReadByte(_state.sp + 1) << 8) | _bus->ReadByte(_state.sp);
	
	_state.sp += 2;
	
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::JP_NN()
{
	_state.pc = JoinBytes(GetOpHigh(InstructionTable::DEFAULT), GetOpLow(InstructionTable::DEFAULT));
	
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::JP_NN_IF(int c)
{
	if (c)
	{
		_state.pc = JoinBytes(GetOpHigh(InstructionTable::DEFAULT), GetOpLow(InstructionTable::DEFAULT));
		
		UpdateAdditionalTicks(InstructionTable::DEFAULT);
	}
	else
	{
		UpdatePC(InstructionTable::DEFAULT);
	}
	
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::JP_RR(int r1, int r2)
{
	_state.pc = JoinBytes(r1, r2);
	
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::POP_RR(int &r1, int &r2)
{
	r1 = _bus->ReadByte(_state.sp + 1);
	r2 = _bus->ReadByte(_state.sp);
	
	_state.sp += 2;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::POP_AF()
{
	_state.a = _bus->ReadByte(_state.sp + 1);
	_state.f = _bus->ReadByte(_state.sp) & 0xF0;
	
	_state.sp += 2;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::PUSH_RR(int r1, int r2)
{
	_state.sp -= 2;
	
	_bus->WriteByte(_state.sp + 1, r1);
	_bus->WriteByte(_state.sp, r2);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::PUSH_AF()
{
	_state.sp -= 2;
	
	_bus->WriteByte(_state.sp + 1, _state.a);
	_bus->WriteByte(_state.sp, _state.f & 0xF0);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::ADD_R_N(int &r)
{
	ADD_R_R(r, GetOpLow(InstructionTable::DEFAULT));
}

void Processor::ADC_R_N(int &r)
{
	ADC_R_R(r, GetOpLow(InstructionTable::DEFAULT));
}

void Processor::SUB_R_N(int &r)
{
	SUB_R_R(r, GetOpLow(InstructionTable::DEFAULT));
}

void Processor::SBC_R_N(int &r)
{
	SBC_R_R(r, GetOpLow(InstructionTable::DEFAULT));
}

void Processor::AND_R_N(int &r)
{
	AND_R_R(r, GetOpLow(InstructionTable::DEFAULT));
}

void Processor::XOR_R_N(int &r)
{
	XOR_R_R(r, GetOpLow(InstructionTable::DEFAULT));
}

void Processor::OR_R_N(int &r)
{
	OR_R_R(r, GetOpLow(InstructionTable::DEFAULT));
}

void Processor::CP_R_N(int &r)
{
	CP_R_R(r, GetOpLow(InstructionTable::DEFAULT));
}

void Processor::RST(int a)
{
	_state.sp -= 2;
	
	_bus->WriteByte(_state.sp + 1, GetHigh(_state.pc + GetOpLength(InstructionTable::DEFAULT)));
	_bus->WriteByte(_state.sp, GetLow(_state.pc + GetOpLength(InstructionTable::DEFAULT)));
	
	_state.pc = a;
	
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LDH_A_R(int a, int r)
{
	LD_A_R(a, r);
}

void Processor::LDH_R_A(int &r, int a)
{
	LD_R_A(r, a);
}

void Processor::LD_A_R(int a, int r)
{
	_bus->WriteByte(JoinBytes(0xFF, a), r);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_R_A(int &r, int a)
{
	r = _bus->ReadByte(JoinBytes(0xFF, a));
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_AA_R(int a1, int a2, int r)
{
	_bus->WriteByte(JoinBytes(a1, a2), r);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::ADD_SP_N()
{
	SetCFlag(0xFF - (_state.sp & 0xFF) < GetOpLow(InstructionTable::DEFAULT));          // to be understood
	SetHFlag(0x0F - (_state.sp & 0x0F) < (GetOpLow(InstructionTable::DEFAULT) & 0x0F)); // to be understood
	SetNFlag(GBC_FALSE);
	SetZFlag(GBC_FALSE);
	
	_state.sp += GetSignedValue(GetOpLow(InstructionTable::DEFAULT));
	_state.sp &= 0xFFFF;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_HL_SP_N()
{
	SetCFlag(0xFF - (_state.sp & 0xFF) < GetOpLow(InstructionTable::DEFAULT));          // to be understood
	SetHFlag(0x0F - (_state.sp & 0x0F) < (GetOpLow(InstructionTable::DEFAULT) & 0x0F)); // to be understood
	SetNFlag(GBC_FALSE);
	SetZFlag(GBC_FALSE);
	
	int result = _state.sp + GetSignedValue(GetOpLow(InstructionTable::DEFAULT));
	
	_state.h = GetHigh(result);
	_state.l = GetLow(result);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::LD_SP_HL()
{
	_state.sp = JoinBytes(_state.h, _state.l);
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::DI()
{
	_state.interruptsEnabled = GBC_FALSE;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

void Processor::EI()
{
	_state.interruptsEnabled = GBC_TRUE;
	
	UpdatePC(InstructionTable::DEFAULT);
	UpdateTicks(InstructionTable::DEFAULT);
}

// cb instruction set

void Processor::RLC_R(int &r)
{
	SetCFlag(GetBit(r, 7));
	
	r = ((r << 1) & 0xFE) | (GetBit(r, 7) ? 0x01 : 0x00);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::RLC_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	SetCFlag(GetBit(memory, 7));
	
	memory = ((memory << 1) & 0xFE) | (GetBit(memory, 7) ? 0x01 : 0x00);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(memory == 0x00);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::RRC_R(int &r)
{
	SetCFlag(GetBit(r, 0));
	
	r = ((r >> 1) & 0x7F) | (GetBit(r, 0) ? 0x80 : 0x00);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::RRC_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	SetCFlag(GetBit(memory, 0));
	
	memory = ((memory >> 1) & 0x7F) | (GetBit(memory, 0) ? 0x80 : 0x00);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(memory == 0x00);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::RL_R(int &r)
{
	int oldCFlag = GetCFlag();
	
	SetCFlag(GetBit(r, 7));
	
	r = ((r << 1) & 0xFE) | (oldCFlag ? 0x01 : 0x00);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::RL_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	int oldCFlag = GetCFlag();
	
	SetCFlag(GetBit(memory, 7));
	
	memory = ((memory << 1) & 0xFE) | (oldCFlag ? 0x01 : 0x00);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(memory == 0x00);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::RR_R(int &r)
{
	int oldCFlag = GetCFlag();
	
	SetCFlag(GetBit(r, 0));
	
	r = ((r >> 1) & 0x7F) | (oldCFlag ? 0x80 : 0x00);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::RR_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	int oldCFlag = GetCFlag();
	
	SetCFlag(GetBit(memory, 0));
	
	memory = ((memory >> 1) & 0x7F) | (oldCFlag ? 0x80 : 0x00);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(memory == 0x00);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SLA_R(int &r)
{
	SetCFlag(GetBit(r, 7));
	
	r = (r << 1) & 0xFE;
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SLA_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	SetCFlag(GetBit(memory, 7));
	
	memory = (memory << 1) & 0xFE;
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(memory == 0x00);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SRA_R(int &r)
{
	SetCFlag(GetBit(r, 0));
	
	r = (r & 0x80) | ((r >> 1) & 0x7F);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SRA_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	SetCFlag(GetBit(memory, 0));
	
	memory = (memory & 0x80) | ((memory >> 1) & 0x7F);
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(memory == 0x00);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SWAP_R(int &r)
{
	r = ((r << 4) | (r >> 4)) & 0xFF;
	
	SetCFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SWAP_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	memory = ((memory << 4) | (memory >> 4)) & 0xFF;
	
	SetCFlag(GBC_FALSE);
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(memory == 0x00);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SRL_R(int &r)
{
	SetCFlag(GetBit(r, 0));
	
	r = (r >> 1) & 0x7F;
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(r == 0x00);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SRL_AA(int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	SetCFlag(GetBit(memory, 0));
	
	memory = (memory >> 1) & 0x7F;
	
	SetHFlag(GBC_FALSE);
	SetNFlag(GBC_FALSE);
	SetZFlag(memory == 0x00);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::BIT_X_R(int x, int &r)
{
	SetHFlag(GBC_TRUE);
	SetNFlag(GBC_FALSE);
	SetZFlag(!GetBit(r, x));
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::BIT_X_AA(int x, int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	SetHFlag(GBC_TRUE);
	SetNFlag(GBC_FALSE);
	SetZFlag(!GetBit(memory, x));
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::RES_X_R(int x, int &r)
{
	r = SetBit(r, x, GBC_FALSE);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::RES_X_AA(int x, int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	memory = SetBit(memory, x, GBC_FALSE);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SET_X_R(int x, int &r)
{
	r = SetBit(r, x, GBC_TRUE);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}

void Processor::SET_X_AA(int x, int a1, int a2)
{
	int address = JoinBytes(a1, a2);
	int memory = _bus->ReadByte(address);
	
	memory = SetBit(memory, x, GBC_TRUE);
	
	_bus->WriteByte(address, memory);
	
	UpdatePC(InstructionTable::CB);
	UpdateTicks(InstructionTable::CB);
}
