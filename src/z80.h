#ifndef Z80_H
#define Z80_H

#include "stdint.h"
#include "stdbool.h"

#define MEM_SIZE 0xFFFF
#define ROM_SIZE 0x8000

#define FLAG_C 4
#define FLAG_H 5
#define FLAG_N 6
#define FLAG_Z 7

struct Z80 {
    uint16_t af;

    uint16_t bc;
    uint16_t de;
    uint16_t hl;

    uint16_t pc;
    uint16_t sp;

    bool is_running_flag;    
    bool is_stepping;    
    bool ime;    

    uint8_t memory[MEM_SIZE];
    uint8_t elapsed_cycles;
};

struct Z80 *init_z80_chip(void);

uint8_t fetch_byte(struct Z80 *z80);
uint16_t fetch_word(struct Z80 *z80); // little endian (reverse byte)

uint8_t address_byte(struct Z80 *z80, uint16_t address);
void write_byte(struct Z80 *z80, u_int16_t address, uint8_t value);

void push_word(struct Z80 *z80, uint16_t word);
uint16_t pop_word(struct Z80 *z80);

uint8_t arith_add(struct Z80 *z80, uint8_t a, uint8_t b);
uint8_t arith_sub(struct Z80 *z80, uint8_t a, uint8_t b);
uint8_t arith_inc(struct Z80 *z80, uint8_t v);
uint8_t arith_dec(struct Z80 *z80, uint8_t v);
uint8_t logical_srl(struct Z80 *z80, uint8_t n);
uint8_t logical_rr(struct Z80 *z80, uint8_t n);

void load_rom(struct Z80 *z80, const char *rom_filename);
void step_instruction(struct Z80 *z80);

static void (*prefixed[0xFF])(struct Z80 *z80);
static void (*unprefixed[0xFF])(struct Z80 *z80);

#endif