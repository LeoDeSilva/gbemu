#include <stdio.h>
#include "unprefixed.h"
#include "z80.h"

void isa_populate_unprefixed(void (*unprefixed[0xFF])(struct Z80 *z80)) {
    for (int i = 0; i < 0xFF; i++) {
        unprefixed[i] = UNDEFINED;
    }

    unprefixed[0x00] = NOP;
    unprefixed[0x01] = LD_01;
    unprefixed[0x03] = INC_03;
    unprefixed[0x05] = DEC_05;
    unprefixed[0x06] = LD_06;
    unprefixed[0x0D] = DEC_0D;
    unprefixed[0x0E] = LD_0E;

    unprefixed[0x11] = LD_11;
    unprefixed[0x12] = LD_12;
    unprefixed[0x13] = INC_13;
    unprefixed[0x14] = INC_14;
    unprefixed[0x18] = JR_18;
    unprefixed[0x1A] = LD_1A;
    unprefixed[0x1C] = INC_1C;
    unprefixed[0x1D] = DEC_1D;
    unprefixed[0x1F] = RRA_1F;

    unprefixed[0x20] = JR_20;
    unprefixed[0x21] = LD_21;
    unprefixed[0x22] = LD_22;
    unprefixed[0x23] = INC_23;
    unprefixed[0x24] = INC_24;
    unprefixed[0x25] = DEC_25;
    unprefixed[0x26] = LD_26;
    unprefixed[0x28] = JR_28;
    unprefixed[0x29] = ADD_29;
    unprefixed[0x2A] = LD_2A;
    unprefixed[0x2C] = INC_2C;
    unprefixed[0x2D] = DEC_2D;

    unprefixed[0x30] = JR_30;
    unprefixed[0x31] = LD_31;
    unprefixed[0x32] = LD_32;
    unprefixed[0x35] = DEC_35;
    unprefixed[0x3C] = INC_3C;
    unprefixed[0x3D] = DEC_3D;
    unprefixed[0x3E] = LD_3E;

    unprefixed[0x46] = LD_46;
    unprefixed[0x47] = LD_47;
    unprefixed[0x4E] = LD_4E;
    unprefixed[0x4F] = LD_4F;

    unprefixed[0x56] = LD_56;
    unprefixed[0x57] = LD_57;
    unprefixed[0x5F] = LD_5F;

    unprefixed[0x6E] = LD_6E;
    unprefixed[0x6F] = LD_6F;

    unprefixed[0x70] = LD_70;
    unprefixed[0x71] = LD_71;
    unprefixed[0x72] = LD_72;
    unprefixed[0x77] = LD_77;
    unprefixed[0x78] = LD_78;
    unprefixed[0x79] = LD_79;
    unprefixed[0x7A] = LD_7A;
    unprefixed[0x7B] = LD_7B;
    unprefixed[0x7C] = LD_7C;
    unprefixed[0x7D] = LD_7D;

    unprefixed[0xA9] = XOR_A9;
    unprefixed[0xAE] = XOR_AE;
    unprefixed[0xAF] = XOR_AF;

    unprefixed[0xB1] = OR_B1;
    unprefixed[0xB6] = OR_B6;
    unprefixed[0xB7] = OR_B7;

    unprefixed[0xC1] = POP_C1;
    unprefixed[0xC3] = JP_C3;
    unprefixed[0xC4] = CALL_C4;
    unprefixed[0xC5] = PUSH_C5;
    unprefixed[0xC6] = ADD_C6;
    unprefixed[0xC8] = RET_C8;
    unprefixed[0xC9] = RET_C9;
    unprefixed[0xCD] = CALL_CD;
    unprefixed[0xCE] = ADC_CE;

    unprefixed[0xD0] = RET_D0;
    unprefixed[0xD1] = POP_D1;
    unprefixed[0xD5] = PUSH_D5;
    unprefixed[0xD6] = SUB_D6;

    unprefixed[0xE0] = LD_E0;
    unprefixed[0xE1] = POP_E1;
    unprefixed[0xE5] = PUSH_E5;
    unprefixed[0xE5] = PUSH_E5;
    unprefixed[0xE6] = AND_E6;
    unprefixed[0xE9] = JP_E9;
    unprefixed[0xEA] = LD_EA;
    unprefixed[0xEE] = XOR_EE;

    unprefixed[0xF0] = LD_F0;
    unprefixed[0xF1] = POP_F1;
    unprefixed[0xF3] = DI_F3;
    unprefixed[0xF5] = PUSH_F5;
    unprefixed[0xFA] = LD_FA;
    unprefixed[0xFE] = CP_FE;
}

