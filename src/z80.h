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
    bool ime;    

    uint8_t memory[MEM_SIZE];
    uint8_t elapsed_cycles;
};

struct Z80 *init_z80_chip(void);

uint8_t fetch_byte(struct Z80 *z80);
uint16_t fetch_word(struct Z80 *z80); // little endian (reverse byte)
uint8_t address_byte(struct Z80 *z80, uint16_t address);
void write_byte(struct Z80 *z80, u_int16_t address, uint8_t value);
void load_rom(struct Z80 *z80, const char *rom_filename);

void step_instruction(struct Z80 *z80);

uint8_t arith_inc(struct Z80 *z80, uint8_t v);
uint8_t arith_dec(struct Z80 *z80, uint8_t v);

static void (*unprefixed[0xFF])(struct Z80 *z80);
static void (*prefixed[0xFF])(struct Z80 *z80);
void populate_instruction_set();

void NOP(struct Z80 *z80);
void UNDEFINED(struct Z80 *z80);

void XOR_AF(struct Z80 *z80);

void DEC_0D(struct Z80 *z80);

void INC_03(struct Z80 *z80);
void INC_14(struct Z80 *z80);
void INC_1C(struct Z80 *z80);
void INC_23(struct Z80 *z80);

void LD_0E(struct Z80 *z80);

void JR_18(struct Z80 *z80);
void JR_20(struct Z80 *z80);

void JP_C3(struct Z80 *z80);

void LD_01(struct Z80 *z80);
void LD_11(struct Z80 *z80);
void LD_12(struct Z80 *z80);
void LD_21(struct Z80 *z80);
void LD_21(struct Z80 *z80);
void LD_2A(struct Z80 *z80);
void LD_31(struct Z80 *z80);
void LD_3E(struct Z80 *z80);
void LD_47(struct Z80 *z80);
void LD_78(struct Z80 *z80);
void LD_7C(struct Z80 *z80);
void LD_7D(struct Z80 *z80);
void LD_E0(struct Z80 *z80);
void LD_EA(struct Z80 *z80);

void DI_F3(struct Z80 *z80);

void CALL_CD(struct Z80 *z80);
void RET_C9(struct Z80 *z80);

void POP_C1(struct Z80 *z80);
void POP_E1(struct Z80 *z80);
void POP_F1(struct Z80 *z80);

void PUSH_C5(struct Z80 *z80);
void PUSH_E5(struct Z80 *z80);
void PUSH_F5(struct Z80 *z80);

#endif