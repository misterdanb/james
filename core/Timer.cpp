#include "Timer.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cpu;

Timer::Timer()
	: _ticksPerCycle (0),
	  _ticksLeftCycle (0),
	  _dividerCounter (0),
	  _timerCounter (0)
{
}

Timer::~Timer()
{
}

void Timer::SetRenderContext (RenderContext& rc)
{
	_rc = &rc;
}

void Timer::ExecuteTicks (int ticks)
{
	_timerCounter = _rc->ioPorts[5];

	if (GetBit (_rc->ioPorts[TIMER_CONTROL_ADDRESS - 0xFF00], 3))
	{
		if (_ticksPerCycle == 0)
		{
			switch (_rc->ioPorts[TIMER_CONTROL_ADDRESS - 0xFF00] & 0x03)
			{
				case 0:
					_ticksPerCycle = 1024;
					break;

				case 1:
					_ticksPerCycle = 16;
					break;

				case 2:
					_ticksPerCycle = 64;
					break;

				case 3:
					_ticksPerCycle = 256;
					break;

				default:
					break;
			}

			_ticksLeftCycle = _ticksPerCycle;
		}

		_ticksLeftCycle -= ticks;

		if (_ticksLeftCycle <= 0)
		{
			_ticksLeftCycle = _ticksPerCycle;
			_timerCounter++;
		}

		if (_timerCounter > 0xFF)
		{
			_rc->ioPorts[TIMER_MODULO_ADDRESS - 0xFF00] =_rc->ioPorts[TIMER_MODULO_ADDRESS - 0xFF00];
			_rc->interruptHandler->SignalTimerInterrupt();
			_ticksPerCycle = 0;
		}
		else
		{
			_rc->ioPorts[5] = _timerCounter;
		}
	}

	_dividerCounter += ticks;

	if (_dividerCounter > 0xFF)
	{
		_rc->ioPorts[DEVIDER_REGISTER_ADDRESS - 0xFF00]++;

		if (_rc->ioPorts[DEVIDER_REGISTER_ADDRESS - 0xFF00] > 0xFF)
		{
			_rc->ioPorts[DEVIDER_REGISTER_ADDRESS - 0xFF00] = 0;
		}

		_dividerCounter = 0;
	}

}
