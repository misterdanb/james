#ifndef _DMAMode_hpp_
#define _DMAMode_hpp_

namespace james
{
	namespace core
	{
		/*
		 * DMAMode indicates which DMA mode is used.
		 */
		
		enum class HDMAMode : int
		{
			GENERAL_PURPOSE_HDMA = 0,
			HBLANK_HDMA = 1
		};
	}
}

#endif
