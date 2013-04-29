#ifndef _Renderer_hpp_
#define _Renderer_hpp_

#include "RenderContext.hpp"

namespace gbc
{
	namespace core
	{
		class Renderer
		{
		public:
			virtual ~Renderer();
			
			virtual void RenderOAMSearch(RenderContext &);
			virtual void RenderTransferData(RenderContext &);
			virtual void RenderHorizontalBlank(RenderContext &);
			virtual void RenderVerticalBlank(RenderContext &);
		};
	}
}

#endif
