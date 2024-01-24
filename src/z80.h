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
    uint8_t memory[MEM_SIZE];
    uint8_t elapsed_cycles;
};

struct Z80 *init_z80_chip(void);

uint8_t fetch_byte(struct Z80 *z80);
uint8_t address_byte(struct Z80 *z80, uint16_t address);
void write_byte(struct Z80 *z80, u_int16_t address, uint8_t value);
void load_rom(struct Z80 *z80, const char *rom_filename);

void step_instruction(struct Z80 *z80);

uint8_t airth_inc(struct Z80 *z80, uint8_t v);

void XOR_AF(struct Z80 *z80);

void INC_14(struct Z80 *z80);
void INC_1C(struct Z80 *z80);

void LD_0E(struct Z80 *z80);

void JR_20(struct Z80 *z80);
void JP_C3(struct Z80 *z80);

void LD_11(struct Z80 *z80);
void LD_12(struct Z80 *z80);
void LD_21(struct Z80 *z80);
void LD_21(struct Z80 *z80);
void LD_2A(struct Z80 *z80);
void LD_47(struct Z80 *z80);