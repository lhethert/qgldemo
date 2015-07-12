#include <sstream>
#include <iostream>

#include <QGLContext>
#include <QString>
#include <QtGlobal>

#if defined(Q_OS_MAC)
#  include <OpenGL/gl.h>
#else
#  include <gl.h>
#endif

#include "glutils.h"


namespace GLDemo
{
    /**
     * \return True if there are no OpenGL errors, false if otherwise (an error message
     *         will also be printed. Note that errors accumulate, so calling this may
     *         indicate an error occurred some time earlier.
     */
    bool glGoodState()
    {
        bool haveErrors = false;
        GLenum errCode = 0;
        int maxErrors = 20;
        int currentError = 0;
        while ((errCode = glGetError()) != GL_NO_ERROR)
        {
            QString errorReason;
            switch (errCode)
            {
            case GL_INVALID_ENUM:
                errorReason = "Invalid enumerant";
                break;
            case GL_INVALID_VALUE:
                errorReason = "Invalid value";
                break;
            case GL_INVALID_OPERATION:
                errorReason = "Invalid operation";
                break;
            case GL_STACK_OVERFLOW:
                errorReason = "Stack overflow";
                break;
            case GL_STACK_UNDERFLOW:
                errorReason = "Stack underflow";
                break;
            case GL_OUT_OF_MEMORY:
                errorReason = "Out of memory";
                break;
            case GL_TABLE_TOO_LARGE:
                errorReason = "Table too large";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorReason = "Invalid framebuffer operation";
                break;
            default:
                errorReason = "Unknown error code";
                break;
            }

            std::ostringstream oss;
            oss << QGLContext::currentContext();
            QString errorString(QString("ERROR: OpenGL error:\n%1\nContext:%2").arg(errorReason).arg(oss.str().c_str()));
            std::cout << errorString.toLatin1().constData() << std::endl;
            std::cerr << errorString.toLatin1().constData() << std::endl;
            haveErrors = true;

            if (currentError++ == maxErrors)
            {
                std::cerr << "Too Many OpenGL errors, giving up on wsglGoodState()" << std::endl;
                break;
            }
        }

        return !haveErrors;
    }
}
