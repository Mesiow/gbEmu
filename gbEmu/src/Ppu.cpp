#include "../include/Ppu.h"
#include "../include/MMU.h"

namespace gbEmu {
	Ppu::Ppu(MMU* mmu)
	{
		this->mmu = mmu;
	}

	u8 Ppu::read(u16 address)
	{
		return mmu->read(address);
	}

	void Ppu::write(u16 address, u8 value)
	{
		mmu->write(address, value);
	}

	void Ppu::init()
	{
		shadesOfGrey[0x0] = sf::Color(224, 248, 208, 255); // 0x0 - White
		shadesOfGrey[0x1] = sf::Color(198, 198, 198); // 0x1 - Light Gray
		shadesOfGrey[0x2] = sf::Color(127, 127, 127); // 0x2 - Dark Gray
		shadesOfGrey[0x3] = sf::Color(0, 0, 0);

		superGbShades[0x0] = sf::Color(255, 239, 206, 255); 
		superGbShades[0x1] = sf::Color(222, 148, 74, 255); 
		superGbShades[0x2] = sf::Color(173, 41, 33, 255);
		superGbShades[0x3] = sf::Color(49, 24, 82, 255);

		pixels.create(160, 144, sf::Color::White);
		framebuffer.loadFromImage(pixels);

		s32 scaleFactor = 3;

		sprite = sf::Sprite(framebuffer);
		sprite.setScale(scaleFactor, scaleFactor);
	}

	void Ppu::render(sf::RenderTarget& target)
	{
		target.draw(sprite);
	}

	void Ppu::update(s32 cycles)
	{
		setLCDStatus();

		if (isLCDEnabled()) {
			//subtract by the amount of clock cycles the last opcode took to execute
			//to keep the graphics in sync with the cpu
			scanlineCounter -= cycles;
		}
		else {
			return;
		}


		//if negative move to next scanline
		if (scanlineCounter <= 0) {
			//move onto the next scanline

			//Register 0xFF44 is the current scanline
			u8 currentScanline = read(LY);
			currentScanline++;
			write(LY, currentScanline);

			//set counter back to 456(each scanline takes 456 t cycles)
			scanlineCounter = 456;

			

			//VBlank period entered
			if (currentScanline == 144 && isLCDEnabled()) {
				requestInterrupt(0);
			}
			////if past line 153, reset to 0
			else if (currentScanline > 153) {
				write(LY, 0);
			}
			//draw current scanline
			else if (currentScanline < 144) {
				drawLine();
			}
		}
	}

	void Ppu::drawLine()
	{
		u8 lcdc = read(LCDC);

		if (testBit(lcdc, 7)) {
			//If bit 0 is on, we render the background tiles,
			if (testBit(lcdc, 0))
				drawBackground();

			if (testBit(lcdc, 5))
				drawWindow();

			if (testBit(lcdc, 1))
				drawSprites();
		}
	}

	void Ppu::drawBackground()
	{
		u8 lcdc = read(LCDC);
		u8 scx = read(SCX);
		u8 scy = read(SCY);
		u8 currentScanline = read(LY);
	
		u16 backgroundMapRegion;
		if (testBit(lcdc, 3)) {
			backgroundMapRegion = 0x9C00;
		}
		else {
			backgroundMapRegion = 0x9800;
		}

		u8 palette = read(PALETTE);

		// For each pixel in the 160x1 scanline:
		// 1. Calculate where the pixel resides in the overall 256x256 background map
		// 2. Get the tile ID where that pixel is located
		// 3. Get the pixel color based on that coordinate relative to the 8x8 tile grid
		// 4. Plot pixel in 160x144 display view

		s32 y = (s32)currentScanline;
		for (size_t x = 0; x < 160; ++x) {
			//1. Get pixel x,y in overall background map, offset by scroll x and y
			s32 mapx = (s32)scx + x;
			s32 mapy = (s32)scy + y;

			//Make sure coords loop around if exceeding 256 x 256 area
			if (mapx >= 256) mapx = mapx - 256;
			else mapx = mapx;

			if (mapy >= 256) mapy = mapy - 256;
			else mapy = mapy;

			//2. Get tile id where that pixel is located
			s32 tileCol = floor(mapx / 8);
			s32 tileRow = floor(mapy / 8);
			s32 tileMapId = (tileRow * 32) + tileCol;
			u16 location = backgroundMapRegion + tileMapId;
			u8 tileId = read(location);

			//3. Get pixel color based on that coord relative to
			//the 8x8 tile grid
			s32 tileXPixel = mapx % 8;
			s32 tileYPixel = mapy % 8;

			//Invert x pixels because they're stored backwards
			tileXPixel = abs(tileXPixel - 7);

			//4. Plot pixel in 160 x 144 display view
			updateBackgroundTilePx(palette, x, y, tileXPixel, tileYPixel, tileId);
		}
	}

