#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "z80.h"

struct Z80 *init_z80_chip(void) {
    struct Z80 *z80 = malloc(sizeof(struct Z80));
    for (int i = 0; i < MEM_SIZE; i++) {
        z80->memory[i] = 0;
    }


    z80->af = 0x0180;
    z80->bc = 0x0013;
    z80->de = 0x00D8;
    z80->hl = 0x014D;

    z80->pc = 0x0100;
    z80->sp = 0xFFFE;

    z80->elapsed_cycles = 0;

    populate_instruction_set();
    return z80;
}

void populate_instruction_set() {
    for (int i = 0; i < 0xFF; i++) {
        unprefixed[i] = UNDEFINED;
    }

    unprefixed[0x00] = NOP;
    unprefixed[0x01] = LD_01;
}


uint8_t fetch_byte(struct Z80 *z80) {
    return z80->memory[z80->pc++];
}

uint16_t fetch_word(struct Z80 *z80) {
    uint8_t lsb = fetch_byte(z80);
    uint8_t msb = fetch_byte(z80);
    return (msb << 8) | lsb;
}

void push_word(struct Z80 *z80, uint16_t word) {
    z80->memory[z80->sp-1] = word >> 8;
    z80->memory[z80->sp-2] = word & 0xFF;
    z80->sp -= 2;
}

uint16_t pop_word(struct Z80 *z80) {
    //sp points to the low byte initially
    uint16_t word = (z80->memory[z80->sp + 1] << 8) | z80->memory[z80->sp];
    z80->sp += 2;
    return word;
}

uint8_t address_byte(struct Z80 *z80, uint16_t address) {
    return z80->memory[address];
}

// enforce protected memory
void write_byte(struct Z80 *z80, u_int16_t address, uint8_t value) { 
    if (address < 0x8000)
        return;

    z80->memory[address] = value;
}

void load_rom(struct Z80 *z80, const char *rom_filename) {
    long romsize;
    FILE *fileptr = fopen(rom_filename, "rb");
    fseek(fileptr, 0, SEEK_END);
    romsize = ftell(fileptr);
    rewind(fileptr);

    uint8_t *buffer = (uint8_t *)malloc(romsize * sizeof(uint8_t));
    fread(buffer, romsize, 1, fileptr);
    fclose(fileptr);

    memcpy(z80->memory, buffer, ROM_SIZE);
}

uint8_t arith_inc(struct Z80 *z80, uint8_t v) {
    uint8_t n = v + 1;

    z80->af &= 0xFF10;
    z80->af |= (v == 0xF) << FLAG_H;
    z80->af |= (n == 0) << FLAG_Z;

    return n;
}

uint8_t arith_dec(struct Z80 *z80, uint8_t v) {
    uint8_t n = v - 1;

    z80->af &= 0xFF00;
    z80->af |= 1 << FLAG_N;
    z80->af |= (v == 0) << FLAG_H;
    z80->af |= (n == 0) << FLAG_Z;

    return n;
}

void step_instruction(struct Z80 *z80) {
    uint8_t op_byte = fetch_byte(z80);
    switch (op_byte) {
        case 0x00: // NOP
            z80->elapsed_cycles = 4;
            break;

        case 0x01: // NOP
            LD_01(z80);
            break;

        case 0x03: // INC BC
            INC_03(z80);
            break;

        case 0x0D: // DEC C
            DEC_0D(z80);
            break;

        case 0x0E: // LD C n
            LD_0E(z80);
            break;

        case 0x11: // LD DE, n16
            LD_11(z80);
            break;

        case 0x12: // LD (DE), A
            LD_12(z80);
            break;

        case 0x14: // INC D
            INC_14(z80);
            break;

        case 0x1C: // INC E
            INC_1C(z80);
            break;

        case 0x18: // JR s8
            JR_18(z80);
            break;

        case 0x20: // JR, NZ, s8
            JR_20(z80);
            break;

        case 0x21: // LD HL, nn
            LD_21(z80);
            break;

        case 0x23: // INC HL
            INC_23(z80);
            break;

        case 0x2A: // LD HL, nn
            LD_2A(z80);
            break;

        case 0x31: // LD SP, d16
            LD_31(z80);
            break;

        case 0x3e: // LD A, d8
            LD_3E(z80);
            break;

        case 0x47: // LD B, A
            LD_47(z80);
            break;
        
        case 0x78: // LD A, B
            LD_78(z80);
            break;

        case 0x7C: // LD A, H
            LD_7C(z80);
            break;

        case 0x7D: // LD A, L
            LD_7D(z80);
            break;

        case 0xAF: // XOR A, A
            XOR_AF(z80);
            break;

        case 0xC3: // JP nn
            JP_C3(z80);
            break;

        case 0xC1: // POP BC
            POP_C1(z80);
            break;

        case 0xC5: // PUSH BC
            PUSH_C5(z80);
            break;

        case 0xCD: // CALL a16
            CALL_CD(z80);
            break;

        case 0xC9: // RET 
            RET_C9(z80);
            break;

        case 0xE0: // LD (a8), A
            LD_E0(z80);
            break;

        case 0xE1: // POP HL
            POP_E1(z80);
            break;

        case 0xE5: // PUSH HL
            PUSH_E5(z80);
            break;

        case 0xF1: // POP AF
            POP_F1(z80);
            break;

        case 0xF5: // PUSH AF
            PUSH_F5(z80);
            break;

        case 0xEA: // LD (a16), A
            LD_EA(z80);
            break;

        case 0xF3: // DI
            DI_F3(z80);
            break;

        default:
            printf("%02x\n", op_byte);
            z80->is_running_flag = false;
    }
}

