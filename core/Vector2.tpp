#ifndef _Vector2_template_
#define _Vector2_template_

namespace james
{
	namespace core
	{
		template<class T>
		class Vector2 : public Array<T, 2>
		{
		public:
			Vector2(T x, T y)
			{
				SetX(x);
				SetY(y);
			}
			
			~Vector2()
			{
			}
			
			void SetX(T x)
			{
				(*this)[0] = x;
			}
			
			void SetY(T y)
			{
				(*this)[1] = y;
			}
			
			T GetX() const
			{
				return (*this)[0];
			}
			
			T GetY() const
			{
				return (*this)[1];
			}
		};
	}
}

#endif
