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
		/*std::memset(framebuf, 0x0, 160 * 144 * 3);
		std::memset(framebufA, 0x00, 160 * 144 * 4);
		std::memset(backgroundMapA, 0x0, 256 * 256 * 4);*/

		//store pixels in image
		//load texture with image pixel data
		image.create(256, 256, sf::Color::White);
		texture.loadFromImage(image);
	}

	void Ppu::update(u32 cycles)
	{
		setLCDStatus();

		if (isLCDEnabled()) {
			//subtract by the amount of clock cycles the lasst opcode took to execute
			//to keep the graphics in sync with the cpu
			scanlineCounter -= cycles; 
		}
		else
			return;

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
			else if (currentScanline < 144) {
				drawLine();
			}
		}
	}

	void Ppu::drawLine()
	{
		u8 lcdc = read(LCDC);

		//If bit 0 is on, we render the background tiles,
		//otherwise we don't
		if (testBit(lcdc, 0))
			drawTiles();

		//if (testBit(lcdc, 1))
			;//drawSprites();
	}

	void Ppu::drawTiles ()
	{
		//LCDC reg
		u8 lcdc = read(LCDC);
		u8 scx = read(SCX);
		u8 scy = read(SCY);
		u8 currentScanline = read(LY);

		u8 winx = read(WINDOWX) - 7;
		u8 winy = read(WINDOWY);


		/*
			In order to set which tiles should be displayed in the Background / Window grids,
			background maps are used. The VRAM sections $9800-$9BFF and
			$9C00-$9FFF each contain one of these background maps.
			A background map consists of 32x32 bytes representing tile numbers
			organized row by row. This means that the first byte in a background map
			is the Tile Number of the Tile at the very top left.
			The byte after is the Tile Number of the Tile to the right of it and so on.
			The 33rd byte would represent the Tile Number of the leftmost tile in the second tile row.

		*/

		bool window = false;
		//Display window?
		if (testBit(lcdc, 5)) {
			if (winy <= currentScanline)
				window = true;
		}


		/*
			*Addressing methods for tile data:

			8000 method uses $8000 as a base pointer and adds (TILE_NUMBER * 16) to it,
			where TILE_NUMBER is an unsigned 8 bit int. Thus, the Tile Number 0 would refer
			to address $8000, 1 would refer to $8010, 2 to $8020 and so on.

			The 8800 method uses $9000 as a base pointer and adds (SIGNED_TILE_NUMBER * 16) to it,
			where SIGNED_TILE_NUMBER is a signed 8-bit integer. Thus, the tile number 0 would refer
			to address $9000, 1 would refer to $9010, 2 to $9020 and so on.
			However, 0xFF would refer to $8FF0, 0xFE to $8FE0 and so on.

			*Which of these addressing methods is used depends on bit 4 of the LCDC register
		*/

		u16 tile_data_region;
		bool sign = false;
		//Bit 4 - determines tile data addressing mode
		u8 bit4 = (lcdc & (0x10)) >> 4;
		if (bit4) { //bit is on fetch tile data using 8000 mode
			tile_data_region = 0x8000;
		}
		else { //bit is off use 8800 mode(0x9000 as base pointer)
			//This region uses signed bytes as tile identifiers
			tile_data_region = 0x8800;
			sign = true;
		}
		

		/*
			So, we will read out the tile IDs at the tile_map, 
			and then read the actual graphic at tile_data, 
			identified by the tileID. The graphic data will then go into 
			our BG-array, pixel per pixel, in RGB format.
		*/
		
	
		u16 tile_map_region = 0;
		if (!window) {
			//Background memory region
			if (testBit(lcdc, 3))
				tile_map_region = 0x9C00;
			else
				tile_map_region = 0x9800;
		}
		else {
			//Window memory region
			if (testBit(lcdc, 6))
				tile_map_region = 0x9C00;
			else
				tile_map_region = 0x9800;
		}
	

		//Palette (2 bits per color)
		//bits 7-6 mapts to color id 11
		//bits 5-4 map to color id 10
		//bits 3-2 map to color id 01
		//bits 1-0 map to color id 00
		/*
			00: White
			01: Light Grey
			10: Dark Grey
			11: Black
		*/
		u8 palette = read(PALETTE);
		u8 offx = 0; u8 offy = 0;

		//Start drawing 160 horizontal pixels for this scanline
		for (size_t x = 0; x < 160; x++) {

			//Is window on?
			u8 bit5 = (lcdc & (0x20)) >> 5;
			if (bit5) {
				if (x >= winx && currentScanline >= winy)
					window = true;
			}


			if (!window) {
				offx = x + scx;
				offy = scy + currentScanline;
			}
			else {
				//window
				offx = x - winx;
				offy = currentScanline - winy;
			}

			u8 tilex = offx / 8; u8 tiley = offy / 8;
			u8 tilexc = offx % 8; u8 tileyc = offy % 8;

			u16 offset = (tiley * 32) + tilex;
			u8 tilen = read(tile_map_region + offset);
			u8 colorval = 0;

			if (sign) {
				//Signed data region
				s8 tile_num = (s8)tilen;
				u16 tileaddr = tile_data_region + 0x8000 + (tile_num * 16);
				u16 tile_line = tileaddr + (tileyc * 2);

				u8 byte1 = read(tile_line);
				u8 byte2 = read(tile_line + 1);

				u8 bit1 = (byte1 >> (7 - tilexc) & 0x1);
				u8 bit2 = (byte2 >> (7 - tilexc) & 0x1);

				colorval = (bit2 << 1) | bit1;
			}
			else {
				u16 tileaddr = tile_data_region + (tilen * 16);
				u16 tile_line = tileaddr + (tileyc * 2);

				u8 byte1 = mmu->read(tile_line);
				u8 byte2 = mmu->read(tile_line + 1);

				u8 bit1 = (byte1 >> (7 - tilexc)) & 0x1;
				u8 bit2 = (byte2 >> (7 - tilexc)) & 0x1;

				colorval = (bit2 << 1) | bit1;
			}
			sf::Color colr = getColor(colorval, palette);
			image.setPixel(x, currentScanline, colr);
		}	
	}

	sf::Color Ppu::getColor(u8 val, u8 pal)
	{
		u8 colorfrompal = (pal >> (2 * val)) & 0x3;
		if (colorfrompal == 0)
			return sf::Color(224, 248, 208, 255);
		else if (colorfrompal == 1)
			return sf::Color(192, 192, 192, 255);
		else if (colorfrompal == 2)
			return sf::Color(96, 96, 96, 255);
		else if (colorfrompal == 3)
			return sf::Color::Black;
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
		if (reqInterrupt && (mode != currentMode)) {
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
		return ((lcdc & (0x80)) >> 7);
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
		texture.loadFromImage(image);
	}
}
