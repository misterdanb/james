#ifndef _Processor_hpp_
#define _Processor_hpp_

#include "../GBC.hpp"
#include "../Tools.hpp"
#include "../MemoryBus.hpp"
#include "State.hpp"
#include "LookUpTables.hpp"

#define STATE (_state)
#define BUS (*_bus)

#define READ(addr) (BUS.ReadByte((addr)))
#define WRITE(addr, val) (BUS.WriteByte((addr), (val)))

#define S_A STATE.a
#define S_F STATE.f
#define S_B STATE.b
#define S_C STATE.c
#define S_D STATE.d
#define S_E STATE.e
#define S_H STATE.h
#define S_L STATE.l
#define S_PC STATE.pc
#define S_SP STATE.sp

#define S_TICKS STATE.ticks

#define S_INTERRUPTS_ENABLED STATE.interruptsEnabled
#define S_STOPPED STATE.stopped
#define S_HALTED STATE.halted

#define CFLAG_NUM 4
#define HFLAG_NUM 5
#define NFLAG_NUM 6
#define ZFLAG_NUM 7

#define SET_CFLAG(val) S_F = SET_BIT(S_F, CFLAG_NUM, (val))
#define SET_HFLAG(val) S_F = SET_BIT(S_F, HFLAG_NUM, (val))
#define SET_NFLAG(val) S_F = SET_BIT(S_F, NFLAG_NUM, (val))
#define SET_ZFLAG(val) S_F = SET_BIT(S_F, ZFLAG_NUM, (val))

#define GET_CFLAG() (GET_BIT(S_F, CFLAG_NUM))
#define GET_HFLAG() (GET_BIT(S_F, HFLAG_NUM))
#define GET_NFLAG() (GET_BIT(S_F, NFLAG_NUM))
#define GET_ZFLAG() (GET_BIT(S_F, ZFLAG_NUM))

#define FETCH_OP_CODE() int opCode = (READ(S_PC))
#define FETCH_OP_LOW() int opLow = (READ(S_PC + 1))
#define FETCH_OP_HIGH() int opHigh = (READ(S_PC + 2))

#define CB_FETCH_OP_CODE() int cbOPCode = (READ(S_PC + 1))
#define CB_FETCH_OP_LOW() int cbOPLow = (READ(S_PC + 2))
#define CB_FETCH_OP_HIGH() int cbOPHigh = (READ(S_PC + 3))

#define FETCH_INSTRUCTION() \
	FETCH_OP_CODE(); \
	FETCH_OP_LOW(); \
	FETCH_OP_HIGH();
	
#define CB_FETCH_INSTRUCTION() \
	CB_FETCH_OP_CODE(); \
	CB_FETCH_OP_LOW(); \
	CB_FETCH_OP_HIGH();

#define GET_OP_CODE() opCode
#define GET_OP_LOW() opLow
#define GET_OP_HIGH() opHigh

#define CB_GET_OP_CODE() cbOPCode
#define CB_GET_OP_LOW() cbOPLow
#define CB_GET_OP_HIGH() cbOPHigh

#define LENGTH LookUpTables::INSTRUCTION_LENGTHS[DEFAULT_TABLE][GET_OP_CODE()]
#define CYCLES LookUpTables::INSTRUCTION_CYCLES[DEFAULT_TABLE][GET_OP_CODE()]
#define ADDITIONAL_CYCLES LookUpTables::ADDITIONAL_INSTRUCTION_CYCLES[DEFAULT_TABLE][GET_OP_CODE()]

#define CB_LENGTH LookUpTables::INSTRUCTION_LENGTHS[CB_TABLE][CB_GET_OP_CODE()]
#define CB_CYCLES LookUpTables::INSTRUCTION_CYCLES[CB_TABLE][CB_GET_OP_CODE()]
#define CB_ADDITIONAL_CYCLES LookUpTables::ADDITIONAL_INSTRUCTION_CYCLES[CB_TABLE][CB_GET_OP_CODE()]

#define UPDATE_PC() S_PC += LENGTH
#define UPDATE_TICKS() S_TICKS += CYCLES
#define UPDATE_ADDITIONAL_TICKS() S_TICKS += CYCLES

#define CB_UPDATE_PC() S_PC += CB_LENGTH
#define CB_UPDATE_TICKS() S_TICKS += CB_CYCLES
#define CB_UPDATE_ADDITIONAL_TICKS() S_TICKS += CB_ADDITIONAL_CYCLES

