#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "z80.h"
#include "unprefixed.h"
#include "cb_prefixed.h"

struct Z80 *init_z80_chip(void) {
    struct Z80 *z80 = malloc(sizeof(struct Z80));
    for (int i = 0; i < MEM_SIZE; i++) {
        z80->memory[i] = 0;
    }

    z80->af = 0x01B0;
    z80->bc = 0x0013;
    z80->de = 0x00D8;
    z80->hl = 0x014D;

    z80->pc = 0x0100;
    z80->sp = 0xFFFE;

    // z80->af = 0x0180;
    // z80->bc = 0x0013;
    // z80->de = 0x00D8;
    // z80->hl = 0x014D;

    // z80->pc = 0x0100;
    // z80->sp = 0xFFFE;

    z80->elapsed_cycles = 0;

    isa_populate_unprefixed(unprefixed);
    isa_populate_prefixed(prefixed);
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

uint8_t arith_add(struct Z80 *z80, uint8_t a, uint8_t b) {
    uint8_t n = a + b;

    z80->af &= 0xFF00;
    z80->af |= (n == 0) << FLAG_Z;
    z80->af |= (((a & 0xF) + (b & 0xF)) > 0xF) << FLAG_H;
    z80->af |= (a + b > 0xFF) << FLAG_C;

    return n;
}

uint16_t arith_add16(struct Z80 *z80, uint16_t a, uint16_t b) {
    uint16_t n = a + b;

    z80->af &= 0xFF80;
    z80->af |= (((a & 0xFFF) + (b & 0xFFF)) > 0xFFF) << FLAG_H;
    z80->af |= (a + b > 0xFFFF) << FLAG_C;

    return n;
}

uint8_t arith_sub(struct Z80 *z80, uint8_t a, uint8_t b) {
    uint8_t n = a - b;

    z80->af &= 0xFF00;
    z80->af |= (n == 0) << FLAG_Z;
    z80->af |= 1 << FLAG_N;
    z80->af |= (((a & 0xF) - (b & 0xF)) < 0) << FLAG_H;
    z80->af |= (a - b < 0) << FLAG_C;

    return n;
}


uint8_t arith_inc(struct Z80 *z80, uint8_t v) {
    uint8_t n = v + 1;

    z80->af &= 0xFF10;
    z80->af |= ((v & 0xF) == 0xF) << FLAG_H;
    z80->af |= (n == 0) << FLAG_Z;

    return n;
}

uint8_t arith_dec(struct Z80 *z80, uint8_t v) {
    uint8_t n = v - 1;

    z80->af &= 0xFF10;
    z80->af |= 1 << FLAG_N;
    z80->af |= ((v & 0xF) - 1 < 0) << FLAG_H;
    z80->af |= (n == 0) << FLAG_Z;

    return n;
}

uint8_t logical_srl(struct Z80 *z80, uint8_t n) {
    uint8_t carry = n & 1;
    uint8_t rot = n >> 1;

    z80->af &= 0xFF00;
    z80->af |= (rot == 0) << FLAG_Z  | (carry == 1) << FLAG_C;
    return rot;
}

uint8_t logical_rr(struct Z80 *z80, uint8_t n) {
    uint8_t carry = n & 1;
    uint8_t old_carry = (z80->af & (1 << FLAG_C)) >> FLAG_C;
    uint8_t rot = n >> 1 | old_carry << 7;

    z80->af &= 0xFF00;
    z80->af |= (rot == 0) << FLAG_Z  | (carry == 1) << FLAG_C;
    return rot;
}



void step_instruction(struct Z80 *z80) {
    uint8_t op_byte = fetch_byte(z80);

    if (op_byte == 0xcb) {
        uint8_t cb_op_byte = fetch_byte(z80);
        printf("%04x: cb %02x\n", z80->pc - 2, cb_op_byte);
        printf("af = %04x, bc = %04x\n", z80->af, z80->bc);
        printf("de = %04x, hl = %04x\n", z80->de, z80->hl);
        printf("sp = %04x, ime = %d\n", z80->sp, z80->ime);
        prefixed[cb_op_byte](z80);
    } else {
        printf("%04x: %02x\n", z80->pc - 1, op_byte);
        printf("af = %04x, bc = %04x\n", z80->af, z80->bc);
        printf("de = %04x, hl = %04x\n", z80->de, z80->hl);
        printf("sp = %04x, ime = %d\n", z80->sp, z80->ime);
        unprefixed[op_byte](z80); // unless op_byte = CB
    }
}
