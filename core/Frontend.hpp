#ifndef _Frontend_hpp_
#define _Frontend_hpp_

#include "GameboyColor.hpp"
#include "LCD.hpp"
#include "Joypad.hpp"

namespace james
{
	namespace core
	{
		class Frontend : public LCD, public Joypad
		{
		  public:
			Frontend();
			virtual ~Frontend();

			/* Initialize the emulation. */
			void Initialize();

			/* Finalize the emulation. */
			void Finalize();

			/* Reset the emulation */
			void Reset();

			/* Load a rom from file. */
			void LoadRom (std::string);

			/* Start the emulation */
			void Start();

			/* Pause the emulation */
			void Pause();

			/* Check if the emulation is paused. */
			bool IsPaused();

			/* Render the next frame. */
			void RenderFrame();

			/* Signal that a button has been pressed or serial
			 * data has been received to make an interrupt occur. */
			void SignalJoypadInterrupt();
			void SignalSerialInterrupt();

			/* Offers the actual emulation device to operatore
			 * on it and do debugging an such things. */
			GameboyColor& GetDevice();

			/* callback method to draw the rendered frame */
			virtual void DrawFrame (Frame&) = GBC_NULL;

			/* Methods to read the current state of the joypad.
			 * State should be holded for more than just one
			 * read to guarantee a correct emulation */
			virtual int GetRight() = 0;
			virtual int GetLeft() = 0;
			virtual int GetUp() = 0;
			virtual int GetDown() = 0;
			virtual int GetButtonA() = 0;
			virtual int GetButtonB() = 0;
			virtual int GetSelect() = 0;
			virtual int GetStart() = 0;

		  private:
			GameboyColor _gbc;
		};
	}
}

#endif