	void Ppu::drawWindow()
	{
		u8 lcdc = read(LCDC);
		u8 currentScanline = read(LY);

		u16 windowMemoryRegion;
		if (testBit(lcdc, 6)) {
			windowMemoryRegion = 0x9C00;
		}
		else
			windowMemoryRegion = 0x9800;

		s32 winx = (s32)read(WINDOWX);
		s32 winy = (s32)read(WINDOWY);

		u8 palette = read(PALETTE);

		s32 y = (s32)currentScanline;

		for (size_t x = 0; x < 160; ++x) {
			//Window is relative to the screen
			//Shift x & y pixels based on window register value
			s32 displayX = x + winx - 7;
			s32 displayY = y;

			//1. Get tile id where that pixel is located
			s32 tileCol = floor(x / 8);
			s32 tileRow = floor((y - winy) / 8);
			s32 tileMapId = (tileRow * 32) + tileCol;
			u16 location = windowMemoryRegion + tileMapId;
			u8 tileId = read(location);

			// 2. Get the pixel color based on that coordinate relative to the 8x8 tile grid
			// 3. Plot pixel in 160x144 display view
			s32 tileXPixel = x % 8;
			s32 tileYPixel = y % 8;

			//Inver x pixels because theyre stored backwards
			tileXPixel = abs(tileXPixel - 7);

			if (currentScanline < winy) {
				//set window pixel
				pixels.setPixel(x, y, sf::Color::Transparent);
			}
			else {
				updateWindowTilePx(palette, displayX, displayY, tileXPixel, tileYPixel, tileId);
			}
		}
	}

	void Ppu::drawSprites()
	{
		u8 lcdc = read(LCDC);
		u16 oamLocation = 0xFE00;

		//Sprite size to display
		//If bit 2 is on sprites are displayed as 1x2 tiles(8 x 16 pixel),
		//otherwise 1x1 tile(8x8 pixel)
		bool sprite8x16 = testBit(lcdc, 2);

		u8 palette0 = read(OBP0);
		u8 palette1 = read(OBP1);

		//Each sprite is 4 bytes in OAM, there are 160 bytes of 
		//sprite data, 160 / 4 = 40 sprites total can be rendered.
		u8 spriteSize = 4;
		//Start from the right
		for (s32 id = 39; id >= 0; id--) {
			u16 offset = oamLocation + (id * spriteSize);

			//Byte 0
			//Sub 16 to determine actual y position
			s32 ypos = ((s32)read(offset)) - 16;

			//Byte 1
			//Sub 8 to determine actual x position
			s32 xpos = ((s32)read(offset + 1)) - 8;

			//Byte 2
			//Tile Number used for fetching the graphics data
			//for the sprite
			u8 tileNum = read(offset + 2);

			//Byte 3
			//Bit flags to apply certain effects and options to a sprite
			u8 flags = read(offset + 3);

			//Check which sprite palette to use
			bool paletteNum = testBit(flags, 4);
			u8 spritePalette = (paletteNum) ? palette1 : palette0;

			if (sprite8x16) {
				// If in 8x16 mode, the tile pattern for the top is tileNum & 0xFE
				// Lower 8x8 tile is tileNum | 0x1
				tileNum = tileNum & 0xFE;
				u8 tileNumBottom = tileNum | 0x1;

				updateSpriteTilePx(spritePalette, xpos, ypos, tileNum, flags);
				updateSpriteTilePx(spritePalette, xpos, ypos + 8, tileNumBottom, flags);
			}
			else {
				updateSpriteTilePx(spritePalette, xpos, ypos, tileNum, flags);
			}
		}
	}

