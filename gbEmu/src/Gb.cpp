#include "../include/Gb.h"

namespace gbEmu {
    
    Gb::Gb()
        :mmu(), cpu(&mmu), ppu(&mmu), joypad(&mmu)
    {
        //cart.load("roms/SUPERMAR.GBC");
        //cart.load("roms/Dr. Mario.gb");
        cart.load("roms/Tetris.GB");
        //cart.load("roms/ZELDA.gbc");
        //cart.load("test_roms/02-interrupts.gb");
        mmu.loadBios("roms/DMG_ROM.GB");
        mmu.loadCartridge(&cart);

        ppu.init();
    }

    void Gb::update()
    {
        s32 cycles_this_frame = 0;
        static s32 delta_cycles = 0;
        //each scanline takes 456 t cycles. There are 154 scanlines per frame
        //(456 * 154) = 70224
        while (cycles_this_frame < 70224) {
            u32 cycle = cpu.clock();
            cycles_this_frame += cycle;

            cpu.handleTimer(cycle);
            ppu.update(cycles_this_frame);
            cpu.handleInterrupts();
        }
        delta_cycles += cycles_this_frame - 70224;
        ppu.bufferPixels();
    }

    void Gb::render(sf::RenderTarget& target)
    {
        ppu.render(target);
    }
}
