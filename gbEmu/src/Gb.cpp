#include "../include/Gb.h"

namespace gbEmu {
    
    Gb::Gb()
        :mmu(), cpu(&mmu), ppu(&mmu), joypad(&mmu)
    {
        mmu.connectJoypad(&joypad);
        
        //cart.load("roms/Kirby's Dream Land.gb");
        //cart.load("roms/Pokemon Red.gb");
        //cart.load("roms/SUPERMAR.gbc");
        //cart.load("roms/Dr. Mario.gb");
        //cart.load("roms/Tetris.gb");
         //cart.load("roms/ZELDA.gbc");
        //cart.load("roms/fairylake.gb");
        //cart.load("roms/Tennis.gb");

        //Tests
        //cart.load("test_roms/02-interrupts.gb");
         cart.load("test_roms/ppu/dmg-acid2.gb");
        //cart.load("test_roms/ppu/bg_m9800_d8800.gb");
        //cart.load("test_roms/numism.gb");
        //cart.load("test_roms/statcount.gb");

        //cart.load("test_roms/cpu_instrs.gb");
        //cart.load("test_roms/mbc/mbc1/bits_mode.gb");
        //cart.load("test_roms/instr_timing.gb");
        //cart.load("test_roms/mem_timing.gb");

        mmu.loadBios("roms/DMG_ROM.GB");
        mmu.loadCartridge(&cart);

        ppu.init();
    }

    void Gb::update(float dt)
    {
        s32 cycles_this_frame = 0;
        //each scanline takes 456 t cycles.
        //There are 154 scanlines per frame.
        //(456 * 154) = 70224(maxCycles)
        while (cycles_this_frame < maxCycles) {
            s32 cycle = cpu.clock();
            cycles_this_frame += cycle;

            cpu.handleTimer(cycle);
            ppu.update(cycles_this_frame);
            cpu.handleInterrupts();
        }
        ppu.bufferPixels();

      /*  if (dt < delayTime) {
            std::this_thread::sleep_for(std::chrono::duration<float,
                std::milli>(delayTime - dt));
        }*/
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
