#pragma once
#include "Utils.h"

namespace gbEmu {
	struct MMU;

	//Sound registers

	//Channel 1
	#define NR10 0xFF10
	#define NR11 0xFF11
	#define NR12 0xFF12
	#define NR13 0xFF13
	#define NR14 0xFF14

	//Channel 2
	#define NR21 0xFF16
	#define NR22 0xFF17
	#define NR23 0xFF18
	#define NR24 0xFF19

	//Channel 3
	#define NR30 0xFF1A
	#define NR31 0xFF1B
	#define NR32 0xFF1C
	#define NR33 0xFF1D
	#define NR34 0xFF1E

	//Channel 4
	#define NR41 0xFF20
	#define NR42 0xFF21
	#define NR43 0xFF22
	#define NR44 0xFF23

	//GPRs
	#define NR50 0xFF24
	#define NR51 0xFF25

	//APU Status (bit 7 = apu enabled)
	//(bits 0 - 3 = status of sound channels (read only))
	#define NR52 0xFF26 

	struct WavePattern {
		u8 duty;
		u8 waveform;
		float ratio;
	};

	struct Apu {
		Apu(MMU* mmu);
		void tick(s32 cycles);

		u8 read(u16 address);
		void write(u16 address, u8 value);


		u8 waveDutyTable[4];
		u8 currentPosition;
		u8 waveDutyPosition;

		s32 cycles;
		
		MMU* mmu;
	};
}