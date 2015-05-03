#ifndef GLDEMO_SHADER_H
#define GLDEMO_SHADER_H

#include <QGLShaderProgram>
#include <QSharedPointer>

#include "Math/matrix4.h"

class QString;

namespace GLDemo
{
    class Renderer;

    /**
     * \brief Used for shading a model with a particular algorithm.
     */
    class Shader
    {
    public:
        ~Shader();

        /**
         * Activates a shader, causing models that are subsequently drawn to be
         * rendered with its effect.
         */
        virtual bool activate(const Matrix4f& view,
                              const Matrix4f& worldView,
                              const Matrix4f& worldViewInvTranspose,
                              const Matrix4f& worldViewProj) = 0;

    protected:
        QGLShaderProgram* m_program;

        Shader();
        bool compileAndLink(const QString& vertexShaderFilename,
                            const QString& fragmentShaderFilename);

    private:
        bool addShaderToProgram(const QString& fileName, QGLShader::ShaderType type);
        bool readShaderSource(const QString& sourceFileName, QString& sourceOut);
    };

    typedef QSharedPointer<Shader> PtrShader;
}

#endif
