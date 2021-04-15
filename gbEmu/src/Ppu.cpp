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
		std::memset(framebuf, 0x0, 160 * 144 * 3);
		std::memset(framebufA, 0x00, 160 * 144 * 4);
		std::memset(backgroundMapA, 0x0, 256 * 256 * 4);

		//store pixels in image
		//load texture with image pixel data
		image.create(160, 144, framebufA);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}

	void Ppu::draw(sf::RenderTarget& target)
	{
		for (size_t row = 0; row < 144; ++row) {
			for (size_t col = 0; col < 160; ++col) {

			}
		}
		//update image
		//update texture
		//set texture on sprite
		//render new sprite
		//texture.update()
	}

	void Ppu::drawBackground()
	{
		//LCDC reg
		u8 lcdc = read(0xFF40);


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

		u16 tile_map;
		//Bit 3 - determines Background tile map
		u16 bit3 = (lcdc & (0x8)) >> 3;
		if (bit3) { //bit is on, the background will use the bg map located at 0x9C00 - 0x9FFF
			tile_map = 0x9C00;
		}
		else { //use bg map at 0x9800 - 0x9BFF
			tile_map = 0x9800;
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

		u16 tile_data;
		bool signed_tile_num;
		//Bit 4 - determines tile data addressing mode
		u8 bit4 = (lcdc & (0x10)) >> 4;
		if (bit4) { //bit is on fetch tile data using 8000 mode
			tile_data = 0x8000;
			signed_tile_num = false;
		}
		else { //bit is off use 8800 mode(0x9000 as base pointer)
			tile_data = 0x8800;
			signed_tile_num = true;
		}
		

		/*
			So, we will read out the tile IDs at the tile_map, 
			and then read the actual graphic at tile_data, 
			identified by the tileID. The graphic data will then go into 
			our BG-array, pixel per pixel, in RGB format.
		*/
		const int tileSizeInMem = 16;
		if (signed_tile_num) {

		}
		else {

		}

		
	}
}
