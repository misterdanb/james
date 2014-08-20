#ifndef _Color_template_
#define _Color_template_

namespace james
{
	namespace core
	{
		template<class T>
		class Color : public Array<T, 3>
		{
		  public:
			Color()
			{
			}

			Color (T red, T green, T blue)
			{
				SetRed (red);
				SetGreen (green);
				SetBlue (blue);
			}

			~Color()
			{
			}

			void SetRed (T red)
			{
				(*this)[0] = red;
			}

			void SetGreen (T green)
			{
				(*this)[1] = green;
			}

			void SetBlue (T blue)
			{
				(*this)[2] = blue;
			}

			T GetRed() const
			{
				return (*this)[0];
			}

			T GetGreen() const
			{
				return (*this)[1];
			}

			T GetBlue() const
			{
				return (*this)[2];
			}
		};
	}
}

#endif
