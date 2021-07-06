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

		kirokazeShades[0x0] = sf::Color(51, 44, 80, 255);
		kirokazeShades[0x1] = sf::Color(70, 135, 143, 255);
		kirokazeShades[0x2] = sf::Color(148, 227, 68, 255);
		kirokazeShades[0x3] = sf::Color(226, 243, 228, 255);
		
		purpleDawnShades[0x0] = sf::Color(238, 253, 237, 255);
		purpleDawnShades[0x1] = sf::Color(154, 123, 188, 255);
		purpleDawnShades[0x2] = sf::Color(45, 117, 126, 255);
		purpleDawnShades[0x3] = sf::Color(0, 27, 46, 255);


		pixels.create(160, 144, sf::Color::White);
		framebuffer.loadFromImage(pixels);

		s32 scaleFactor = 3;

		sprite = sf::Sprite(framebuffer);
		sprite.setScale(scaleFactor, scaleFactor);

		mode = PpuMode::OAMScan;
	}

	void Ppu::render(sf::RenderTarget& target)
	{
		target.draw(sprite);
	}

	void Ppu::update(s16 cycles)
	{
		if (isLCDEnabled()) {
			//subtract by the amount of clock cycles the last opcode took to execute
			//to keep the graphics in sync with the cpu
			scanlineCounter += cycles;
			switch (mode) {
				case PpuMode::HBlank:
					handleHBlankMode();
					break;
				case PpuMode::VBlank:
					handleVBlankMode(cycles);
					break;
				case PpuMode::OAMScan:
					handleOAMMode();
					break;
				case PpuMode::LCDTransfer:
					handleLCDTransferMode();
					break;
			}
		}
		else {
			disableLCD();
		}
	}

	void Ppu::handleHBlankMode()
	{
		if (scanlineCounter >= MIN_HBLANK_CYCLES) {
			scanlineCounter -= MIN_HBLANK_CYCLES;

			u8 ly = read(LY);
			mmu->memory[LY] = ++ly;
			compareLYandLYC();

			if (ly == VBLANK_SCANLINE) {
				mode = PpuMode::VBlank;

				//change to mode 1(vblank)
				u8 stat = read(STAT);
				stat = setBit(stat, 0);
				write(STAT, stat);

				requestInterrupt(Interrupts::VBlank);
				if (testBit(stat, 4)) {
					requestInterrupt(Interrupts::LCD);
				}
			}
			else {
				mode = PpuMode::OAMScan;
				u8 stat = read(STAT);
				stat = setBit(stat, 1);
				stat = resetBit(stat, 0);
				write(STAT, stat);

				if (testBit(stat, 5)) {
					requestInterrupt(Interrupts::LCD);
				}
			}
		}
	}

	void Ppu::handleVBlankMode(s16 cycles)
	{
		vblankCycles += cycles;
		u8 ly = read(LY);

		if (vblankCycles >= MAX_PPU_CYCLES) {
			vblankCycles = 0;
			if (ly < VBLANK_SCANLINE_MAX) {
				mmu->memory[LY] = ++ly;
			}
			compareLYandLYC();
		}

		if (scanlineCounter >= VBLANK_CYCLES) {
			vblankCycles = 0;
			scanlineCounter = 0;

			//Reset ly
			mmu->memory[LY] = 0;
			compareLYandLYC();

			//Change to OAM mode
			mode = PpuMode::OAMScan;
			u8 stat = read(STAT);
			stat = setBit(stat, 1);
			stat = resetBit(stat, 0);
			write(STAT, stat);

			if (testBit(stat, 5)) {
				requestInterrupt(Interrupts::LCD);
			}
		}
	}

	void Ppu::handleOAMMode()
	{
		if (scanlineCounter >= MIN_OAM_CYCLES) {
			scanlineCounter -= MIN_OAM_CYCLES;

			//Change to lcd transfer mode
			mode = PpuMode::LCDTransfer;
			u8 stat = read(STAT);
			stat = setBit(stat, 1);
			stat = setBit(stat, 0);
			write(STAT, stat);
		}
	}

	void Ppu::handleLCDTransferMode()
	{
		if (scanlineCounter >= MIN_LCD_TRANSFER_CYCLES) {
			scanlineCounter -= MIN_LCD_TRANSFER_CYCLES;

			drawLine();

			mode = PpuMode::HBlank;
			u8 stat = read(STAT);
			stat = resetBit(stat, 1);
			stat = resetBit(stat, 0);
			write(STAT, stat);

			if (testBit(stat, 3)) {
				requestInterrupt(Interrupts::LCD);
			}
		}
	}

	void Ppu::compareLYandLYC()
	{
		u8 ly = read(LY);
		u8 lyc = read(COINCIDENCE);
		u8 stat = read(STAT);

		if (lyc == ly) {
			stat = setBit(stat, 2);
			if (testBit(stat, 6)) {
				requestInterrupt(Interrupts::LCD);
			}
		}
		else {
			stat = resetBit(stat, 2);
		}
		write(STAT, stat);
	}

	void Ppu::drawLine()
	{
		u8 lcdc = read(LCDC);
		if (testBit(lcdc, 0)) {
			drawBackground(lcdc);
			if (testBit(lcdc, 5)) {
				u8 ly = read(LY);
				u8 wy = read(WINDOWY);
				if (ly >= wy)
					drawWindow(lcdc, ly, wy);
			}
		}			

		if (testBit(lcdc, 1))
			drawSprites(lcdc);
	}

	void Ppu::drawBackground(u8 lcdc)
	{
		u8 scx = read(SCX);
		u8 scy = read(SCY);

		u8 ly = read(LY);
		bool unsign = testBit(lcdc, 4);
		u8 bgPal = read(PALETTE);
		u16 tileMap = testBit(lcdc, 3) ? 0x9C00 : 0x9800;
		u16 tileData = unsign ? 0x8000 : 0x8800;

		u8 xpos = 0;
		u8 ypos = scy + ly;
		u16 tileRow;
		u16 tileLocation;
		for (s32 pixel = 0; pixel < 160; pixel++) {
			xpos = pixel + scx;
			ypos = scy + ly;
			drawTile(unsign, tileMap, tileData, xpos, ypos, pixel,
				bgPal, ly);
		}

	}

	void Ppu::drawWindow(u8 lcdc, u8 ly, u8 wy)
	{
		u8 wx = read(WINDOWX);
		if (wx <= 0x07)
			wx -= wx;
		else
			wx -= 7;

		bool unsign = testBit(lcdc, 4);
		u16 winTileMap = testBit(lcdc, 6) ? 0x9C00 : 0x9800;
		u16 tileData = unsign ? 0x8000 : 0x8800;
		u8 bgPal = read(PALETTE);

		u16 tileMap;
		u8 xpos = 0;
		u8 ypos = 0;
		u16 tileRow;
		for (s32 pixel = wx; pixel < 160; pixel++) {
			xpos = pixel - wx;
			ypos = ly - wy;
			drawTile(unsign, winTileMap, tileData, xpos, ypos, pixel,
				bgPal, ly);
		}
	}

	void Ppu::drawTile(bool unsign, u16 tileMap, u16 tileData, u8 xpos, u8 ypos, u8 pixel, u8 bgPal, u8 ly)
	{
		u16 tileRow = ((ypos / 8) * 32);
		if (unsign) {
			u8 tileNum = read(tileMap + tileRow + (xpos / 8));
			tileData += (tileNum * 16);
		}
		else {
			s8 tileNum = read(tileMap + tileRow + (xpos / 8));
			tileData += ((tileNum + 128) * 16);
		}

		u8 address = (ypos % 8) * 2;
		u8 upper = read(tileData + address);
		u8 lower = read(tileData + address + 1);

		s32 colorbit = xpos % 8;
		colorbit -= 7;
		colorbit = -colorbit;

		s32 colornum = testBit(lower, colorbit);
		colornum <<= 1;
		colornum |= testBit(upper, colorbit);

		sf::Color color = getPixelColor(colornum, bgPal);
		pixels.setPixel(pixel, ly, color);
	}

	void Ppu::drawSprites(u8 lcdc)
	{
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
				u16 dataAddress = ((0x8000 + (tileLocation * 16)) + line);
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


					s32 pixel = xpos + (7 - tilePixel);

					if ((ly < 0) || (ly > 143) || (pixel < 0) || (pixel > 159))
						continue;

					if (testBit(attributes, 7) == 1) {
						if ((pixels.getPixel(pixel, ly).r != 255) || (pixels.getPixel(pixel, ly).g != 255)
							|| (pixels.getPixel(pixel, ly).b != 255))
							continue;
					}

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

	void Ppu::disableLCD()
	{
		lcdEnabled = false;
		vblankCycles = 0;
		scanlineCounter = 0;
		mmu->memory[LY] = 0;

		u8 stat = read(STAT);
		stat &= 0xFC;
		write(STAT, stat);
		mode = PpuMode::HBlank;
	}

	bool Ppu::isLCDEnabled()
	{
		u8 lcdc = read(LCDC);
		return testBit(lcdc, 7);
	}

	PpuMode Ppu::getLCDMode()
	{
		u8 stat = read(STAT);
		return PpuMode(stat & 0x3);
	}

	void Ppu::requestInterrupt(Interrupts interrupt)
	{
		//interrupt request flag
		u8 IF = read(0xFF0F);
		IF = setBit(IF, (u8)interrupt);
		write(0xFF0F, IF);
	}

	void Ppu::bufferPixels()
	{
		framebuffer.loadFromImage(pixels);
	}
}
