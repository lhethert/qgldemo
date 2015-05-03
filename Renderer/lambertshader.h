#ifndef GLDEMO_LAMBERTSHADER_H
#define GLDEMO_LAMBERTSHADER_H

#include <QColor>
#include <QGLShaderProgram>
#include <QGLFunctions>

#include "shader.h"

namespace GLDemo
{
    class Renderer;

    /**
     * \brief Used for shading a model using a simple lambertian lighting model.
     */
    class LambertShader : public Shader, protected QGLFunctions
    {
    public:
        LambertShader();
        ~LambertShader();

        virtual bool activate(const Matrix4f& view,
                              const Matrix4f& worldView,
                              const Matrix4f& worldViewInvTranspose,
                              const Matrix4f& worldViewProj);

        void setColor(const QColor& color) { m_color = color; }

    private:
        QColor            m_color;

        int m_locMatWorldView;
        int m_locMatWorldViewProj;
        int m_locMatWorldViewInvTranspose;
        int m_locColor;
        int m_locLightPos;

        LambertShader(const LambertShader&);
        LambertShader& operator=(const LambertShader&);
    };

}

#endif
