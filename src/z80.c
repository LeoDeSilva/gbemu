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

    return z80;
}

uint8_t fetch_byte(struct Z80 *z80) {
    return z80->memory[z80->pc++];
}

uint16_t fetch_word(struct Z80 *z80) {
    uint8_t lsb = fetch_byte(z80);
    uint8_t msb = fetch_byte(z80);
    return (msb << 8) | lsb;
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

        case 0x20: // JR, NZ, s8
            JR_20(z80);
            break;

        case 0x21: // LD HL, nn
            LD_21(z80);
            break;

        case 0x2A: // LD HL, nn
            LD_2A(z80);
            break;

        case 0x47: // LD B, A
            LD_47(z80);
            break;

        case 0xAF: // XOR A, A
            XOR_AF(z80);
            break;

        case 0xC3: // JP nn
            JP_C3(z80);
            break;

        default:
            printf("%02x\n", op_byte);
            z80->is_running_flag = false;
    }
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

void LD_11(struct Z80 *z80) { // LD DE, nn
    z80->de = fetch_word(z80);
    z80->elapsed_cycles = 12;
}

void LD_12(struct Z80 *z80) { // LD (DE), A
    write_byte(z80, z80->de, z80->af >> 8);
    z80->elapsed_cycles = 8;
}

void DEC_0D(struct Z80 *z80) { // INC C 
    z80->bc = (z80->bc & 0xFF00) | arith_dec(z80, z80->bc & 0xFF);
    z80->elapsed_cycles = 4;
}

void INC_14(struct Z80 *z80) { // INC D
    z80->de = (z80->de & 0x00FF) | (arith_inc(z80, z80->de >> 8) << 8);
    z80->elapsed_cycles = 4;
}

// void INC_1C(struct Z80 *z80) { // INC E
//     uint8_t v = (z80->de & 0xFF) + 1;

//     z80->af &= 0xFF10;
//     z80->af |= (((z80->de & 0xF) + 1) > 0xF) << FLAG_H;

//     z80->de &= 0xFF00;
//     z80->de |= v;
//     z80->af |= ((z80->de & 0xFF) == 0) << FLAG_Z;

//     z80->elapsed_cycles = 4;
// }

void INC_1C(struct Z80 *z80) { // INC E 
    z80->de = (z80->de & 0xFF00) | arith_inc(z80, z80->de & 0xFF);
    z80->elapsed_cycles = 4;
}

void JR_20(struct Z80 *z80) { // JR NZ, s8
    int s = ((fetch_byte(z80) ^ 0x80) - 0x80); // convert byte in to 2s compliment offset

    if ((z80->af & (1 << FLAG_Z)) == 0) {
        z80->pc += s;
        z80->elapsed_cycles = 12;
    } else {
        z80->elapsed_cycles = 8;
    }
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