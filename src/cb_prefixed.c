#include <stdio.h>
#include "cb_prefixed.h"
#include "z80.h"

void isa_populate_prefixed(void (*prefixed[0xFF])(struct Z80 *z80)) {
    for (int i = 0; i < 0xFF; i++) {
        prefixed[i] = CB_UNDEFINED;
    }

    prefixed[0x19] = RR_19;
    prefixed[0x1A] = RR_1A;

    prefixed[0x38] = SRL_38;
}

void CB_UNDEFINED(struct Z80 *z80) {
    printf("0xcb%02x\n", z80->memory[z80->pc-1]);
    z80->is_running_flag = false;
    z80->elapsed_cycles = 4;
}

// RR Instructions =====

void RR_19(struct Z80 *z80) { // RR C
    uint8_t c = z80->bc & 0xFF;
    z80->bc &= 0xFF00;
    z80->bc |= logical_rr(z80, c);
    z80->elapsed_cycles = 8;
}

void RR_1A(struct Z80 *z80) { // RR D
    uint8_t d = z80->de >> 8;
    z80->de &= 0x00FF;
    z80->de |= logical_rr(z80, d) << 8;
    z80->elapsed_cycles = 8;
}

// SRL Instructions =====

void SRL_38(struct Z80 *z80) { // SRL B
    uint8_t b = z80->bc >> 8;
    z80->bc &= 0x00FF;
    z80->bc |= logical_srl(z80, b) << 8;
    z80->elapsed_cycles = 8;
}