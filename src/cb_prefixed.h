#ifndef CB_PREFIXED_H
#define CB_PREFIXED_H

#include "z80.h"

void isa_populate_prefixed(void (*prefixed[0xFF])(struct Z80 *z80));

void CB_UNDEFINED(struct Z80 *z80);

void RR_19(struct Z80 *z80);
void RR_1A(struct Z80 *z80);

void SRL_38(struct Z80 *z80);

#endif