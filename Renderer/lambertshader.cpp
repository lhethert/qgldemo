#include "lambertshader.h"
#include "glutils.h"
#include "glrenderer.h"

namespace GLDemo
{
    LambertShader::LambertShader() :
        Shader(),
        m_color(255, 0, 0, 255),
        m_locMatWorldView(-1),
        m_locMatWorldViewProj(-1),
        m_locMatWorldViewInvTranspose(-1),
        m_locColor(-1),
        m_locLightPos(-1)
    {
    }


    LambertShader::~LambertShader()
    {
    }


    bool LambertShader::activate(const Matrix4f& view,
                                 const Matrix4f& worldView,
                                 const Matrix4f& worldViewInvTranspose,
                                 const Matrix4f& worldViewProj)
    {
        if (!m_program)
        {
            initializeGLFunctions();

            if (!compileAndLink(":/shaders/lambertshader.vert", ":/shaders/lambertshader.frag"))
            {
                return false;
            }
            assert(m_program->isLinked());

            // Bind the vertex attribute locations
            m_program->bindAttributeLocation("vertPosition", GLRenderer::Position);
            m_program->bindAttributeLocation("vertNormal", GLRenderer::Normal);

            // Store the uniform locations so we don't have to look them up each time.
            m_program->bind();
            m_locMatWorldView = m_program->uniformLocation("matWorldView");
            m_locMatWorldViewProj = m_program->uniformLocation("matWorldViewProj");
            m_locMatWorldViewInvTranspose = m_program->uniformLocation("matWorldViewInvTranspose");
            m_locColor = m_program->uniformLocation("diffuseColor");
            m_locLightPos = m_program->uniformLocation("lightPos");
        }

        // Set uniform values. Use the native GL functions for matrices, as Qt doesn't
        // appear to offer us an equivalent function for a matrix.
        m_program->bind();
        glUniformMatrix4fv(m_locMatWorldView, 1, false, worldView.toPointer());
        glUniformMatrix4fv(m_locMatWorldViewInvTranspose, 1, false, worldViewInvTranspose.toPointer());
        glUniformMatrix4fv(m_locMatWorldViewProj, 1, false, worldViewProj.toPointer());
        m_program->setUniformValue(m_locColor, m_color);
        Vector4f lightPos(view * Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
        glUniform3f(m_locLightPos, lightPos.x(), lightPos.y(), lightPos.z());
        return GL_GOOD_STATE();
    }

}
