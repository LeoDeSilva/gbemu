#include <stdio.h>
#include "SDL.h"
#include "z80.h"
#include "screen.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Expected ROM filename argument");
        return 0;
    }

    char* rom_filename = argv[1];

    struct Z80 *z80 = init_z80_chip();
    if (SDL_init()) {
        printf("Failed to initialise SDL\n");
        return 0;
    }


    load_rom(z80, rom_filename);

    z80->is_running_flag = true;
    z80->is_stepping = false;
    while (z80->is_running_flag) {
        SDL_Event e;
        while (SDL_PollEvent(&e) > 0) {
            switch(e.type) {
                case SDL_QUIT:
                    z80->is_running_flag = 0;
                    break;
            } 

            SDL_update();
        }

        if (z80->elapsed_cycles == 0) {
            // if (z80->pc == 0x020c)
            //     z80->is_stepping = true;

            step_instruction(z80);
            /*Flush input stream in case \n left out*/
            if (z80->is_stepping) {
                char ch = getchar();
                if (ch == 'r')
                    z80->is_stepping = false;
            }
        }

        z80->elapsed_cycles--;
        if (z80->memory[0xff02] == 0x81) {
            char c = z80->memory[0xff01];
            printf("%c", c);
            z80->memory[0xff02] = 0x0;
        }
    }

    SDL_close();
    return 0;
}