	void Ppu::updateBackgroundTilePx(u8 palette, s32 displayX, s32 displayY, s32 tileX, s32 tileY, u8 tileId)
	{
		u8 lcdc = read(LCDC);
		// Figure out where the current background character data is being stored
		// if selection=0 bg area is 0x8800-0x97FF and tile ID is determined by SIGNED -128 to 127
		// 0x9000 represents the zero ID address in that range
		u16 tileDataLocation;
		bool sign = false;
		u16 offset;

		if (testBit(lcdc, 4)) {
			tileDataLocation = 0x8000;
		}
		else {
			tileDataLocation = 0x9000;
			sign = true;
		}

		if (sign) { // 0x8800 - 0x97FF signed
			s8 id = (s8)tileId;
			u16 tempoffset = (tileDataLocation) + (id * 16);
			offset = (u16)tempoffset;
		}
		else { // 0x8000 - 0x8FFF unsigned 
			offset = (tileId * 16) + tileDataLocation;
		}

		u8 hi = read(offset + (tileY * 2) + 1),
			lo = read(offset + (tileY * 2));

		sf::Color color = getPixelColor(palette, lo, hi, tileX);
		pixels.setPixel(displayX, displayY, color);
	}

	void Ppu::updateWindowTilePx(u8 palette, s32 displayX, s32 displayY, s32 tileX, s32 tileY, u8 tileId)
	{
		if (displayX >= 160 || displayX < 0) return;
		if (displayY >= 144 || displayY < 0) return;

		u8 lcdc = read(LCDC);

		u16 tileDataLocation;
		bool sign = false;
		u16 offset;
		if (testBit(lcdc, 4)) {
			tileDataLocation = 0x8000;
		}
		else {
			tileDataLocation = 0x9000;
			sign = true;
		}

		// 0x8800 - 0x97FF signed
		if (sign) {
			s8 id = (s8)tileId;
			u16 tempoffset = (tileDataLocation)+(id * 16);
			offset = (u16)tempoffset;
		}
		else {// 0x8000 - 0x8FFF unsigned 
			offset = (tileId * 16) + tileDataLocation;
		}

		u8 hi = read(offset + (tileY * 2) + 1),
			lo = read(offset + (tileY * 2));

		sf::Color color = getPixelColor(palette, lo, hi, tileX);
		pixels.setPixel(displayX, displayY, color);
	}

	void Ppu::updateSpriteTilePx(u8 palette, s32 startX, s32 startY, u8 tileId, u8 flags)
	{
		//Sprites always use the 8000 addressing method (always unsigned 8 bit integer)
		u16 address = 0x8000;

		//If priority is set to 0, then sprite is rendered above background
		//If set to 1, sprite is hidden behind background and window
		//unless color of bg or win is white, the sprite is then rendered on top of that
		bool priority = testBit(flags, 7);
		bool yflip = testBit(flags, 6);
		bool xflip = testBit(flags, 5);

		for (s32 y = 0; y < 8; y++) {
			s32 offset = (tileId * 16) + address;

			u8 hi = read(offset + (y * 2) + 1),
				lo = read(offset + (y * 2));

			for (s32 x = 0; x < 8; x++) {
				s32 pixelX = (xflip) ? (startX + x) : (startX + 7 - x);
				s32 pixelY = (yflip) ? (startY + 7 - y) : (startY + y);

				u32 boundsX = spritePixels.getSize().x;
				u32 boundsY = spritePixels.getSize().y;

				if (pixelX < 0 || pixelX >= boundsX) continue;
				if (pixelY < 0 || pixelY >= boundsY) continue;

				sf::Color color = getPixelColor(palette, lo, hi, x, true);

				//If color in bg/win is not white, hide the sprite pixel
				sf::Color bgColor = pixels.getPixel(pixelX, pixelY);
				
				if (priority) {
					if (bgColor != superGbShades[0]) continue;
				}
				pixels.setPixel(pixelX, pixelY, color);
			}
		}
	}

