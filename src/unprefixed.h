#ifndef UNPREFIXED_H
#define UNPREFIXED_H

#include "z80.h"

void isa_populate_unprefixed(void (*unprefixed[0xFF])(struct Z80 *z80));

void NOP(struct Z80 *z80);
void UNDEFINED(struct Z80 *z80);

void RRA_1F(struct Z80 *z80);

void OR_B1(struct Z80 *z80);
void OR_B6(struct Z80 *z80);
void OR_B7(struct Z80 *z80);

void XOR_A9(struct Z80 *z80);
void XOR_AE(struct Z80 *z80);
void XOR_AF(struct Z80 *z80);
void XOR_EE(struct Z80 *z80);

void AND_E6(struct Z80 *z80);

void ADD_C6(struct Z80 *z80);
void ADC_CE(struct Z80 *z80);

void SUB_D6(struct Z80 *z80);

void CP_FE(struct Z80 *z80);

void DEC_05(struct Z80 *z80);
void DEC_0D(struct Z80 *z80);
void DEC_25(struct Z80 *z80);
void DEC_2D(struct Z80 *z80);
void DEC_35(struct Z80 *z80);
void DEC_3D(struct Z80 *z80);

void INC_03(struct Z80 *z80);
void INC_13(struct Z80 *z80);
void INC_14(struct Z80 *z80);
void INC_1C(struct Z80 *z80);
void INC_23(struct Z80 *z80);
void INC_24(struct Z80 *z80);
void INC_2C(struct Z80 *z80);

void LD_0E(struct Z80 *z80);

void JR_18(struct Z80 *z80);
void JR_20(struct Z80 *z80);
void JR_28(struct Z80 *z80);
void JR_30(struct Z80 *z80);

void JP_C3(struct Z80 *z80);

void LD_01(struct Z80 *z80);
void LD_06(struct Z80 *z80);
void LD_11(struct Z80 *z80);
void LD_12(struct Z80 *z80);
void LD_1A(struct Z80 *z80);
void LD_21(struct Z80 *z80);
void LD_22(struct Z80 *z80);
void LD_26(struct Z80 *z80);
void LD_2A(struct Z80 *z80);
void LD_31(struct Z80 *z80);
void LD_32(struct Z80 *z80);
void LD_3E(struct Z80 *z80);
void LD_46(struct Z80 *z80);
void LD_47(struct Z80 *z80);
void LD_4E(struct Z80 *z80);
void LD_4F(struct Z80 *z80);
void LD_56(struct Z80 *z80);
void LD_57(struct Z80 *z80);
void LD_5F(struct Z80 *z80);

void LD_70(struct Z80 *z80);
void LD_71(struct Z80 *z80);
void LD_72(struct Z80 *z80);
void LD_77(struct Z80 *z80);
void LD_78(struct Z80 *z80);
void LD_79(struct Z80 *z80);
void LD_7A(struct Z80 *z80);
void LD_7B(struct Z80 *z80);
void LD_7C(struct Z80 *z80);
void LD_7D(struct Z80 *z80);

void LD_E0(struct Z80 *z80);
void LD_EA(struct Z80 *z80);
void LD_F0(struct Z80 *z80);
void LD_FA(struct Z80 *z80);

void DI_F3(struct Z80 *z80);

void CALL_C4(struct Z80 *z80);
void CALL_CD(struct Z80 *z80);
void RET_C8(struct Z80 *z80);
void RET_C9(struct Z80 *z80);
void RET_D0(struct Z80 *z80);

void POP_C1(struct Z80 *z80);
void POP_D1(struct Z80 *z80);
void POP_E1(struct Z80 *z80);
void POP_F1(struct Z80 *z80);

void PUSH_C5(struct Z80 *z80);
void PUSH_D5(struct Z80 *z80);
void PUSH_E5(struct Z80 *z80);
void PUSH_F5(struct Z80 *z80);

#endif