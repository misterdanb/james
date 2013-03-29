#ifndef _CartridgeType_hpp_
#define _CartridgeType_hpp_

namespace gbc
{
	namespace core
	{
		namespace cartridges
		{
			enum CartrdigeType
			{
				romOnly = 0x00,
				mbc1 = 0x01
				mbc1Ram = 0x02,
				mbc1RamBattery = 0x03,
				mbc2 = 0x05,
				mbc2Battery = 0x06,
				romRam = 0x08,
				romRamBattery = 0x09,
				mmm01 = 0x0B,
				mmm01Ram = 0x0C,
				mmm01RamBattery = 0x0D,
				mbc3TimerBattery = 0x0F,
				mbc3TimerRamBattery = 0x10,
				mbc3 = 0x11,
				mbc3Ram = 0x12,
				mbc3RamBattery = 0x13,
				mbc4 = 0x15,
				mbc4Ram = 0x16,
				mbc4RamBattery = 0x17,
				mbc5 = 0x19,
				mbc5Ram = 0x1A,
				mbc5RamBattery = 0x1B,
				mbc5Rumble = 0x1C,
				mbc5RumbleRam = 0x1D,
				mbc5RumbleRamBattery = 0x1E,
				PocketCamera = 0xFC,
				BandaiTamas = 0xFD,
				HuC3 = 0xFE,
				HuC1RamBattery = 0xFF
			};
		}
	}
}

#endif
