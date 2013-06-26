#ifndef _Processor_hpp_
#define _Processor_hpp_

#include "../gbc.hpp"
#include "../MemoryBus.hpp"
#include "State.hpp"
#include "../InterruptHandler.hpp"
#include "InstructionTable.hpp"
#include "LookUpTables.hpp"

namespace gbc
{
	namespace core
	{
		namespace cpu
		{
			class Processor : public IInterruptHandler
			{
			public:
				static const int CFLAG_BIT = 4;
				static const int HFLAG_BIT = 5;
				static const int NFLAG_BIT = 6;
				static const int ZFLAG_BIT = 7;
			
			public:
				Processor();
				~Processor();
				
				void SetMemoryBus(IMemoryBus *);
				
				void Step();
				void Execute(int);
				
				void ExecuteInstruction();
				void ExecuteInterrupt();
				
				void PowerUp();
				
				void SignalVBlankInterrupt();
				void SignalLCDStatusInterrupt();
				void SignalTimerInterrupt();
				void SignalSerialInterrupt();
				void SignalJoypadInterrupt();
				
				void SetState(State state);
				State GetState();
				
				int GetCounter();
				
			private:
				// helper methods
				void FetchInstruction(InstructionTable instructionTable);
				
				void UpdatePC(InstructionTable instructionTable);
				void UpdateTicks(InstructionTable instructionTable);
				void UpdateAdditionalTicks(InstructionTable instructionTable);
				
				void SetCFlag(int value);
				void SetHFlag(int value);
				void SetNFlag(int value);
				void SetZFlag(int value);
				
				int GetCFlag();
				int GetHFlag();
				int GetNFlag();
				int GetZFlag();
				
				int GetOpCode(InstructionTable instructionTable);
				int GetOpLow(InstructionTable instructionTable);
				int GetOpHigh(InstructionTable instructionTable);
				int GetOpLength(InstructionTable instructionTable);
				
				/* BEGIN INSTRUCTIONS */
				// default instruction set
				void NOP();
				void INC_RR(int &, int &);
				void INC_R(int &);
				void INC_AA(int, int);
				void INC_SP();
				void DEC_AA(int, int);
				void DEC_RR(int &, int &);
				void DEC_R(int &);
				void DEC_SP();
				void LD_RR_NN(int &, int &);
				void LD_SP_NN();
				void RLCA();
				void LD_AA_SP(int, int);
				void ADD_RR_RR(int &, int &, int, int);
				void ADD_RR_SP(int &, int &);
				void LD_R_AA(int &, int, int);
				void LD_R_N(int &);
				void LD_AA_N(int, int);
				void RRCA();
				void STOP();
				void RLA();
				void JR_N();
				void RRA();
				void JR_N_IF(int);
				void LD_AA_R_INC(int &, int &, int);
				void LD_AA_R_DEC(int &, int &, int);
				void LD_R_AA_INC(int &, int &, int &);
				void LD_R_AA_DEC(int &, int &, int &);
				void DAA();
				void CPL();
				void SCF();
				void CCF();
				void LD_R_R(int &, int);
				void HALT();
				void ADD_R_R(int &, int);
				void ADD_R_AA(int &, int, int);
				void ADC_R_R(int &, int);
				void ADC_R_AA(int &, int, int);
				void SUB_R_R(int &, int);
				void SUB_R_AA(int &, int, int);
				void SBC_R_R(int &, int);
				void SBC_R_AA(int &, int, int);
				void AND_R_R(int &, int);
				void AND_R_AA(int &, int, int);
				void XOR_R_R(int &, int);
				void XOR_R_AA(int &, int, int);
				void OR_R_R(int &, int);
				void OR_R_AA(int &, int, int);
				void CP_R_R(int &, int);
				void CP_R_AA(int &, int, int);
				void CALL_NN();
				void CALL_NN_IF(int);
				void RET();
				void RET_IF(int);
				void RETI();
				void JP_NN();
				void JP_NN_IF(int);
				void JP_RR(int, int);
				void POP_RR(int &, int &);
				void POP_AF();
				void PUSH_RR(int, int);
				void PUSH_AF();
				void ADD_R_N(int &);
				void ADC_R_N(int &);
				void SUB_R_N(int &);
				void SBC_R_N(int &);
				void AND_R_N(int &);
				void XOR_R_N(int &);
				void OR_R_N(int &);
				void CP_R_N(int &);
				void RST(int);
				void LDH_A_R(int, int);
				void LDH_R_A(int &, int);
				void LD_A_R(int, int);
				void LD_R_A(int &, int);
				void LD_AA_R(int, int, int);
				void ADD_SP_N();
				void LD_HL_SP_N();
				void LD_SP_HL();
				void DI();
				void EI();
				// cb instruction set
				void RLC_R(int &);
				void RLC_AA(int, int);
				void RRC_R(int &);
				void RRC_AA(int, int);
				void RL_R(int &);
				void RL_AA(int, int);
				void RR_R(int &);
				void RR_AA(int, int);
				void SLA_R(int &);
				void SLA_AA(int, int);
				void SRA_R(int &);
				void SRA_AA(int, int);
				void SWAP_R(int &);
				void SWAP_AA(int, int);
				void SRL_R(int &);
				void SRL_AA(int, int);
				void BIT_X_R(int, int &);
				void BIT_X_AA(int, int, int);
				void RES_X_R(int, int &);
				void RES_X_AA(int, int, int);
				void SET_X_R(int, int &);
				void SET_X_AA(int, int, int);
				/* END INSTRUCTIONS */
				
				int _counter;
				
				Array<int, LookUpTables::AMOUNT_OF_INSTUCTION_TABLES> _currentOpCodes;
				Array<int, LookUpTables::AMOUNT_OF_INSTUCTION_TABLES> _currentOpLows;
				Array<int, LookUpTables::AMOUNT_OF_INSTUCTION_TABLES> _currentOpHighs;
				
				State _state;
				IMemoryBus *_bus;
			};
		}
	}
}

#endif
