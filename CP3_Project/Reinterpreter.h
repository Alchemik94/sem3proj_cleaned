#ifndef _REINTERPRETER_DECLARATION
#define _REINTERPRETER_DECLARATION

#include <vector>

namespace Application
{
	template <typename sourceClass, typename destinationClass>
	class VectorReinterpreter
	{
		public:
			std::vector<destinationClass*> Convert(std::vector<sourceClass*> src)
			{
				std::vector<destinationClass*> dst(src.size());
				for (int i = 0; i < src.size(); ++i)
					dst[i] = reinterpret_cast<destinationClass*>(src[i]);
				return dst;
			}
	};
}

#endif