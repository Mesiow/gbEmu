#include "../include/Apu.h"
#include "../include/MMU.h"

namespace gbEmu {
	Apu::Apu(MMU* mmu)
	{
		this->mmu = mmu;

		cycles = 0;

		waveDutyTable[0] = 0x1;
		waveDutyTable[1] = 0x81;
		waveDutyTable[2] = 0x87;
		waveDutyTable[3] = 0x7E;

		currentPosition = 0;
		waveDutyPosition = 0;
	}

	void Apu::tick(s32 cycles)
	{
		u8 status = read(NR52);
		//APU is enabled
		if (testBit(status, 7)) {
			//Channel two (produces square waves)

			//NR23 = lo frequency
			//NR24 = hi frequency
			u8 freqLo = read(NR23);
			u8 freqHi = read(NR24);
			u16 frequency = ((freqHi << 8) | freqLo);

			u16 freqTimer = (2048 - frequency) * 4;

			u8 dutyPattern = read(NR21);

			currentPosition = waveDutyPosition;

			//Wave duty position is wrapped back to 0 when it goes above 7
			waveDutyPosition = (currentPosition + 1) % 8;

			//u8 amplitude = waveDutyTable
		}
	}

	u8 Apu::read(u16 address)
	{
		return mmu->read(address);
	}

	void Apu::write(u16 address, u8 value)
	{
		mmu->write(address, value);
	}
}