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
	//Ppu register locations
	#define LY 0xFF44
	#define LCDC 0xFF40
	#define STAT 0xFF41
	#define SCX 0xFF43
	#define SCY 0xFF42
	#define COINCIDENCE 0xFF45
	#define PALETTE 0xFF47

	#define WINDOWX 0xFF4B
	#define WINDOWY 0xFF4A


	enum class PpuMode {
		HBlank = 0,
		VBlank,
		OAMScan,
		Drawing
	};


	struct MMU;
	struct Ppu {
		Ppu(MMU* mmu);

		u8 read(u16 address);
		void write(u16 address, u8 value);



		void init();
		void update(u32 cycles);

		void drawLine();
		void drawTiles();

		sf::Color getColor(u8 val, u8 pal);





		void setLCDStatus();
		bool isLCDEnabled();

		/*
			Takes in bit of interrupt to request
		*/
		void requestInterrupt(u8 interruptBit);
		void bufferPixels();

		sf::Image image;
		sf::Texture texture;
		

		/*
			One framebuffer for background layer
			and one for window layer

			*main framebuffer is 160 x 144 pixels
			*The background map is a larger size of 256 x 256
		//*/
		//u8 framebuf[160 * 144 * 3]; //3 bytes per pixel, RGB24
		//u8 framebufA[160 * 144 * 4]; //4 bytes per pixel, RGBA24
		//u8 backgroundMapA[256 * 256 * 4];

		s16 scanlineCounter = 0;

		MMU* mmu;
		bool disablePpu = false;
	};
}