// Placeholder =====

void NOP(struct Z80 *z80) {
    z80->elapsed_cycles = 4;
}

void UNDEFINED(struct Z80 *z80) {
    printf("%02x\n", z80->memory[z80->pc-1]);
    z80->is_running_flag = false;
    z80->elapsed_cycles = 4;
}


// Logical Operations =====

void RRA_1F(struct Z80 *z80) { // RR A
    uint8_t a = z80->af >> 8;
    z80->af &= 0x00FF;
    z80->af |= logical_rr(z80, a) << 8;
    z80->elapsed_cycles = 4;
}

void OR_B1(struct Z80 *z80) { // OR C
    uint8_t n = (z80->af >> 8) | (z80->bc & 0xFF);
    z80->af &= 0x0000;
    z80->af |= (n << 8) | (n == 0) << FLAG_Z;
    z80->elapsed_cycles = 4;
}

void OR_B6(struct Z80 *z80) { // OR (HL)
    uint8_t n = (z80->af >> 8) | address_byte(z80, z80->hl);
    z80->af &= 0x0000;
    z80->af |= (n << 8) | (n == 0) << FLAG_Z;
    z80->elapsed_cycles = 8;
}

void OR_B7(struct Z80 *z80) { // OR A
    uint8_t n = (z80->af >> 8) | (z80->af >> 8);
    z80->af &= 0x0000;
    z80->af |= (n << 8) | (n == 0) << FLAG_Z;
    z80->elapsed_cycles = 4;
}

void XOR_A9(struct Z80 *z80) { // XOR C
    uint8_t n = (z80->af >> 8) ^ (z80->bc & 0xFF);
    z80->af &= 0x0000;
    z80->af |= (n << 8) | (n == 0) << FLAG_Z;
    z80->elapsed_cycles = 4;
}

void XOR_AE(struct Z80 *z80) { // XOR (HL)
    uint8_t n = (z80->af >> 8) ^ address_byte(z80, z80->hl);
    z80->af &= 0x0000;
    z80->af |= (n << 8) | (n == 0) << FLAG_Z;
    z80->elapsed_cycles = 8;
}

void XOR_AF(struct Z80 *z80) { // XOR A, A
    uint8_t a = z80->af >> 8;
    z80->af &= 0x0000;
    z80->af |= ((a^a) << 8) | 1 << FLAG_Z;
    z80->elapsed_cycles = 4;
}

void XOR_EE(struct Z80 *z80) { // XOR d8
    uint8_t n = (z80->af >> 8) ^ fetch_byte(z80);
    z80->af &= 0x0000;
    z80->af |= (n << 8) | (n == 0) << FLAG_Z;
    z80->elapsed_cycles = 8;
}

void AND_E6(struct Z80 *z80) { // AND d8
    uint8_t a = z80->af >> 8;
    uint8_t n = fetch_byte(z80);
    z80->af &= 0x0000;
    uint8_t f = 0b00100000 | ((a & n) == 0) << FLAG_Z;
    z80->af |= ((a&n) << 8) | f;
    z80->elapsed_cycles = 8;
}

// Arithmetic instructions ====

void ADD_29(struct Z80 *z80) { // ADD HL, HL
    uint16_t hl = z80->hl;
    z80->hl = arith_add16(z80, hl, hl);
    z80->elapsed_cycles = 8;
}

void ADD_C6(struct Z80 *z80) { // ADD d8
    uint8_t a = z80->af >> 8;
    uint8_t n = fetch_byte(z80);
    z80->af &= 0x00FF;
    z80->af |= (arith_add(z80, a, n)) << 8;
    z80->elapsed_cycles = 8;
}

