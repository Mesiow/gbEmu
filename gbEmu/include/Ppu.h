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
		void update(u32 cycles);

		void drawLine();
		void drawBackground();
		void drawWindow();
		void drawSprites();

		void updateBackgroundTilePx(u8 palette, s32 displayX, s32 displayY, s32 tileX, s32 tileY,
			u8 tileId);
		void updateWindowTilePx(u8 palette, s32 displayX, s32 displayY, s32 tileX, s32 tileY,
			u8 tileId);
		void updateSpriteTilePx(u8 palette, s32 startX, s32 startY, u8 tileId, u8 flags);

		sf::Color getPixelColor(u8 palette, u8 top, u8 bottom, s32 bit,
			bool sprite = false);

		void setLCDStatus();
		bool isLCDEnabled();

		/*
			Takes in bit of interrupt to request
		*/
		void requestInterrupt(u8 interruptBit);
		void bufferPixels();

		sf::Image bgPixels;
		sf::Texture bgTexture;

		sf::Image winPixels;
		sf::Texture winTexture;

		sf::Image spritePixels;
		sf::Texture spriteTexture;

		sf::Sprite bgLayer;
		sf::Sprite winLayer;
		sf::Sprite spriteLayer;

		sf::Color shadesOfGrey[4];
		sf::Color superGbShades[4];
	
		s16 scanlineCounter = 0;
		s16 scanlinesRendered = 0;

		MMU* mmu;
		bool disablePpu = false;
	};
}