#ifndef _NonCopyable_hpp_
#define _NonCopyable_hpp_

namespace james
{
    namespace core
    {
        /**
         * @class NonCopyable
         * 
         * NonCopyable is an abstract implementation for all classes
         * that must not be copied.
         */
        class NonCopyable
        {
        public:
            NonCopyable(const NonCopyable&) = delete;
            NonCopyable& operator=(const NonCopyable&) = delete;
        };
    }
}

#endif