void ADC_CE(struct Z80 *z80) { // ADC d8
    uint8_t a = z80->af >> 8;
    uint8_t n = fetch_byte(z80) + ((z80->af & (1 << FLAG_C)) != 0);
    z80->af &= 0x00FF;
    z80->af |= (arith_add(z80, a, n)) << 8;
    z80->elapsed_cycles = 8;
}

void SUB_D6(struct Z80 *z80) { // SUB d8
    uint8_t a = z80->af >> 8;
    uint8_t n = fetch_byte(z80);
    z80->af &= 0x00FF;
    z80->af |= (arith_sub(z80, a, n)) << 8;
    z80->elapsed_cycles = 8;
}

// DEC Instructions =====

void DEC_05(struct Z80 *z80) { // DEC B 
    uint8_t b = z80->bc >> 8;
    z80->bc &= 0x00FF;
    z80->bc |= arith_dec(z80, b) << 8;
    z80->elapsed_cycles = 4;
}

void DEC_0D(struct Z80 *z80) { // DEC C 
    uint8_t c = z80->bc & 0xFF;
    z80->bc &= 0xFF00;
    z80->bc |= arith_dec(z80, c);
    z80->elapsed_cycles = 4;
}

void DEC_1D(struct Z80 *z80) { // DEC E 
    uint8_t e = z80->de & 0xFF;
    z80->de &= 0xFF00;
    z80->de |= arith_dec(z80, e);
    z80->elapsed_cycles = 4;
}

void DEC_25(struct Z80 *z80) { // DEC H 
    uint8_t h = z80->hl >> 8;
    z80->hl &= 0x00FF;
    z80->hl |= arith_dec(z80, h) << 8;
    z80->elapsed_cycles = 4;
}

void DEC_2D(struct Z80 *z80) { // DEC L 
    uint8_t l = z80->hl & 0xFF;
    z80->hl &= 0xFF00;
    z80->hl |= arith_dec(z80, l);
    z80->elapsed_cycles = 4;
}

void DEC_35(struct Z80 *z80) { // DEC (HL)
    uint8_t n = address_byte(z80, z80->hl);
    write_byte(z80, z80->hl, arith_dec(z80, n));
    z80->elapsed_cycles = 12;
}

void DEC_3D(struct Z80 *z80) { // DEC A 
    uint8_t a = z80->af >> 8;
    z80->af &= 0x00FF;
    z80->af |= arith_dec(z80, a) << 8;
    z80->elapsed_cycles = 4;
}


// INC Instructions =====

void INC_13(struct Z80 *z80) { // INC DE
    z80->de++;
    z80->elapsed_cycles = 8;
}

void INC_14(struct Z80 *z80) { // INC D
    uint8_t d = z80->de >> 8;
    z80->de &= 0x00FF;
    z80->de |= arith_inc(z80, d) << 8;
    z80->elapsed_cycles = 4;
}

void INC_1C(struct Z80 *z80) { // INC E 
    uint8_t e = z80->de & 0xFF;
    z80->de &= 0xFF00;
    z80->de |= arith_inc(z80, e);
    z80->elapsed_cycles = 4;
}

void INC_03(struct Z80 *z80) { // INC BC
    z80->bc++;
    z80->elapsed_cycles = 8;
}

void INC_23(struct Z80 *z80) { // INC HL
    z80->hl++;
    z80->elapsed_cycles = 8;
}

void INC_24(struct Z80 *z80) { // INC H
    uint8_t h = z80->hl >> 8;
    z80->hl &= 0x00FF;
    z80->hl |= arith_inc(z80, h) << 8;
    z80->elapsed_cycles = 4;
}

void INC_2C(struct Z80 *z80) { // INC L 
    uint8_t l = z80->hl & 0xFF;
    z80->hl &= 0xFF00;
    z80->hl |= arith_inc(z80, l);
    z80->elapsed_cycles = 4;
}

