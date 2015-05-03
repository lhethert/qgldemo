#ifndef GLDEMO_VECTOR3_H
#define GLDEMO_VECTOR3_H

#include "mathdefs.h"
#include "vectorn.h"

namespace GLDemo
{

    /**
     * The Vector3 class encapsulates a vector in 3 dimensions. It is templated so that it can be
     * used with any numeric type, notably floats or doubles.
     */
    template<class Real>
    class Vector3 : public VectorN<Vector3<Real>, Real, 3>
    {
        typedef VectorN<Vector3<Real>, Real, 3> VectorBase;

    public:
        Vector3();
        Vector3(const Real v[]);
        Vector3(const Real& x, const Real& y, const Real& z);

        //Access Operations
        Real  x() const	{ return VectorBase::operator[](0); }
        Real& x()		{ return VectorBase::operator[](0); }
        Real  y() const	{ return VectorBase::operator[](1); }
        Real& y()		{ return VectorBase::operator[](1); }
        Real  z() const	{ return VectorBase::operator[](2); }
        Real& z()		{ return VectorBase::operator[](2); }

        //Geometric Operations Specific to 3D
        Vector3 cross(const Vector3& v) const;
        Vector3 unitCross(const Vector3& v) const;

        static void orthonormalize(Vector3& u, Vector3& v, Vector3& w);
        static void generateOrthonormalBasis(Vector3& u, Vector3& v, Vector3& w, bool unitLengthV);

    };

    typedef Vector3<float> Vector3f;
    typedef Vector3<double> Vector3d;

    template<class Real>
    Vector3<Real>::Vector3() :
        VectorBase()
    {
    }


    template<class Real>
    Vector3<Real>::Vector3(const Real v[]) :
        VectorBase(v)
    {
    }


    template<class Real>
    Vector3<Real>::Vector3(const Real& vx, const Real& vy, const Real& vz) :
        VectorBase()
    {
        x() = vx;
        y() = vy;
        z() = vz;
    }


    template<class Real>
    inline Vector3<Real> Vector3<Real>::cross(const Vector3<Real>& v) const
    {
        return Vector3((*this)[1] * v[2] - (*this)[2] * v[1],
                       (*this)[2] * v[0] - (*this)[0] * v[2],
                       (*this)[0] * v[1] - (*this)[1] * v[0]);
    }

    template<class Real>
    inline Vector3<Real> Vector3<Real>::unitCross(const Vector3& v) const
    {
        Vector3<Real> w = cross(v);
        w.normalize();
        return w;
    }


    /**
     * Orthonormalises the three input vectors in-place using Gramm-Schmidt orthonormalization.
     * 'u' is used as the primary vector from which the orthonormal basis is generated.
     *
     * \param u First output vector
     * \param v Second output vector
     * \param w Input vector from which to generate the basis. Will also be the third output vector
     */
    template<class Real>
    void Vector3<Real>::orthonormalize(Vector3<Real>& u, Vector3<Real>& v, Vector3<Real>& w)
    {
        u.normalize();
        v = (v - v.vectorProjection(u, true));
        v.normalize();
        w = (w - w.vectorProjection(u, true) - w.vectorProjection(v, true));
        w.normalize();
    }


    /**
     * Generates an orthonormal basis from the input vector w.
     * </summary>
     * \param u First output vector
     * \param v Second output vector
     * \param w Input vector from which to generate the basis. Will also be the third output vector
     * \param unitLengthW Indictaes whether W is unit length or not, and therefore if normalization need occur.</param>
     */
    template<class Real>
    void Vector3<Real>::generateOrthonormalBasis(Vector3<Real>& u, Vector3<Real>& v, Vector3<Real>& w, bool unitLengthW)
    {
        if (!unitLengthW)
        {
            u = w / w.length();
        }

        if (w.x() > w.y())
        {
            v.x() = -w.z();
            v.y() = 0;
            v.z() = w.x();
        }
        else
        {
            v.x() = 0;
            v.y() = -w.z();
            v.z() = w.y();
        }

        v /= v.length();
        w = u.cross(v);
    }

}

#endif //Vector3_H
