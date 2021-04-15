#pragma once
#include <SFML\Graphics.hpp>
#include "Utils.h"

/*
	Pixel Processing Unit (PPU)
	*160 x 144 pixel LCD
	*4 different shades of grey
	*2 bits for a single pixel(2BPP)
*/

namespace gbEmu {
	struct MMU;
	struct Ppu {
		Ppu(MMU* mmu);

		u8 read(u16 address);
		void write(u16 address, u8 value);



		void init();
		void update(u32 cycles);
		void draw(sf::RenderTarget &target);

		void drawBackground();





		void setLCDStatus();
		bool isLCDEnabled();

		/*
			Takes in bit of interrupt to request
		*/
		void requestInterrupt(u8 interruptBit);

		sf::Image image;
		sf::Texture texture;
		sf::Sprite sprite;
		

		/*
			One framebuffer for background layer
			and one for window layer

			*main framebuffer is 160 x 144 pixels
			*The background map is a larger size of 256 x 256
		*/
		u8 framebuf[160 * 144 * 3]; //3 bytes per pixel, RGB24
		u8 framebufA[160 * 144 * 4]; //4 bytes per pixel, RGBA24
		u8 backgroundMapA[256 * 256 * 4];

		s16 scanlineCounter = 0;

		MMU* mmu;
		bool disablePpu = false;
	};
}