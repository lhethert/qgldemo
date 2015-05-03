#include <QString>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include "Scene/helpers.h"
#include "shader.h"

namespace GLDemo
{
    Shader::Shader() :
        m_program(0)
    {
    }


    Shader::~Shader()
    {
        delete m_program;
    }


    /**
     * \param vertexShaderFileName The name of the vertex shader file to compile.
     * \param fragmentShaderFileName The name of the fragment shader to compile.
     * \return true if the shader program was compiled and linked successfully, false otherwise.
     */
    bool Shader::compileAndLink(const QString& vertexShaderFileName, const QString& fragmentShaderFileName)
    {
        if (!m_program)
        {
            m_program = new QGLShaderProgram();
        }

        // Make sure we remove any shaders that have already been attached to this program.
        // We also need to clear any uniforms we have indexed to make sure that we're only using what we need to.
        m_program->removeAllShaders();

        // We must have at a minimum a vertex and fragment shader
        if (vertexShaderFileName.isEmpty() ||
            fragmentShaderFileName.isEmpty())
        {
            std::cout << "ERROR: Must provide both a vertex and fragment shader when linking." << std::endl;
            return false;
        }

        if (!addShaderToProgram(vertexShaderFileName, QGLShader::Vertex))
        {
            return false;
        }

        if (!addShaderToProgram(fragmentShaderFileName, QGLShader::Fragment))
        {
            return false;
        }

        if (!m_program->link())
        {
            std::cout << "ERROR: Could not link shader program. Log follows:" << std::endl;
            std::cout << m_program->log() << std::endl;
            return false;
        }

        return true;
    }


    /**
     * \param filename  The filename of the shader program to load.
     * \param type      The type of shader to compile.
     *
     * Adds a shader to the shader program of this shader.
     *
     * \pre The internal shader program object must have been created prior to invoking this function.
     */
    bool Shader::addShaderToProgram(const QString& filename, QGLShader::ShaderType type)
    {
        assert(m_program);
        QString sourceText;
        if (!readShaderSource(filename, sourceText))
        {
            return false;
        }

        if (!m_program->addShaderFromSourceCode(type, sourceText))
        {
            std::cout << QString("ERROR: Could not add shader from source file \"%1\"").arg(filename) << std::endl;
            QStringList errors( m_program->log().split("\n", QString::SkipEmptyParts) );
            std::cout << errors.join("\n") << std::endl;;
            return false;
        }

        return true;
    }


    /**
     * \param sourceFileName The name of the source file to read.
     * \param sourceOut      A string to which the contents of the source file will be written.
     *
     * This function exists so that in the future, we can add additional processes here
     * that allow us to dynamically add additional components to shaders prior to compiling them.
     * For example, moving forward, we may want to add transform uniforms to every shader so that
     * we don't have to write it in every single one.
     */
    bool Shader::readShaderSource(const QString& sourceFileName, QString& sourceOut)
    {
        QFile sourceFile(sourceFileName);
        if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            std::cout << QString("ERROR: Could not read shader source from file \"%1\".").arg(sourceFileName) + "\n";
            return false;
        }

        QTextStream sourceStream(&sourceFile);
        sourceOut = sourceStream.readAll();
        sourceFile.close();
        return true;
    }
}
