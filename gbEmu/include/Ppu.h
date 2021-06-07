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
	#define OBP0 0xFF48
	#define OBP1 0xFF49

	#define WINDOWX 0xFF4B
	#define WINDOWY 0xFF4A
	#define OAM 0xFE00


	enum class PpuMode : u8 {
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
		void render(sf::RenderTarget& target);
		void update(s32 cycles);

		void drawLine();
		void drawTiles();
		void drawSprites();

		sf::Color getPixelColor(u8 colorNum, u8 palette, bool sprite = false);


		void setLCDStatus();
		bool isLCDEnabled();

		/*
			Takes in bit of interrupt to request
		*/
		void requestInterrupt(u8 interruptBit);
		void bufferPixels();

		sf::Image pixels;
		sf::Texture framebuffer;
		sf::Sprite sprite;

		sf::Color shadesOfGrey[4];
		sf::Color superGbShades[4];
		sf::Color customShades[4];
	
		s16 scanlineCounter = 456;
		s16 scanlinesRendered = 0;

		MMU* mmu;
		bool disablePpu = false;
	};
}