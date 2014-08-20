#ifndef _BackgroundAttribute_hpp_
#define _BackgroundAttribute_hpp_

#include "James.hpp"
#include "HorizontalFlip.hpp"
#include "VerticalFlip.hpp"
#include "BackgroundToOAMPriority.hpp"

namespace james
{
	namespace core
	{
		/**
		 * @class BackgroundAttribute
		 *
		 * BackgroundAttribute represents a one of the background
		 * attributes located in the second bank of the video ram of
		 * a game boy color:
		 *
		 * <code>
		 * Bit 0-2: background color palette number<br />
		 * Bit 3: tile video ram bank number (0 = bank 0, 1 = bank 1)<br />
		 * Bit 4: not used<br />
		 * Bit 5: horizontal flip (0 = normal, 1 = horizontally flipped)<br />
		 * Bit 6: vertical flip (0 = normal, 1 = vertically flipped)<br />
		 * Bit 7: background to oam priority (0 = use oam priority bit, 1 = background priority)
		 * </code>
		 */
		class BackgroundAttribute
		{
		  public:
			/**
			 * Constructor of BackgroundAttribute.
			 *
			 * Initializes with the default values.
			 */
			BackgroundAttribute();

			/**
			 * Copy constructor of BackgroundAttribute.
			 *
			 * Initializes with the given background attribute.
			 *
			 * @param backgroundAttributeData Reference of the
			 *        background attribute to be used for
			 *        initialization.
			 */
			BackgroundAttribute (const BackgroundAttribute& backgroundAttribute);

			/**
			 * Constructor of BackgroundAttribute.
			 *
			 * Initializes with the given raw data.
			 *
			 * @param backgroundAttributeData The raw data to be used
			 *        for initialization.
			 */
			BackgroundAttribute (int backgroundAttributeData);

			/**
			 * Destructor of BackgroundAttribute.
			 */
			~BackgroundAttribute() = default;

			/**
			 * Modifies the raw data.
			 *
			 * @param backgroundAttributeData New raw data.
			 */
			void SetData (int backgroundAttributeData);

			/**
			 * Modifies the background color pallete number.
			 *
			 * @param backgroundColorPaletteNumber New the background
			 *        color palette number.
			 */
			void SetBackgroundColorPaletteNumber (int backgroundColorPaletteNumber);

			/**
			 * Modifies the tile video ram bank number.
			 *
			 * @param tileVideoRamBankNumber New tile video ram bank
			 *        number.
			 */
			void SetTileVideoRamBankNumber (int tileVideoRamBankNumber);

			/**
			 * Modifies the horizontal flip.
			 *
			 * @param horizontalFlip New horizontal flip.
			 */
			void SetHorizontalFlip (HorizontalFlip horizontalFlip);

			/**
			 * Modifies the vertical flip.
			 *
			 * @param verticalFlip New vertical flip.
			 */
			void SetVerticalFlip (VerticalFlip verticalFlip);

			/**
			 * Modifies the background to oam priority.
			 *
			 * @param backgroundToOAMPriority New background to oam
			 *        priority.
			 */
			void SetBackgroundToOAMPriority (BackgroundToOAMPriority backgroundToOAMPriority);

			/**
			 * Returns the raw data.
			 *
			 * @return The raw data.
			 */
			int GetData() const;

			/**
			 * Returns the background color palette number.
			 *
			 * @return The background color palette number.
			 */
			int GetBackgroundColorPaletteNumber() const;

			/**
			 * Returns the tile video ram bank number.
			 *
			 * @return The tile video ram bank number.
			 */
			int GetTileVideoRamBankNumber() const;

			/**
			 * Returns the horizontal flip.
			 *
			 * @return The horizontal flip.
			 */
			HorizontalFlip GetHorizontalFlip() const;

			/**
			 * Returns the vertical flip.
			 *
			 * @return The vertical flip.
			 */
			VerticalFlip GetVerticalFlip() const;

			/**
			 * Returns the background to oam priority.
			 *
			 * @return The background to oam priority.
			 */
			BackgroundToOAMPriority GetBackgroundToOAMPriority() const;

		  private:
			int _backgroundColorPaletteNumber;
			int _tileVideoRamBankNumber;
			HorizontalFlip _horizontalFlip;
			VerticalFlip _verticalFlip;
			BackgroundToOAMPriority _backgroundToOAMPriority;
		};
	}
}

#endif
