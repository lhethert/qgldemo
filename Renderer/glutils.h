#ifndef GLDEMO_GLUTILS_H
#define GLDEMO_GLUTILS_H

namespace GLDemo
{
    // Basic error checking functions which can be used anywhere a valid context exists.
    bool glGoodState();
}

// Macros to ensure that the checks are only implemented in debug mode.
#ifndef NDEBUG
#   define GL_GOOD_STATE() glGoodState()
#else
#   define GL_GOOD_STATE() true
#endif

#endif
