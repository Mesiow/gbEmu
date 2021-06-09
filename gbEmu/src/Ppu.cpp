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

		customShades[0x0] = sf::Color(186, 247, 235, 255);
		customShades[0x1] = sf::Color(116, 217, 217, 255);
		customShades[0x2] = sf::Color(72, 163, 195, 255);
		customShades[0x3] = sf::Color(29, 118, 167, 255);

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

		//If bit 0 is on, we render the background tiles,
		if (testBit(lcdc, 0))
			drawTiles();

		if (testBit(lcdc, 1))
			drawSprites();
	}

	void Ppu::drawTiles()
	{
		u8 lcdc = read(LCDC);
		u8 scx = read(SCX);
		u8 scy = read(SCY);
		u8 wx = read(WINDOWX) - 7;
		u8 wy = read(WINDOWY);

		u16 tileData = 0;
		u16 backgroundAddress = 0;
		bool unsign = true;
		bool windowEnabled = false;

		if (testBit(lcdc, 5)) {
			if (wy <= read(LY))
				windowEnabled = true;
		}

		//Tile data
		if (testBit(lcdc, 4)) {
			tileData = 0x8000;
		}
		else {
			tileData = 0x8800;
			unsign = false;
		}

		//Background/Window memory (for TileID)
		if (!windowEnabled) {
			if (testBit(lcdc, 3)) {
				backgroundAddress = 0x9C00;
			}
			else {
				backgroundAddress = 0x9800;
			}
		}
		else {
			if (testBit(lcdc, 6)) {
				backgroundAddress = 0x9C00;
			}
			else {
				backgroundAddress = 0x9800;
			}
		}

		//Current tile y position of the 32 vertical tiles
		u8 ypos = 0;
		if (!windowEnabled)
			ypos = scy + read(LY);
		else
			ypos = read(LY) - wy;

		//Which 8 pixels of the current tile is the scanline on
		u16 tileRow = (((u8)(ypos / 8)) * 32);
	
		for (s32 pixel = 0; pixel < 160; pixel++) {
			u8 xpos = pixel + scx;

			//translate current x to window space
			if (windowEnabled) {
				if (pixel >= wx) {
					xpos = pixel - wx;
				}
			}

			//which of 32 horizontal tiles does this xpos fall within
			u16 tileCol = (xpos / 8);
			s16 tileNum;
			

			//Get tile id
			u16 tileAddress = backgroundAddress + tileRow + tileCol;
			if (unsign) {
				tileNum = (u8)read(tileAddress);
			}
			else {
				tileNum = (s8)read(tileAddress);
			}

			//Where is tile id in memory
			u16 tileLocation = tileData;
			if (unsign) {
				tileLocation += (tileNum * 16);
			}
			else {
				tileLocation += ((tileNum + 128) * 16);
			}

			//Find correct vertical line we're on to get tile data
			u8 line = ypos % 8;
			line *= 2; //each vertical line = 2 bytes
			u8 data1 = read(tileLocation + line);
			u8 data2 = read(tileLocation + line + 1);

			//Pixel 0 in tile is bit 7 of data 1 and data 2
			//Pixel 1 is bit 6 etc
			s32 colorBit = xpos % 8;
			colorBit -= 7;
			colorBit *= -1;

			//Combine data 2 and data 1 to get color id for this pixel
			//in the tile
			s32 colorNum = testBit(data2, colorBit);
			colorNum <<= 1;
			colorNum |= testBit(data1, colorBit);

			//Now we have the color id to get the actual color
			//from palette
			sf::Color color = getPixelColor(colorNum, read(PALETTE));

			u8 ly = read(LY);
			if ((ly < 0) || (ly > 143) || (pixel < 0) || (pixel > 159))
				continue;

			pixels.setPixel(pixel, ly, color);
		}
	}

	void Ppu::drawSprites()
	{
		u8 lcdc = read(LCDC);

		bool use8x16 = false;
		if (testBit(lcdc, 2)) {
			use8x16 = true;
		}

		for (s32 sprite = 0; sprite < 40; sprite++) {
			//Sprite takes up 4 bytes in the sprite attribute table
			u8 index = sprite * 4;
			u8 ypos = read(OAM + index) - 16;
			u8 xpos = read(OAM + index + 1) - 8;
			u8 tileLocation = read(OAM + index + 2);
			u8 attributes = read(OAM + index + 3);

			bool yflip = testBit(attributes, 6);
			bool xflip = testBit(attributes, 5);

			s32 ly = read(LY);

			s32 ysize = 8;
			if (use8x16)
				ysize = 16;

			//Sprite intercept scanline?
			if ((ly >= ypos) && (ly < (ypos + ysize))) {
				s32 line = ly - ypos;

				//read sprite in reverse y axis
				if (yflip) {
					line -= ysize;
					line *= -1;
				}

				line *= 2; //same as tiles
				u16 dataAddress = (0x8000 + (tileLocation * 16)) + line;
				u8 data1 = read(dataAddress);
				u8 data2 = read(dataAddress + 1);

				//Loop right to left
				for (s32 tilePixel = 7; tilePixel >= 0; tilePixel--) {
					s32 colorBit = tilePixel;

					//read sprite in reverse for x axis
					if (xflip) {
						colorBit -= 7;
						colorBit *= -1;
					}

					//Rest is same as tiles
					s32 colorNum = testBit(data2, colorBit);
					colorNum <<= 1;
					colorNum |= testBit(data1, colorBit);

					u16 colorAddress = testBit(attributes, 4) ? OBP1 : OBP0;
					sf::Color color = getPixelColor(colorNum, read(colorAddress));

					//if color is first in palette then is transparent for sprites
					if (colorNum == 0)
						continue;

					s32 xPix = 0 - tilePixel;
					xPix += 7;

					s32 pixel = xpos + xPix;

					if ((ly < 0) || (ly > 143) || (pixel < 0) || (pixel > 159))
						continue;

					/*if (testBit(attributes, 7) == 1) {
						if ((pixels.getPixel(pixel, ly).r != 255) || (pixels.getPixel(pixel, ly).g != 255)
							|| (pixels.getPixel(pixel, ly).b != 255))
							continue;
					}*/

					pixels.setPixel(pixel, ly, color);
				}
			}
		}
	}

	sf::Color Ppu::getPixelColor(u8 colorNum, u8 palette, bool sprite)
	{
		u8 colorfrompal = (palette >> (2 * colorNum)) & 0x3;

		if (colorfrompal == 0)
			return superGbShades[0];
		else if (colorfrompal == 1)
			return superGbShades[1];
		else if (colorfrompal == 2)
			return superGbShades[2];
		else if (colorfrompal == 3)
			return superGbShades[3];
	}

	void Ppu::setLCDStatus()
	{

		u8 stat = read(STAT);
		if (!isLCDEnabled()) {
			//Clear stat and reset scanline during lcd disabled
			scanlineCounter = 456;
			write(LY, 0);

			stat &= 0xFC;
			stat = resetBit(stat, 0);
			stat = resetBit(stat, 1);

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
		if (reqInterrupt && (currentMode != mode)) {
			requestInterrupt(1);
		}

		/*
			LYC is used to compare a value to the LY register.
			If they match, the match flag is set in the STAT register.
		*/
		if ((currentLine == read(COINCIDENCE))) {
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
