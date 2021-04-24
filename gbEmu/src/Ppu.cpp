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
		bgPixels.create(160, 144, sf::Color::White);
		bgTexture.loadFromImage(bgPixels);

		winPixels.create(160, 144, sf::Color::Transparent);
		winTexture.loadFromImage(winPixels);

		spritePixels.create(160, 144, sf::Color::Transparent);
		spriteTexture.loadFromImage(spritePixels);


		bgLayer = sf::Sprite(bgTexture);
		bgLayer.setScale(3, 3);

		winLayer = sf::Sprite(winTexture);
		winLayer.setScale(3, 3);
	}

	void Ppu::render(sf::RenderTarget& target)
	{
		target.draw(bgLayer);
		target.draw(winLayer);
		//target.draw(spriteLayer);
	}

	void Ppu::update(u32 cycles)
	{
		setLCDStatus();

		if (isLCDEnabled()) {
			//subtract by the amount of clock cycles the lasst opcode took to execute
			//to keep the graphics in sync with the cpu
			scanlineCounter -= cycles;
		}
		else {
			return;
		}

	
		if (read(LY) > 153)
			write(LY, 0);

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
			if (currentScanline == 144) {
				requestInterrupt(0);
			}
			//if past line 153, reset to 0
			else if (currentScanline > 153) {
				write(LY, 0);
			}
			//draw current scanline
			else if (currentScanline <= 144) {
				drawLine();
			}
		}
	}

	void Ppu::drawLine()
	{
		u8 lcdc = read(LCDC);

		if (testBit(lcdc, 7)) {
			//If bit 0 is on, we render the background tiles,
			//otherwise we don't
			if (testBit(lcdc, 0))
				drawBackground();

			if (testBit(lcdc, 5))
				drawWindow();
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

		s32 y = currentScanline;
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
				winPixels.setPixel(x, y, sf::Color::Transparent);
			}
			else {
				updateWindowTilePx(palette, displayX, displayY, tileXPixel, tileYPixel, tileId);
			}
		}
	}

	void Ppu::drawSprites()
	{

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
		bgPixels.setPixel(displayX, displayY, color);
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
		winPixels.setPixel(displayX, displayY, color);
	}

	sf::Color Ppu::getPixelColor(u8 palette, u8 top, u8 bottom, s32 bit)
	{
		u8 color3 = (palette >> 6); //extract bits 7 & 6
		u8 color2 = (palette >> 4) & 0x3;
		u8 color1 = (palette >> 2) & 0x3;
		u8 color0 = (palette & 0x3);


		//Get color code from the two bytes
		u8 first = testBit(top, bit);
		u8 second = testBit(bottom, bit);
		u8 colorCode = (second << 1) | first;

		sf::Color shadesOfGrey[4];
		shadesOfGrey[0x0] = sf::Color(224, 248, 208, 255); // 0x0 - White
		shadesOfGrey[0x1] = sf::Color(198, 198, 198); // 0x1 - Light Gray
		shadesOfGrey[0x2] = sf::Color(127, 127, 127); // 0x2 - Drak Gray
		shadesOfGrey[0x3] = sf::Color(0, 0, 0);

		if (colorCode == 0x0) {
			return shadesOfGrey[color0];
		}
		else if (colorCode == 0x1) {
			return shadesOfGrey[color1];
		}
		else if (colorCode == 0x2) {
			return shadesOfGrey[color2];
		}
		else if (colorCode == 0x03) {
			return shadesOfGrey[color3];
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
			s32	Oambounds = 456 - 80;
			s32 Drawingbounds = Oambounds - 172;

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
		if (reqInterrupt && (currentMode != mode)) {
			requestInterrupt(1);
		}

		/*
			LYC is used to compare a value to the LY register.
			If they match, the match flag is set in the STAT register.
		*/
		u8 ly = read(LY);
		if (ly == read(COINCIDENCE)) {
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
		bgTexture.loadFromImage(bgPixels);
		winTexture.loadFromImage(winPixels);
	}
}