void INC_3C(struct Z80 *z80) { // INC A 
    uint8_t a = z80->af >> 8;
    z80->af &= 0x00FF;
    z80->af |= arith_inc(z80, a) << 8;
    z80->elapsed_cycles = 4;
}

// Compare Instructions =====
void CP_FE(struct Z80 *z80) {
    uint8_t a = z80->af >> 8;
    uint8_t n = fetch_byte(z80);
    uint8_t f = 0b01000000 | (a - n == 0) << FLAG_Z | (a - n < 0) << FLAG_C | ((a & 0xF) - (n & 0xF) < 0) << FLAG_H;
    z80->af &= 0xFF00;
    z80->af |= f;
    z80->elapsed_cycles = 8;
}

// JUMP Instructions ====

void JP_C3(struct Z80 *z80) { // JP nn
    z80->pc = fetch_word(z80);
    z80->elapsed_cycles = 16;
}

void JP_E9(struct Z80 *z80) { // JP HL
    z80->pc = z80->hl;
    z80->elapsed_cycles = 4;
}


void JR_18(struct Z80 *z80) { // JR s8
    int8_t s = fetch_byte(z80);
    z80->pc += s;
    z80->elapsed_cycles = 12;
}

void JR_20(struct Z80 *z80) { // JR NZ, s8
    int8_t s = fetch_byte(z80);

    if ((z80->af & (1 << FLAG_Z)) == 0) {
        z80->pc += s;
        z80->elapsed_cycles = 12;
    } else {
        z80->elapsed_cycles = 8;
    }
}

void JR_28(struct Z80 *z80) { // JR Z, s8
    int8_t s = fetch_byte(z80);

    if ((z80->af & (1 << FLAG_Z)) != 0) {
        z80->pc += s;
        z80->elapsed_cycles = 12;
    } else {
        z80->elapsed_cycles = 8;
    }
}

void JR_30(struct Z80 *z80) { // JR NC, s8
    int8_t s = fetch_byte(z80);

    if ((z80->af & (1 << FLAG_C)) == 0) {
        z80->pc += s;
        z80->elapsed_cycles = 12;
    } else {
        z80->elapsed_cycles = 8;
    }
}


// LOAD Instructions =====

void LD_01(struct Z80 *z80) { // LD BC, d16
    z80->bc = fetch_word(z80);
    z80->elapsed_cycles = 12;
}

void LD_11(struct Z80 *z80) { // LD DE, nn
    z80->de = fetch_word(z80);
    z80->elapsed_cycles = 12;
}

void LD_12(struct Z80 *z80) { // LD (DE), A
    write_byte(z80, z80->de, z80->af >> 8);
    z80->elapsed_cycles = 8;
}

void LD_1A(struct Z80 *z80) { // LD A, (DE)
    z80->af &= 0x00FF;
    z80->af |= address_byte(z80, z80->de);
    z80->elapsed_cycles = 8;
}

void LD_21(struct Z80 *z80) { // LD HL, nn
    z80->hl = fetch_word(z80);
    z80->elapsed_cycles = 12;
}

void LD_22(struct Z80 *z80) { // LD (HL+), A
    write_byte(z80, z80->hl, z80->af >> 8);
    z80->hl++;
    z80->elapsed_cycles = 8;
}

void LD_26(struct Z80 *z80) { // LD H, d8
    z80->hl &= 0x00FF;
    z80->hl |= fetch_byte(z80) << 8;
    z80->elapsed_cycles = 12;
}

void LD_2A(struct Z80 *z80) { // LD A, (HL++)
    uint8_t n = address_byte(z80, z80->hl);

    z80->hl++;
    z80->af &= 0x00FF;
    z80->af |= n << 8;

    z80->elapsed_cycles = 8;
}

void LD_31(struct Z80 *z80) { // LD SP, d16
    z80->sp = fetch_word(z80);
    z80->elapsed_cycles = 12;
}

void LD_32(struct Z80 *z80) { // LD (HL-), A
    write_byte(z80, z80->hl, z80->af >> 8);
    z80->hl--;
    z80->elapsed_cycles = 8;
}

