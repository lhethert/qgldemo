#include <iostream>

#include "Math/matrix4.h"
#include "transformation.h"

namespace GLDemo
{

    /**
     * Constructs a new default transformation that effectively corresponds to
     * a 4x4 affine transformation matrix set to the identity.
     */
    Transformation::Transformation() :
        m_rotation(Matrix3f::createIdentity()),
        m_scale(1.0f, 1.0f, 1.0f),
        m_translation(),
        m_isIdentity(true),
        m_isUniformScale(true)
    {
    }


    /**
     *
     */
    Transformation::Transformation(const Transformation& trans) :
        m_rotation(trans.m_rotation),
        m_scale(trans.m_scale),
        m_translation(trans.m_scale),
        m_isIdentity(trans.m_isIdentity),
        m_isUniformScale(trans.m_isUniformScale)
    {
    }


    /**
     *
     */
    Transformation::~Transformation()
    {
    }


    /**
     * \param v The vector to pre-multiply with this transformation.
     *
     * Applies this transformation to the specified vector \a v. This is the equivalent
     * of multiplying the vector by a 4x4 affine transformation matrix. The order of
     * operations is scale, rotate (pre-multiply with a rotation matrix), then translate.
     *
     * \sa applyInverse
     */
    Vector3f Transformation::apply(const Vector3f& v) const
    {
        if (m_isIdentity)
        {
            return v;
        }

        Vector3f outV(v);
        outV.x() *= m_scale.x();
        outV.y() *= m_scale.y();
        outV.z() *= m_scale.z();
        outV = m_rotation * outV;
        outV += m_translation;
        return outV;
    }



    /**
     * \param v The vector to apply this transformation to.
     *
     * Convenience function for applying the inverse of this transformation to
     * the specified vector \a v.
     *
     * \sa apply
     */
    Vector3f Transformation::applyInverse(const Vector3f& v) const
    {
        if (m_isIdentity)
        {
            return v;
        }

        // Easy calculation if uniform scale - no inverse required
        if (m_isUniformScale)
        {
            return (m_rotation.transpose() * (v - m_translation)) / m_scale.x();
        }

        // More complex calculation for non-uniform scale - instead of calculating normal inverse, use knowledge
        // of the fact that m_scale is a diagonal matrix to compute inverse more efficiently (more multiplications
        // than divisions)
        Vector3f outV = (m_rotation.transpose() * (v - m_translation));
        float sXY = m_scale.x() * m_scale.y();
        float sYZ = m_scale.y() * m_scale.z();
        float sXZ = m_scale.x() * m_scale.z();
        float sInvDet = 1 / (sXY * m_scale.z());
        outV.x() *= sInvDet * sYZ;
        outV.y() *= sInvDet * sXZ;
        outV.z() *= sInvDet * sXY;
        return outV;
    }


    /**
     * \param m1 The first transformation to combine.
     * \param m2 The second transformation.
     *
     * Combines transformation by as though multiplying 2 4x4 homogenous matrices.
     * Order of operations is essentially (tOut = tr1 + r1 * s2 * tr2)
     */
    void Transformation::combine(const Transformation& m1, const Transformation& m2)
    {
        // If identity, no need to combine anything - just take the second transform
        if (m1.m_isIdentity)
        {
            if (m2.m_isIdentity)
            {
                return;
            }

            m_scale = m2.m_scale;
            m_rotation = m2.m_rotation;
            m_translation = m2.m_translation;

            m_isIdentity = false;
            m_isUniformScale = m2.m_isUniformScale;

            return;
        }

        // Assuming uniform scale, so r = r1 * r2 and s = s1 * s2
        m_scale.x() = m1.m_scale.x() * m2.m_scale.x();
        m_scale.y() = m1.m_scale.y() * m2.m_scale.y();
        m_scale.z() = m1.m_scale.z() * m2.m_scale.z();

        m_rotation = m1.m_rotation * m2.m_rotation;

        // t = t1 + r1s2t2
        m_translation = m1.m_translation;
        Vector3f temp = m2.m_translation;
        temp.x() *= m1.m_scale.x();
        temp.y() *= m1.m_scale.y();
        temp.z() *= m1.m_scale.z();
        temp = m1.m_rotation * temp;
        m_translation += temp;

        m_isIdentity = false;
        m_isUniformScale = m1.m_isUniformScale | m2.m_isUniformScale;
    }


    void Transformation::toMatrix(Matrix4f& m) const
    {
        Matrix3f s;
        s(0,0) = m_scale[0];
        s(1,1) = m_scale[1];
        s(2,2) = m_scale[2];

        // TODO: Move the below into a function for Matrix4
        Matrix3f rs = m_rotation * s;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                m(i,j) = rs(i,j);
            }
        }

        m(0,3) = m_translation[0];
        m(1,3) = m_translation[1];
        m(2,3) = m_translation[2];

        m(3,3) = 1.0f;
    }


    /**
     * Loads the transformation from the provided 4x4 homogeneous matrix
     */
    void Transformation::fromMatrix(const Matrix4f& m)
    {
        Matrix3f scaleMat;
        Matrix3f rotMat;
        Vector3f vTranslate;

        // We make the assumption of uniform scale. Very difficult to decompose otherwise.
        m.polarDecomposition(scaleMat, m_rotation, m_translation);
        m_scale.x() = scaleMat(0,0);
        m_scale.y() = scaleMat(1,1);
        m_scale.z() = scaleMat(2,2);
        m_isUniformScale = true;
        m_isIdentity = false;
    }
}

