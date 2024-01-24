# GBEMU (Gameboy Emulator)

* [x] initialise z80 architecture struct
* [x] load ROMS into defined memory address
* [ ] implement z80->inc() function handling register setting

AF (accumulator + flags)
BC
DE
HL
SP
PC

## Flags Register (F)
* Zero (set when the result of the previous operation was 0)
* Subtraction (set when the previous operation was a subtraction - else unset)
* Half Carry ()
* Carry

* 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
* 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
* 8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
* A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
* C000-CFFF   4KB Work RAM Bank 0 (WRAM)
* D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
* E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
* FE00-FE9F   Sprite Attribute Table (OAM)
* FEA0-FEFF   Not Usable
* FF00-FF7F   I/O Ports
* FF80-FFFE   High RAM (HRAM)
* FFFF        Interrupt Enable Register

Split instructions into smaller steps each of which take 4 clock cycles to execute
```
{ 0xC3, new Opcode(0xC3, "JP ${0:x4}", 3, 16, 
            new Step[] {
                // 4 clock cycles
                () => { lsb = mmu.Read8(regs.PC++); }, 
                // 4 clock cycles
                () => { msb = mmu.Read8(regs.PC++); }, 
                // 4 clock cycles
                () => { regs.PC = BitUtils.ToUnsigned16(msb, lsb);
            } })},
```

* [ ] Interrupts

Let’s take a look at an example. If the timer needs to trigger an interrupt, it has to set IF’s bit 2 to one. As part of the CPU’s execution loop, there’s a step to check if there’s an interrupt pending. But in order for the CPU to handle it, IME must be true and IE’s bit 2 must be turned on as well. If all the conditions are true then the following will happen:

IME will be set to false and the corresponding IF bit will be set to zero.
The current PC value will be pushed to the stack.
PC will be set to a memory address which contains the interrupt handling routine.
Once the handling routine is done, PC will be restored and execution will continue.
IME will be enabled again.

```
public List<u16> ISR_Address = new List<u16> {
        { 0x0040 },    // Vblank
        { 0x0048 },    // LCD Status
        { 0x0050 },    // TimerOverflow
        { 0x0058 },    // SerialLink
        { 0x0060 } };  // JoypadPress,
```

DIV (0xFF04): the divider register increments at a fixed rate of 16,384 Hz. This means that we have to increment DIV’s value every 256 clock cycles (4,194,304 / 16,384 = 256).

TIMA (0xFF05): the timer register increments at a configurable frequency. In this case, we have to update it every 16, 64, 256 or 1024 clock cycles depending on the frequency set in the TAC register (see how the previous clock cycle value was calculated). When TIMA overflows an interrupt is triggered and it’s value is reset to TMA’s value. TIMA should only be counting if the timer is enabled in the TAC register.

TMA (0xFF06): this value is used when TIMA overflows.

TAC (0xFF07): Timer Control, it has the following structure:

```
// Timer Control 
// Bits 1-0 - Input Clock Select
//            00: 4096   Hz 
//            01: 262144 Hz
//            10: 65536  Hz
//            11: 16384  Hz
// Bit  2   - Timer Enable
// 
// Note: The "Timer Enable" bit only affects TIMA, 
// DIV is ALWAYS counting.
```

PROTECT MEMORY WRITE BELOW 0x8000
SERIAL LINK

FLAGC 4
FLAGH 5
FLAGN 6
FLAGZ 7