void LD_3E(struct Z80 *z80) { // LD A, d8 
    z80->af &= 0x00FF;
    z80->af |= fetch_byte(z80) << 8;
    z80->elapsed_cycles = 8;
}

void LD_06(struct Z80 *z80) { // LD B d8
    uint8_t n = fetch_byte(z80);
    z80->bc &= 0x00FF;
    z80->bc |= n << 8;
    z80->elapsed_cycles = 8;
}

void LD_0E(struct Z80 *z80) { // LD C d8
    uint8_t n = fetch_byte(z80);
    z80->bc &= 0xFF00;
    z80->bc |= n;
    z80->elapsed_cycles = 8;
}

void LD_46(struct Z80 *z80) { // LD B, (HL)
    z80->bc &= 0x00FF;
    z80->bc |= address_byte(z80, z80->hl) << 8;
    z80->elapsed_cycles = 8;
}

void LD_47(struct Z80 *z80) { // LD B, A
    uint8_t a = z80->af >> 8;
    z80->bc &= 0x00FF;
    z80->bc |= a << 8;

    z80->elapsed_cycles = 4;
}

void LD_4E(struct Z80 *z80) { // LD C, (HL)
    z80->bc &= 0xFF00;
    z80->bc |= address_byte(z80, z80->hl);
    z80->elapsed_cycles = 8;
}

void LD_4F(struct Z80 *z80) { // LD C, A
    z80->bc &= 0xFF00;
    z80->bc |= z80->af >> 8;
    z80->elapsed_cycles = 4;
}

void LD_56(struct Z80 *z80) { // LD D, (HL)
    z80->de &= 0x00FF;
    z80->de |= address_byte(z80, z80->hl) << 8;
    z80->elapsed_cycles = 8;
}

void LD_57(struct Z80 *z80) { // LD D, A
    z80->de &= 0x00FF;
    z80->de |= z80->af & 0xFF00;
    z80->elapsed_cycles = 4;
}

void LD_5F(struct Z80 *z80) { // LD E, A
    z80->de &= 0xFF00;
    z80->de |= z80->af >> 8;
    z80->elapsed_cycles = 4;
}

void LD_6E(struct Z80 *z80) { // LD L, (HL)
    uint8_t n = address_byte(z80, z80->hl);
    z80->hl &= 0xFF00;
    z80->hl |= n;
    z80->elapsed_cycles = 8;
}

void LD_6F(struct Z80 *z80) { // LD L, A
    z80->hl &= 0xFF00;
    z80->hl |= z80->af >> 8;
    z80->elapsed_cycles = 1;
}

void LD_70(struct Z80 *z80) { // LD (HL), B
    write_byte(z80, z80->hl, z80->bc >> 8);
    z80->elapsed_cycles = 8;
}

void LD_71(struct Z80 *z80) { // LD (HL), C
    write_byte(z80, z80->hl, z80->bc & 0xFF);
    z80->elapsed_cycles = 8;
}

void LD_72(struct Z80 *z80) { // LD (HL), D
    write_byte(z80, z80->hl, z80->de >> 8);
    z80->elapsed_cycles = 8;
}

void LD_77(struct Z80 *z80) { // LD (HL), A
    write_byte(z80, z80->hl, z80->af >> 8);
    z80->elapsed_cycles = 8;
}

void LD_78(struct Z80 *z80) { // LD A, B
    z80->af &= 0x00FF;
    z80->af |= z80->bc & 0xFF00;
    z80->elapsed_cycles = 4;
}

void LD_79(struct Z80 *z80) { // LD A, C
    z80->af &= 0x00FF;
    z80->af |= (z80->bc & 0xFF) << 8;
    z80->elapsed_cycles = 4;
}

void LD_7A(struct Z80 *z80) { // LD A, D
    z80->af &= 0x00FF;
    z80->af |= z80->de & 0xFF00;
    z80->elapsed_cycles = 4;
}

void LD_7B(struct Z80 *z80) { // LD A, E
    z80->af &= 0x00FF;
    z80->af |= (z80->de & 0xFF) << 8;
    z80->elapsed_cycles = 4;
}