void NOP(struct Z80 *z80) {
    z80->elapsed_cycles = 4;
}

void UNDEFINED(struct Z80 *z80) {
    printf("%02x\n", z80->memory[z80->pc-1]);
    z80->is_running_flag = false;
    z80->elapsed_cycles = 4;
}

void JP_C3(struct Z80 *z80) { // JP nn
    z80->pc = fetch_word(z80);
    z80->elapsed_cycles = 16;
}

void XOR_AF(struct Z80 *z80) { // XOR A, A
    uint8_t n = z80->af >> 8;

    z80->af &= 0x0000;
    z80->af |= ((n^n) << 8) | 1 << FLAG_Z;
    z80->elapsed_cycles = 4;
}

void DEC_0D(struct Z80 *z80) { // INC C 
    z80->bc = (z80->bc & 0xFF00) | arith_dec(z80, z80->bc & 0xFF);
    z80->elapsed_cycles = 4;
}

void INC_14(struct Z80 *z80) { // INC D
    z80->de = (z80->de & 0x00FF) | (arith_inc(z80, z80->de >> 8) << 8);
    z80->elapsed_cycles = 4;
}

void INC_1C(struct Z80 *z80) { // INC E 
    z80->de = (z80->de & 0xFF00) | arith_inc(z80, z80->de & 0xFF);
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

// LOAD Instructions

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


void LD_21(struct Z80 *z80) { // LD HL, nn
    z80->hl = fetch_word(z80);
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

void LD_3E(struct Z80 *z80) { // LD A, d8 
    z80->af &= 0x00FF;
    z80->af |= fetch_byte(z80) << 8;
    z80->elapsed_cycles = 8;
}

void LD_0E(struct Z80 *z80) { // LD C n
    uint8_t n = fetch_byte(z80);
    z80->bc &= 0xFF00;
    z80->bc |= n;
    z80->elapsed_cycles = 8;
}

void LD_47(struct Z80 *z80) { // LD B, A
    uint8_t a = z80->af >> 8;
    z80->bc &= 0x00FF;
    z80->bc |= a << 8;

    z80->elapsed_cycles = 4;
}

void LD_78(struct Z80 *z80) { // LD A, B
    z80->af &= 0x00FF;
    z80->af |= z80->bc & 0xFF00;
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

void LD_EA(struct Z80 *z80) { // LD (a16), A
    write_byte(z80, fetch_word(z80), z80->af >> 8);
    z80->elapsed_cycles = 16;
}

void DI_F3(struct Z80 *z80) { // DI (disable interrupts)
    z80->ime = false;
    z80->elapsed_cycles = 4;
}

void CALL_CD(struct Z80 *z80) { // CALL a16
    uint16_t a = fetch_word(z80);
    push_word(z80, z80->pc);
    z80->pc = a;
    z80->elapsed_cycles = 24;
}

void RET_C9(struct Z80 *z80) { // RET
    z80->pc = pop_word(z80);
    z80->elapsed_cycles = 16;
}

// Register/Stack operations

void POP_C1(struct Z80 *z80) { // POP BC
    z80->bc = pop_word(z80);
    printf("poped bc: %04x\n", z80->bc);
    z80->elapsed_cycles = 16;
}

void PUSH_C5(struct Z80 *z80) { // PUSH BC
    printf("pushed bc: %04x\n", z80->bc);
    push_word(z80, z80->bc);
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