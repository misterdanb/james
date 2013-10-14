#ifndef _IOPorts_hpp_
#define _IOPorts_hpp_

#include "MemoryBlock.hpp"
#include "MemoryBus.hpp"
#include "SpriteSize.hpp"
#include "LCDMode.hpp"
#include "HDMAMode.hpp"
#include "Joypad.hpp"
#include "SerialPort.hpp"

namespace james
{
	namespace core
	{
		class IOPorts : public MemoryBlock, public MemoryBus
		{
		public:
			static const int IO_PORTS_OFFSET = 0xFF00;
			
			static const int IO_PORTS_BANKS = 1;
			static const int IO_PORTS_BANK_SIZE = 0x0080;
			
		public:
			IOPorts();
			~IOPorts();
			
			int ReadByte(int);
			void WriteByte(int, int);
			
			void Serialize(std::ostream &);
			void Deserialize(std::istream &);
			
			void ExecuteOAMDMATransfer();
			void ExecuteHDMATransfer();
			
			void SetMemoryBus(MemoryBus &);
			
			void SetJoypad(Joypad &);
			void SetSerialPort(SerialPort &);
			
			void SetHDMATransferActive(bool);
			void SetHDMAMode(HDMAMode);
			void SetHDMATransferSourceAddress(int);
			void SetHDMATransferDestinationAddress(int);
			void SetHDMATransferLength(int);
			void SetHBlankHDMATransferCurrentOffset(int);
			
			void SetSelectedWorkRamBank(bool);
			void SetSelectedVideoRamBank(bool);
			
			void SetDirectionKeysSelected(bool);
			void SetButtonKeysSelected(bool);
			
			void SetLCDDisplayEnabled(bool);
			void SetWindowMapDisplaySelect(bool);
			void SetWindowDisplayEnabled(bool);
			void SetBackgroundAndWindowTileDataSelect(bool);
			void SetBackgroundMapDisplaySelect(bool);
			void SetSpriteSize(SpriteSize);
			void SetSpriteDisplayEnabled(bool);
			void SetBackgroundDisplayEnabled(bool);
			
			void SetLCDMode(LCDMode);
			void SetCoincidenceFlag(bool);
			void SetHBlankInterruptEnabled(bool);
			void SetVBlankInterruptEnabledInLCD(bool);
			void SetOAMInterruptEnabled(bool);
			void SetCoincidenceInterruptEnabled(bool);
			
			void SetScrollX(int);
			void SetScrollY(int);
			void SetLCDY(int);
			void SetLCDYCompare(int);
			void SetWindowX(int);
			void SetWindowY(int);
			
			void SetDevider(int);
			void SetTimerCounter(int);
			void SetTimerModulo(int);
			void SetTimerClockFrequency(int);
			void SetTimerStopped(bool);
			
			MemoryBus &GetMemoryBus();
			
			Joypad &GetJoypad();
			SerialPort &GetSerialPort();
			
			bool GetRightPressed();
			bool GetLeftPressed();
			bool GetUpPressed();
			bool GetDownPressed();
			bool GetButtonAPressed();
			bool GetButtonBPressed();
			bool GetSelectPressed();
			bool GetStartPressed();
			
			bool GetHDMATransferActive();
			HDMAMode GetHDMAMode();
			int GetHDMATransferSourceAddress();
			int GetHDMATransferDestinationAddress();
			int GetHDMATransferLength();
			int GetHBlankHDMATransferCurrentOffset();
			
			bool GetSelectedWorkRamBank();
			bool GetSelectedVideoRamBank();
			
			bool GetDirectionKeysSelected();
			bool GetButtonKeysSelected();
			
			bool GetLCDDisplayEnabled();
			bool GetWindowMapDisplaySelect();
			bool GetWindowDisplayEnabled();
			bool GetBackgroundAndWindowTileDataSelect();
			bool GetBackgroundMapDisplaySelect();
			SpriteSize GetSpriteSize();
			bool GetSpriteDisplayEnabled();
			bool GetBackgroundDisplayEnabled();
			
			LCDMode GetLCDMode();
			bool GetCoincidenceFlag();
			bool GetHBlankInterruptEnabled();
			bool GetVBlankInterruptEnabledInLCD();
			bool GetOAMInterruptEnabled();
			bool GetCoincidenceInterruptEnabled();
			
			int GetScrollX();
			int GetScrollY();
			int GetLCDY();
			int GetLCDYCompare();
			int GetWindowX();
			int GetWindowY();
			
			int GetDevider();
			int GetTimerCounter();
			int GetTimerModulo();
			int GetTimerClockFrequency();
			bool GetTimerStopped();
			
			Array<Color<int>, 4> &GetMonochromeBackgroundPalette();
			Array<Color<int>, 4> &GetMonochromeSpritePalette(int);
			
			Array<Color<int>, 4> &GetColorBackgroundPalette(int);
			Array<Color<int>, 4> &GetColorSpritePalette(int);
			
		private:
			MemoryBus *_bus;
			
			Joypad *_joypad;
			SerialPort *_serialPort;
			
			// dma config
			bool _hdmaTransferActive;
			HDMAMode _hdmaMode;
			int _hdmaTransferSourceAddress;
			int _hdmaTransferDestinationAddress;
			int _hdmaTransferLength;
			int _hBlankHDMATransferCurrentOffset;
			
			// ram banks
			int _selectedWorkRamBank;
			int _selectedVideoRamBank;
			
			// joypad config
			bool _directionKeysSelected;
			bool _buttonKeysSelected;
			
			// lcd control
			bool _lcdDisplayEnabled;
			bool _windowMapDisplaySelect;
			bool _windowDisplayEnabled;
			bool _backgroundAndWindowTileDataSelect;
			bool _backgroundMapDisplaySelect;
			SpriteSize _spriteSize;
			bool _spriteDisplayEnabled;
			bool _backgroundDisplayEnabled;
			
			// lcd status
			LCDMode _lcdMode;
			bool _coincidenceFlag;
			bool _hBlankInterruptEnabled;
			bool _vBlankInterruptEnabledInLCD;
			bool _oamInterruptEnabled;
			bool _coincidenceInterruptEnabled;
			
			// lcd position and scrolling
			int scrollX;
			int scrollY;
			int lcdY;
			int lcdYCompare;
			int windowX;
			int windowY;
			
			// timer config
			int _divider;
			int _timerCounter;
			int _timerModulo;
			int _timerClockFrequency;
			bool _timerStopped;
			
			// monochrome palettes
			Array<Color<int>, 4> _monochromePalette;
			
			Array<Color<int>, 4> _monochromeBackgroundPalette;
			Array2<Color<int>, 4, 2> _monochromeSpritePalettes;
			
			// color palettes
			int _colorBackgroundPaletteIndexAutoIncrement;
			Array2<Color<int>, 4, 8> _colorBackgroundPalettes;
			
			int _colorSpritePaletteIndexAutoIncrement;
			Array2<Color<int>, 4, 8> _colorSpritePalettes;
		};
	}
}

#endif