void LD_7C(struct Z80 *z80) { // LD A, H
    z80->af &= 0x00FF;
    z80->af |= z80->hl & 0xFF00;
    z80->elapsed_cycles = 4;
}

void LD_7D(struct Z80 *z80) { // LD A, L
    z80->af &= 0x00FF;
    z80->af |= (z80->hl & 0x00FF) << 8;
    z80->elapsed_cycles = 4;
}

void LD_E0(struct Z80 *z80) { // LD (a8), A
    write_byte(z80, 0xFF00 + fetch_byte(z80), z80->af >> 8);
    z80->elapsed_cycles = 12;
}

void LD_F0(struct Z80 *z80) { // LD A, a8
    z80->af &= 0x00FF;
    z80->af |= (address_byte(z80, 0xFF00 + fetch_byte(z80))) << 8;
    z80->elapsed_cycles = 12;
}

void LD_EA(struct Z80 *z80) { // LD (a16), A
    write_byte(z80, fetch_word(z80), z80->af >> 8);
    z80->elapsed_cycles = 16;
}

void LD_FA(struct Z80 *z80) { // LD A, (a16)
    z80->af &= 0x00FF;
    z80->af |= address_byte(z80, fetch_word(z80)) << 8;
    z80->elapsed_cycles = 16;
}

// Stack control flow =====

void CALL_C4(struct Z80 *z80) { // CALL NZ, a16
    uint16_t a = fetch_word(z80);
    if ((z80->af & (1 << FLAG_Z)) == 0) {
        push_word(z80, z80->pc);
        z80->pc = a;
        z80->elapsed_cycles = 24;
    } else {
        z80->elapsed_cycles = 12;
    }
}

void CALL_CD(struct Z80 *z80) { // CALL a16
    uint16_t a = fetch_word(z80);
    push_word(z80, z80->pc);
    z80->pc = a;
    z80->elapsed_cycles = 24;
}

void RET_C8(struct Z80 *z80) { // RET Z
    if ((z80->af & (1 << FLAG_Z)) != 0) {
        z80->pc = pop_word(z80);
        z80->elapsed_cycles = 20;
    }

    z80->elapsed_cycles = 8;
}

void RET_C9(struct Z80 *z80) { // RET
    z80->pc = pop_word(z80);
    z80->elapsed_cycles = 16;
}

void RET_D0(struct Z80 *z80) { // RET NC
    if ((z80->af & (1 << FLAG_C)) == 0) {
        z80->pc = pop_word(z80);
        z80->elapsed_cycles = 20;
    }

    z80->elapsed_cycles = 8;
}


// Register/Stack operations ====

void POP_C1(struct Z80 *z80) { // POP BC
    z80->bc = pop_word(z80);
    z80->elapsed_cycles = 16;
}

void PUSH_C5(struct Z80 *z80) { // PUSH BC
    push_word(z80, z80->bc);
    z80->elapsed_cycles = 16;
}

void PUSH_D5(struct Z80 *z80) { // PUSH DE
    push_word(z80, z80->de);
    z80->elapsed_cycles = 16;
}

void POP_D1(struct Z80 *z80) { // POP DE
    z80->de = pop_word(z80);
    z80->elapsed_cycles = 16;
}

void POP_E1(struct Z80 *z80) { // POP HL
    z80->hl = pop_word(z80);
    z80->elapsed_cycles = 16;
}

void PUSH_E5(struct Z80 *z80) { // PUSH HL
    push_word(z80, z80->hl);
    z80->elapsed_cycles = 16;
}

void POP_F1(struct Z80 *z80) { // POP AF
    z80->af = pop_word(z80);
    z80->elapsed_cycles = 16;
}

void PUSH_F5(struct Z80 *z80) { // PUSH AF
    push_word(z80, z80->af);
    z80->elapsed_cycles = 16;
}


// Interrupts =====

void DI_F3(struct Z80 *z80) { // DI (disable interrupts)
    z80->ime = false;
    z80->elapsed_cycles = 4;
}