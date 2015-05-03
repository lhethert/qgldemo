#ifndef GLDEMO_VECTOR4_H
#define GLDEMO_VECTOR4_H

#include "vectorn.h"


namespace GLDemo
{
    /**
     * The Vector4 class encapsulates a vector with 4 components; x, y, z, w. It is
     * not designed to represent a 4 dimensional vector, rather, it is designed to allow a
     * 3 dimensional vector to be represented in homogeneous coordinates.
     */
    template<class Real>
    class Vector4 : public VectorN<Vector4<Real>, Real, 4>
    {
       typedef VectorN<Vector4<Real>, Real, 4> VectorBase;

    public:
        Vector4();
        Vector4(const Real v[]);
        Vector4(const Real& x, const Real& y, const Real& z, const Real& w);

        //Access Operations
        Real  x() const { return VectorBase::operator[](0); }
        Real& x()       { return VectorBase::operator[](0); }
        Real  y() const { return VectorBase::operator[](1); }
        Real& y()       { return VectorBase::operator[](1); }
        Real  z() const { return VectorBase::operator[](2); }
        Real& z()       { return VectorBase::operator[](2); }
        Real  w() const { return VectorBase::operator[](3); }
        Real& w()       { return VectorBase::operator[](3); }

        //Geometric Operations Specific to 3D
        Vector4 cross(const Vector4& v) const;
        Vector4 unitCross(const Vector4& v) const;

    };

    typedef Vector4<float> Vector4f;
    typedef Vector4<double> Vector4d;


    template<class Real>
    Vector4<Real>::Vector4() :
        VectorBase()
    {
    }


    template<class Real>
    Vector4<Real>::Vector4(const Real v[]) :
        VectorBase(v)
    {
    }


    template<class Real>
    Vector4<Real>::Vector4(const Real& vx, const Real& vy, const Real& vz, const Real& vw) :
        VectorBase()
    {
        x() = vx;
        y() = vy;
        z() = vz;
        w() = vw;
    }


    template<class Real>
    inline Vector4<Real> Vector4<Real>::cross(const Vector4<Real>& v) const
    {
        return Vector4((*this)[1] * v[2] - (*this)[2] * v[1],
                       (*this)[2] * v[0] - (*this)[0] * v[2],
                       (*this)[0] * v[1] - (*this)[1] * v[0],
                       0.0);
    }


    template<class Real>
    inline Vector4<Real> Vector4<Real>::unitCross(const Vector4& v) const
    {
        Vector4<Real> w = cross(v);
        w.normalize();

        return w;
    }


}

#endif
