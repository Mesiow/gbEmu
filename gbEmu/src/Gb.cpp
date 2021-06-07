#include "../include/Gb.h"

namespace gbEmu {
    
    Gb::Gb()
        :mmu(), cpu(&mmu), ppu(&mmu), joypad(&mmu)
    {
        mmu.connectJoypad(&joypad);
        
        //cart.load("roms/Kirby's Dream Land.gb");
        //cart.load("roms/Pokemon Red.gb");
        //cart.load("roms/SUPERMAR.gbc");
        cart.load("roms/Dr. Mario.gb");
        //cart.load("roms/Tetris.gb");
        //cart.load("roms/ZELDA.gbc");
        //cart.load("test_roms/02-interrupts.gb");
        //cart.load("test_roms/ppu/dmg-acid2.gb");
        //cart.load("test_roms/cpu_instrs.gb");
        //cart.load("test_roms/mbc/mbc1/bits_mode.gb");
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

    void Gb::handleEvents(sf::Event& ev)
    {
        handleKeyReleased(ev);
        handleKeyPressed(ev);
    }

    void Gb::handleKeyReleased(sf::Event& ev)
    {
        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::Right) {
                joypad.keyReleased(Key::ArrowRight);
            }
        }
        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::Left) {
                joypad.keyReleased(Key::ArrowLeft);
            }
        }
        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::Up) {
                joypad.keyReleased(Key::ArrowUp);
            }
        }

        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::Down) {
                joypad.keyReleased(Key::ArrowDown);
            }
        }

        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::S) {
                joypad.keyReleased(Key::S);
            }
        }

        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::A) {
                joypad.keyReleased(Key::A);
            }
        }

        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::LShift) {
                joypad.keyReleased(Key::LShift);
            }
        }

        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::Enter) {
                joypad.keyReleased(Key::Enter);
            }
        }
    }

    void Gb::handleKeyPressed(sf::Event& ev)
    {
        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Right) {
                joypad.keyPressed(Key::ArrowRight);
            }
        }
        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Left) {
                joypad.keyPressed(Key::ArrowLeft);
            }
        }
        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Up) {
                joypad.keyPressed(Key::ArrowUp);
            }
        }

        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Down) {
                joypad.keyPressed(Key::ArrowDown);
            }
        }

        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::S) {
                joypad.keyPressed(Key::S);
            }
        }

        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::A) {
                joypad.keyPressed(Key::A);
            }
        }

        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::LShift) {
                joypad.keyPressed(Key::LShift);
            }
        }

        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Enter) {
                joypad.keyPressed(Key::Enter);
            }
        }
    }
}
