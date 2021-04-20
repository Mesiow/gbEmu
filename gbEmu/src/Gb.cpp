#include "../include/Gb.h"

gbEmu::Gb::Gb()
	:mmu(), cpu(&mmu), ppu(&mmu), joypad(&mmu)
{
    cart.load("roms/Tetris.GB");

	mmu.loadBios("roms/DMG_ROM.GB");
    mmu.loadCartridge(&cart);
  
	ppu.init();

    view = sf::Sprite(ppu.texture);
    view.setScale(3.5, 3.5);
}

void gbEmu::Gb::update()
{
    s32 cycles_this_frame = 0;
    static s32 delta_cycles = 0;
    //each scanline takes 456 t cycles. There are 154 scanlines per frame
    //(456 * 154) = 70224
    while (cycles_this_frame < 70224) {
        cycles_this_frame += cpu.clock();
        ppu.update(cycles_this_frame);
    }
    delta_cycles += cycles_this_frame - 70224;
    ppu.bufferPixels();
}

void gbEmu::Gb::render(sf::RenderTarget& target)
{
    target.draw(view);
}
