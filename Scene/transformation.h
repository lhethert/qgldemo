#ifndef GLDEMO_TRANSFORMATION_H
#define GLDEMO_TRANSFORMATION_H

#include "Math/vector3.h"
#include "Math/matrix3.h"

namespace GLDemo
{

    template<class Real>
    class Matrix4;

    /**
     *
     */
    class Transformation
    {
    public:
        Transformation();
        Transformation(const Transformation&);
        ~Transformation();

        // Access and mutation functions
        void setRotation(const Matrix3f& m)  { m_rotation = m; m_isIdentity = false; }
        const Matrix3f& getRotation() const  { return m_rotation; }
        Matrix3f& getRotation()              { m_isIdentity = false; return m_rotation;  }

        void setTranslation(const Vector3f& t) { m_translation = t; m_isIdentity = false; }
        const Vector3f& getTranslation() const { return m_translation; }
        Vector3f& getTranslation()             { m_isIdentity = false; return m_translation; }

        void setScale(const Vector3f& s)  { m_scale = s; m_isUniformScale = false; m_isIdentity = false; }
        const Vector3f& getScale() const  { return m_scale; }
        Vector3f& getScale()              { m_isUniformScale = false; m_isIdentity = false; return m_scale; }

        void setUniformScale(float s)   { m_scale[0] = m_scale[1] = m_scale[2] = s; m_isUniformScale = true; m_isIdentity = false; }
        float getUniformScale() const   { return m_scale[0]; }

        bool isIdentity() const { return m_isIdentity; }

        void toMatrix(Matrix4<float>& m) const;
        void fromMatrix(const Matrix4<float>& m);

        // Application functions
        Vector3f apply(const Vector3f& v) const;
        Vector3f applyInverse(const Vector3f& v) const;
        void combine(const Transformation& m1, const Transformation& m2);

    private:
        Matrix3f m_rotation;
        Vector3f m_scale;
        Vector3f m_translation;

        bool m_isIdentity;
        bool m_isUniformScale;
    };

}

#endif
