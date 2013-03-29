#ifndef _LookUpTables_hpp_
#define _LookUpTables_hpp_

//#include "../GBC.hpp"
//#include "../Tools.hpp"
//#include "../MemoryBus.hpp"
//#include "State.hpp"

/* INSTRUCTIONS START */
/*
#define NOP() result = 0x00

#define INC_RR(r1, r2) \
	r2++; \
	if (r2 > 0xFF) \
	{ \
		r1++; \
		r1 &= 0xFF; \
		r2 &= 0xFF; \
	} \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define INC_R(r) \
	SET_HFLAG((r & 0x0F) == 0x0F); \
	r++; \
	r &= 0xFF; \
	SET_ZFLAG(r == 0x00); \
	SET_NFLAG(GBC_FALSE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define INC_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_HFLAG((memory & 0x0F) == 0x0F); \
	memory++; \
	memory &= 0xFF; \
	SET_ZFLAG(memory == 0x00); \
	SET_NFLAG(GBC_FALSE); \
	WRITE(address, memory); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define DEC_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_HFLAG((memory & 0x0F) == 0x00); \
	memory--; \
	memory &= 0xFF; \
	SET_ZFLAG(memory == 0x00); \
	SET_NFLAG(GBC_TRUE); \
	WRITE(address, memory); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define DEC_RR(r1, r2) \
	r2--; \
	if (r2 < 0x00) \
	{ \
		r1--; \
		r1 &= 0xFF; \
		r2 &= 0xFF; \
	} \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define DEC_R(r) \
	SET_HFLAG((r & 0x0F) == 0x00); \
	r--; \
	r &= 0xFF; \
	SET_ZFLAG(r == 0x00); \
	SET_NFLAG(GBC_TRUE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_RR_NN(r1, r2) \
	r1 = OP_HIGH; \
	r2 = OP_LOW; \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_AA_R(a1, a2, r) \
	WRITE(JOIN_BYTES(a1, a2), r); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RLCA() \
	SET_CFLAG(((A >> 7) & 0x01) != 0x00); \
	A = ((A << 1) & 0xFF) | ((A >> 7) & 0x01); \
	SET_ZFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_AA_SP(a1, a2) \
	WRITE(JOIN_BYTES(a1, a2), SP); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define ADD_RR_RR(r11, r12, r21, r22) \
	SET_HFLAG((JOIN_BYTES(r11, r12) & 0x0FFF) + (JOIN_BYTES(r21, r22) & 0x0FFF)) > 0x0F00; \
	SET_CFLAG((JOIN_BYTES(r11, r12) & 0xFFFF) + (JOIN_BYTES(r21, r22) & 0xFFFF)) > 0xFFFF; \
	result = (JOIN_BYTES(r11, r12) & 0xFFFF) + (JOIN_BYTES(r21, r22) & 0xFFFF); \
	r11 = GET_HIGH(result); \
	r12 = GET_LOW(result); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define ADD_RR_SP(r1, r2) \
	SET_HFLAG((JOIN_BYTES(r1, r2) & 0x0FFF) + (JOIN_BYTES(GET_HIGH(SP), GET_LOW(SP)) & 0x0FFF)) > 0x0F00; \
	SET_CFLAG((JOIN_BYTES(r1, r2) & 0xFFFF) + (JOIN_BYTES(GET_HIGH(SP), GET_LOW(SP)) & 0xFFFF)) > 0xFFFF; \
	result = (JOIN_BYTES(r1, r2) & 0xFFFF) + (JOIN_BYTES(GET_HIGH(SP), GET_LOW(SP)) & 0xFFFF); \
	r1 = GET_HIGH(result); \
	r2 = GET_LOW(result); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_R_AA(r, a1, a2) \
	A = READ(JOIN_BYTES(a1, a2)); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_R_N(r) \
	r = OP_LOW; \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RRCA() \
	SET_CFLAG((A & 0x01) != 0x00); \
	A = ((A >> 1) & 0xFF) | ((A & 0x01) << 7); \
	SET_ZFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define STOP() \
	STOPPED = GBC_TRUE; \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RLA() \
	old_cflag = GET_CFLAG(); \
	SET_CFLAG((A >> 7) & 0x01) != 0x00); \
	A = ((A << 1) & 0xFF) | (old_cflag ? 0x01 : 0x00); \
	SET_ZFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define JR() \
	PC += GET_SIGNED_VALUE(OP_LOW); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RRA() \
	old_cflag = GET_CFLAG(); \
	SET_CFLAG(((s.A >> 1) & 0xFF) | (old_cflag ? 0x80 : 0x00)); \
	SET_ZFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define JR_IF(val) \
	if (val) \
	{ \
		PC += GET_SIGNED_VALUE(OP_LOW); \
		UPDATE_ADDITIONAL_CYCLES(); \
	} \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_AA_R_INC(a1, a2, r) \
	WRITE(JOIN_BYTES(a1, a2), r); \
	a2++; \
	if (a2 > 0xFF) \
	{ \
		a1++; \
		a1 &= 0xFF; \
		a2 &= 0xFF; \
	} \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_AA_R_DEC(a1, a2, r) \
	WRITE(JOIN_BYTES(a1, a2), r); \
	a2--; \
	if (a2 < 0x00) \
	{ \
		a1--; \
		a1 &= 0xFF; \
		a2 &= 0xFF; \
	} \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_R_AA_INC(r, a1, a2) \
	r = READ(JOIN_BYTES(a1, a2)); \
	a2++; \
	if (a2 > 0xFF) \
	{ \
		a1++; \
		a1 &= 0xFF; \
		a2 &= 0xFF; \
	} \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_R_AA_DEC(r, a1, a2) \
	r = READ(JOIN_BYTES(a1, a2)); \
	a2--; \
	if (a2 < 0x00) \
	{ \
		a1--; \
		a1 &= 0xFF; \
		a2 &= 0xFF; \
	} \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define CPL() \
	SET_CFLAG(A == 0); \
	A ^= 0xFF; \
	A &= 0xFF; \
	SET_NFLAG(GBC_TRUE); \
	SET_HFLAG(GBC_TRUE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SCF() \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_CFLAG(GBC_TRUE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define CCF() \
	SET_HFLAG(GET_CFLAG()); \
	SET_CFLAG(!GET_CFLAG()); \
	SET_NFLAG(GBC_FALSE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_R_R(r1, r2) \
	r1 = r2; \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_AA_R(a1, a2, r) \
	WRITE(JOIN_BYTES(a1, a2), r); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define HALT() \
	HALTED = GBC_TRUE; \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define ADD_R_R(r1, r2) \
	SET_NFLAG(GBC_FALSE); \
	SET_HFLAG(((r1 & 0x0F) + (r2 & 0x0F)) > 0x0F); \
	SET_CFLAG(((r2 & 0xFF) + (r2 & 0xFF)) > 0xFF); \
	r1 += r2; \
	r1 &= 0xFF; \
	SET_ZFLAG(r1 == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define ADD_R_AA(r, a1, a2) \
	memory = READ(JOIN_BYTES(a1, a2)); \
	SET_NFLAG(GBC_FALSE); \
	SET_HFLAG(((r & 0x0F) + (memory & 0x0F)) > 0x0F); \
	SET_CFLAG(((r & 0xFF) + (memory & 0xFF)) > 0xFF); \
	r += memory; \
	r &= 0xFF; \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define ADC_R_R(r1, r2) \
	old_cflag = GET_CFLAG(); \
	SET_NFLAG(GBC_FALSE); \
	SET_HFLAG(((r1 & 0x0F) + (r2 & 0x0F) + (old_cflag ? 0x01 : 0x00)) > 0x0F); \
	SET_CFLAG(((r2 & 0xFF) + (r2 & 0xFF) + (old_cflag ? 0x01 : 0x00)) > 0xFF); \
	r1 += r2 + (old_cflag ? 0x01 : 0x00); \
	r1 &= 0xFF; \
	SET_ZFLAG(r1 == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define ADC_R_AA(r, a1, a2) \
	memory = READ(JOIN_BYTES(a1, a2)); \
	old_cflag = GET_CFLAG(); \
	SET_NFLAG(GBC_FALSE); \
	SET_HFLAG(((r & 0x0F) + (memory & 0x0F) + (old_cflag ? 0x01 : 0x00)) > 0x0F); \
	SET_CFLAG(((r & 0xFF) + (memory & 0xFF) + (old_cflag ? 0x01 : 0x00)) > 0xFF); \
	r += memory + (old_cflag ? 0x01 : 0x00); \
	r &= 0xFF; \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SUB_R_R(r1, r2) \
	SET_NFLAG(GBC_TRUE); \
	SET_HFLAG(((r1 & 0x0F) - (r2 & 0x0F)) < 0x00); \
	SET_CFLAG(((r2 & 0xFF) - (r2 & 0xFF)) < 0x00); \
	r1 -= r2; \
	r1 &= 0xFF; \
	SET_ZFLAG(r1 == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SUB_R_AA(r, a1, a2) \
	memory = READ(JOIN_BYTES(a1, a2)); \
	SET_NFLAG(GBC_TRUE); \
	SET_HFLAG(((r & 0x0F) - (memory & 0x0F)) < 0x00); \
	SET_CFLAG(((r & 0xFF) - (memory & 0xFF)) < 0x00); \
	r -= memory; \
	r &= 0xFF; \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SBC_R_R(r1, r2) \
	old_cflag = GET_CFLAG(); \
	SET_NFLAG(GBC_TRUE); \
	SET_HFLAG(((r1 & 0x0F) - ((r2 & 0x0F) + (old_cflag ? 0x01 : 0x00))) < 0x00); \
	SET_CFLAG(((r2 & 0xFF) - ((r2 & 0xFF) + (old_cflag ? 0x01 : 0x00))) < 0x00); \
	r1 -= (r2 + (old_cflag ? 0x01 : 0x00)); \
	r1 &= 0xFF; \
	SET_ZFLAG(r1 == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SBC_R_AA(r, a1, a2) \
	memory = READ(JOIN_BYTES(a1, a2)); \
	old_cflag = GET_CFLAG(); \
	SET_NFLAG(GBC_TRUE); \
	SET_HFLAG(((r & 0x0F) - ((memory & 0x0F) + (old_cflag ? 0x01 : 0x00))) < 0x00); \
	SET_CFLAG(((r & 0xFF) - ((memory & 0xFF) + (old_cflag ? 0x01 : 0x00))) < 0x00); \
	r -= (memory + (old_cflag ? 0x01 : 0x00)); \
	r &= 0xFF; \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define AND_R_R(r1, r2) \
	r1 &= r2; \
	SET_CFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_TRUE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r1 == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define AND_R_AA(r, a1, a2) \
	memory = READ(JOIN_BYTES(a1, a2)); \
	r &= memory; \
	SET_CFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_TRUE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define XOR_R_R(r1, r2) \
	r1 ^= r2; \
	SET_CFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r1 == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define XOR_R_AA(r, a1, a2) \
	memory = READ(JOIN_BYTES(a1, a2)); \
	r ^= memory; \
	SET_CFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define OR_R_R(r1, r2) \
	r1 |= r2; \
	SET_CFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r1 == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define OR_R_AA(r, a1, a2) \
	memory = READ(JOIN_BYTES(a1, a2)); \
	r |= memory; \
	SET_CFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define CP_R_R(r1, r2) \
	SET_CFLAG(r2 > r1); \
	SET_HFLAG((r2 & 0x0F) > (r1 & 0x0F)); \
	SET_NFLAG(GBC_TRUE); \
	SET_ZFLAG(r2 == r1); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define CP_R_AA(r, a1, a2) \
	memory = READ(JOIN_BYTES(a1, a2)); \
	SET_CFLAG(memory > r); \
	SET_HFLAG((memory & 0x0F) > (r & 0x0F)); \
	SET_NFLAG(GBC_TRUE); \
	SET_ZFLAG(memory == r); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define CALL_NN() \
	SP -= 2; \
	WRITE(SP + 1, GET_HIGH(PC + LENGTHS[OP_CODE])); \
	WRITE(SP, GET_LOW(PC + LENGTHS[OP_CODE])); \
	PC = JOIN_BYTES(OP_HIGH, OP_LOW); \
	UPDATE_TICKS()

#define CAL_NN_IF(val) \
	if (val) \
	{ \
		SP -= 2; \
		WRITE(SP + 1, GET_HIGH(PC + LENGTHS[OP_CODE])); \
		WRITE(SP, GET_LOW(PC + LENGTHS[OP_CODE])); \
		PC = JOIN_BYTES(OP_HIGH, OP_LOW); \
		UPDATE_ADDITIONAL_TICKS(); \
	} \
	else \
	{ \
		UPDATE_PC(); \
	} \
	UPDATE_TICKS()

#define RET() \
	PC = JOIN_BYTES(READ(SP + 1), READ(SP)); \
	SP += 2; \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RET_IF(val) \
	if (val) \
	{ \
		PC = JOIN_BYTES(READ(SP + 1), READ(SP)); \
		SP += 2; \
		UPDATE_ADDITIONAL_TICKS(); \
	} \
	else \
	{ \
		UPDATE_PC(); \
	} \
	UPDATE_TICKS()

#define JP_NN() \
	PC = JOIN_BYTES(OP_HIGH, OP_LOW); \
	UPDATE_TICKS()

#define JP_NN_IF(val) \
	if (val) \
	{ \
		PC = JOIN_BYTES(OP_HIGH, OP_LOW); \
		UPDATE_ADDITIONAL_TICKS(); \
	} \
	else \
	{ \
		UPDATE_PC(); \
	} \
	UPDATE_TICKS()

#define POP_RR(r1, r2) \
	r1 = READ(SP + 1); \
	r2 = READ(SP); \
	SP += 2; \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define PUSH_RR(r1, r2) \
	SP -= 2; \
	WRITE(SP + 1, r1); \
	WRITE(SP, r2); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define ADD_R_N(r) ADD_R_R(r, OP_LOW)
#define ADC_R_N(r) ADC_R_R(r, OP_LOW)
#define SUB_R_N(r) SUB_R_R(r, OP_LOW)
#define SBC_R_N(r) SBC_R_R(r, OP_LOW)
#define AND_R_N(r) AND_R_R(r, OP_LOW)
#define XOR_R_N(r) XOR_R_R(r, OP_LOW)
#define OR_R_N(r) OR_R_R(r, OP_LOW)
#define CP_R_N(r) CP_R_R(r, OP_LOW)

#define RST_R(r) \
	SP -= 2; \
	WRITE(SP + 1, GET_HIGH(PC + LENGTHS[OP_CODE])); \
	WRITE(SP, GET_LOW(PC + LENGTHS[OP_CODE])); \
	PC = r; \
	UPDATE_TICKS()

#define LDH_A_R(a, r) \
	WRITE(JOIN_BYTES(0xFF, a), r); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LDH_R_A(r, a) \
	r = READ(JOIN_BYTES(0xFF, a)); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_AA_R(a1, a2, r) \
	WRITE(JOIN_BYTES(a1, a2), r); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define ADD_SP_N() \
	SP += GET_SIGNED_VALUE(OP_LOW); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(GBC_FALSE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define LD_HL_SP_N() \
	HL = SP + GET_SIGNED_VALUE(OP_LOW); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(GBC_FALSE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define DI() \
	INTERRUPTS_ENABLED = GBC_FALSE; \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define EI() \
	INTERRUPTS_ENABLED = GBC_TRUE; \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RLC_R(r) \
	SET_CFLAG(GET_BIT(r, 7)); \
	r = ((r << 1) & 0xFF) | (GET_BIT(r, 7) ? 0x01 : 0x00); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RLC_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_CFLAG(GET_BIT(memory, 7)); \
	memory = ((memory << 1) & 0xFF) | (GET_BIT(memory, 7) ? 0x01 : 0x00); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(memory == 0x00); \
	WRITE(address, memory); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RRC_R(r) \
	SET_CFLAG(GET_BIT(r, 0)); \
	r = ((r >> 1) & 0xFF) | (GET_BIT(r, 0) ? 0x80 : 0x00); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RRC_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_CFLAG(GET_BIT(memory, 0)); \
	memory = ((memory >> 1) & 0xFF) | (GET_BIT(memory, 0) ? 0x80 : 0x00); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(memory == 0x00); \
	WRITE(address, memory); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RL_R(r) \
	old_cflag = GET_CFLAG(); \
	SET_CFLAG(GET_BIT(r, 7)); \
	r = ((r << 1) & 0xFF) | (old_cflag ? 0x01 : 0x00); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RL_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	old_cflag = GET_CFLAG(); \
	SET_CFLAG(GET_BIT(memory, 7)); \
	memory = ((memory << 1) & 0xFF) | (old_cflag ? 0x01 : 0x00); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(memory == 0x00); \
	WRITE(address, memory); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RR_R(r) \
	old_cflag = GET_CFLAG(); \
	SET_CFLAG(GET_BIT(r, 0)); \
	r = ((r >> 1) & 0xFF) | (oldCFlag ? 0x80 : 0x00); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RR_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	old_cflag = GET_CFLAG(); \
	SET_CFLAG(GET_BIT(memory, 0)); \
	memory = ((memory >> 1) & 0xFF) | (oldCFlag ? 0x80 : 0x00); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(memory == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SLA_R(r) \
	SET_CFLAG(GET_BIT(r, 7)); \
	r = (r << 1) & 0xFF; \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SLA_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_CFLAG(GET_BIT(memory, 7)); \
	memory = (memory << 1) & 0xFF; \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(memory == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SRA_R(r) \
	SET_CFLAG(GET_BIT(r, 0)); \
	r = (r & 0x80) | ((r >> 1) & 0xFF); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SRA_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_CFLAG(GET_BIT(memory, 0)); \
	memory = (memory & 0x80) | ((memory >> 1) & 0xFF); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(memory == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SWAP_R(r) \
	r = ((r << 4) | (r >> 4)) & 0xFF; \
	SET_CFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(memory == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SWAP_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	memory = ((memory << 4) | (memory >> 4)) & 0xFF; \
	SET_CFLAG(GBC_FALSE); \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(memory == 0x00); \
	WRITE(address, memory); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SRL_R(r) \
	SET_CFLAG(GET_BIT(r, 0)); \
	r = (r >> 1) & 0xFF; \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(r == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SRL_AA(a1, a2) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_CFLAG(GET_BIT(memory, 0)); \
	memory = (memory >> 1) & 0xFF; \
	SET_HFLAG(GBC_FALSE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(memory == 0x00); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define BIT_R_X(r, x) \
	SET_HFLAG(GBC_TRUE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(!GET_BIT(r, x); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define BIT_AA_X(a1, a2, x) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_HFLAG(GBC_TRUE); \
	SET_NFLAG(GBC_FALSE); \
	SET_ZFLAG(!GET_BIT(memory, x); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RES_R_X(r, x) \
	SET_BIT(r, x, GBC_FALSE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define RES_AA_X(a1, a2, x) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_BIT(memory, x, GBC_FALSE); \
	WRITE(address, memory); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SET_R_X(r, x) \
	SET_BIT(r, x, GBC_TRUE); \
	UPDATE_PC(); \
	UPDATE_TICKS()

#define SET_AA_X(a1, a2, x) \
	address = JOIN_BYTES(a1, a2); \
	memory = READ(address); \
	SET_BIT(memory, x, GBC_TRUE); \
	WRITE(address, memory); \
	UPDATE_PC(); \
	UPDATE_TICKS()*/

/* INSTRUCTIONS END */

#define DEFAULT_TABLE 0
#define CB_TABLE 1

namespace gbc
{
	namespace core
	{
		namespace cpu
		{
			struct LookUpTables
			{
				static const int INSTRUCTION_CYCLES[2][0x0100];
				static const int ADDITIONAL_INSTRUCTION_CYCLES[2][0x0100];
				static const int INSTRUCTION_LENGTHS[2][0x0100];
			};
		}
	}
}

#endif