	sf::Color Ppu::getPixelColor(u8 palette, u8 top, u8 bottom, s32 bit, bool sprite)
	{
		u8 color3 = (palette >> 6); //extract bits 7 & 6
		u8 color2 = (palette >> 4) & 0x3;
		u8 color1 = (palette >> 2) & 0x3;
		u8 color0 = (palette & 0x3);


		//Get color code from the two bytes
		u8 first = testBit(top, bit);
		u8 second = testBit(bottom, bit);
		u8 colorCode = (second << 1) | first;

		

		if (colorCode == 0x0) {
			if (sprite) {
				return sf::Color::Transparent;
			}
			//return shadesOfGrey[color0];
			return superGbShades[color0];
		}
		else if (colorCode == 0x1) {
			//return shadesOfGrey[color1];
			return superGbShades[color1];
		}
		else if (colorCode == 0x2) {
			//return shadesOfGrey[color2];
			return superGbShades[color2];
		}
		else if (colorCode == 0x03) {
			//return shadesOfGrey[color3];
			return superGbShades[color3];
		}
		else {
			return sf::Color(255, 0, 255);
		}
	}

	void Ppu::setLCDStatus()
	{
		u8 stat = read(STAT);
		if (!isLCDEnabled()) {
			//Set mode to 1 during lcd disabled and reset
			//scanline
			scanlineCounter = 456;
			write(LY, 0);

			stat &= 0xFC;
			stat = setBit(stat, 0);

			write(STAT, stat);

			return;
		}

		u8 currentLine = read(LY);
		PpuMode currentMode = (PpuMode)(stat & 0x3);

		PpuMode mode = PpuMode::HBlank;
		bool reqInterrupt = false;

		//In VBlank mode, set mode to 1
		if (currentLine >= 144) {
			mode = PpuMode::VBlank;

			//Mode 1 is VBlank(set bit 0 for VBlank mode)
			stat = setBit(stat, 0);
			stat = resetBit(stat, 1);
			reqInterrupt = testBit(stat, 4);
		}
		else {
			s16	Oambounds = 456 - 80;
			s16 Drawingbounds = Oambounds - 172;

			//In mode 2 (OAM Scan)
			if (scanlineCounter >= Oambounds) {
				mode = PpuMode::OAMScan;
				stat = setBit(stat, 1);
				stat = resetBit(stat, 0);
				reqInterrupt = testBit(stat, 5);
			}
			//In mode 3 (Drawing)
			else if (scanlineCounter >= Drawingbounds) {
				mode = PpuMode::Drawing;
				stat = setBit(stat, 1);
				stat = setBit(stat, 0);
			}
			else {
				//In mode 0 (HBlank)
				mode = PpuMode::HBlank;
				stat = resetBit(stat, 1);
				stat = resetBit(stat, 0);
				reqInterrupt = testBit(stat, 3);
			}
		}

		//Check if there is an interrupt request
		//and we entered a new mode
		if (reqInterrupt && (mode != currentMode)) {
			requestInterrupt(1);
		}

		/*
			LYC is used to compare a value to the LY register.
			If they match, the match flag is set in the STAT register.
		*/
		if ((currentLine == read(COINCIDENCE)) && mode == PpuMode::OAMScan) {
			//Set coincidence flag
			stat = setBit(stat, 2);

			//Request interrupt
			if (testBit(stat, 6))
				requestInterrupt(1);
		}
		else {
			stat = resetBit(stat, 2);
		}
		//Update lcd status
		write(STAT, stat);
	}

	bool Ppu::isLCDEnabled()
	{
		u8 lcdc = read(LCDC);
		return testBit(lcdc, 7);
	}

	void Ppu::requestInterrupt(u8 interruptBit)
	{
		//interrupt request flag
		u8 IF = read(0xFF0F);
		IF = setBit(IF, interruptBit);
		write(0xFF0F, IF);
	}

	void Ppu::bufferPixels()
	{
		framebuffer.loadFromImage(pixels);
	}
}