namespace gbc
{
	namespace core
	{
		namespace cpu
		{
			class Processor
			{
			public:
				Processor();
				~Processor();
				
				void SetMemoryBus(IMemoryBus *);
		
				void Step();
		
				void ExecuteInstruction();
				void ExecuteInterrupt();
		
				void SetState(State state);
				State GetState();
				
			private:
				/* BEGIN INSTRUCTIONS */
				// default instruction set
				void NOP();
				void INC_RR(int *, int *);
				void INC_R(int *);
				void INC_AA(int *, int *);
				void INC_SP();
				void DEC_AA(int *, int *);
				void DEC_RR(int *, int *);
				void DEC_R(int *);
				void DEC_SP();
				void LD_RR_NN(int *, int *);
				void LD_SP_NN();
				void RLCA();
				void LD_AA_SP(int *, int *);
				void ADD_RR_RR(int *, int *, int *, int *);
				void ADD_RR_SP(int *, int *);
				void LD_R_AA(int *, int *, int *);
				void LD_R_N(int *);
				void LD_AA_N(int *, int *);
				void RRCA();
				void STOP();
				void RLA();
				void JR_N();
				void RRA();
				void JR_N_IF(int);
				void LD_AA_R_INC(int *, int *, int *);
				void LD_AA_R_DEC(int *, int *, int *);
				void LD_R_AA_INC(int *, int *, int *);
				void LD_R_AA_DEC(int *, int *, int *);
				void DAA();
				void CPL();
				void SCF();
				void CCF();
				void LD_R_R(int *, int *);
				void HALT();
				void ADD_R_R(int *, int *);
				void ADD_R_AA(int *, int *, int *);
				void ADC_R_R(int *, int *);
				void ADC_R_AA(int *, int *, int *);
				void SUB_R_R(int *, int *);
				void SUB_R_AA(int *, int *, int *);
				void SBC_R_R(int *, int *);
				void SBC_R_AA(int *, int *, int *);
				void AND_R_R(int *, int *);
				void AND_R_AA(int *, int *, int *);
				void XOR_R_R(int *, int *);
				void XOR_R_AA(int *, int *, int *);
				void OR_R_R(int *, int *);
				void OR_R_AA(int *, int *, int *);
				void CP_R_R(int *, int *);
				void CP_R_AA(int *, int *, int *);
				void CALL_NN();
				void CALL_NN_IF(int);
				void RET();
				void RET_IF(int);
				void RETI();
				void JP_NN();
				void JP_NN_IF(int);
				void JP_RR(int *, int *);
				void POP_RR(int *, int *);
				void PUSH_RR(int *, int *);
				void ADD_R_N(int *);
				void ADC_R_N(int *);
				void SUB_R_N(int *);
				void SBC_R_N(int *);
				void AND_R_N(int *);
				void XOR_R_N(int *);
				void OR_R_N(int *);
				void CP_R_N(int *);
				void RST(int);
				void LDH_A_R(int *, int *);
				void LDH_R_A(int *, int *);
				void LD_A_R(int *, int *);
				void LD_R_A(int *, int *);
				void LD_AA_R(int *, int *, int *);
				void ADD_SP_N();
				void LD_HL_SP_N();
				void LD_SP_HL();
				void DI();
				void EI();
				// cb instruction set
				void RLC_R(int *);
				void RLC_AA(int *, int *);
				void RRC_R(int *);
				void RRC_AA(int *, int *);
				void RL_R(int *);
				void RL_AA(int *, int *);
				void RR_R(int *);
				void RR_AA(int *, int *);
				void SLA_R(int *);
				void SLA_AA(int *, int *);
				void SRA_R(int *);
				void SRA_AA(int *, int *);
				void SWAP_R(int *);
				void SWAP_AA(int *, int *);
				void SRL_R(int *);
				void SRL_AA(int *, int *);
				void BIT_X_R(int, int *);
				void BIT_X_AA(int, int *, int *);
				void RES_X_R(int, int *);
				void RES_X_AA(int, int *, int *);
				void SET_X_R(int, int *);
				void SET_X_AA(int, int *, int *);
				/* END INSTRUCTIONS */
				
				State _state;
				IMemoryBus *_bus;
			};
		}
	}
}

#endif
