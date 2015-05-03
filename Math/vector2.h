#ifndef GLDEMO_VECTOR2_H
#define GLDEMO_VECTOR2_H

#include "vectorn.h"

namespace GLDemo
{
    /**
     * \brief The Vector2 class represents a 2 dimensional vector.
     */
    template<class Real>
    class Vector2 : public VectorN<Vector2<Real>, Real, 2>
    {
        typedef VectorN<Vector2<Real>, Real, 2> VectorBase;

    public:
        Vector2();
        Vector2(const Real v[]);
        Vector2(const Real& x, const Real& y);

        //Access Operations
        Real  x() const { return VectorBase::operator[](0); }
        Real& x()       { return VectorBase::operator[](0); }
        Real  y() const { return VectorBase::operator[](1); }
        Real& y()       { return VectorBase::operator[](1); }

        //Geometric Operations Specific to 2D
        Vector2 perp() const;
        Vector2 unitPerp() const;

        static void orthonormalize(Vector2& u, Vector2& v);
        static void generateOrthonormalBasis(Vector2& u, Vector2& v, bool unitLengthV);

    };

    typedef Vector2<float> Vector2f;
    typedef Vector2<double> Vector2d;

    template<class Real>
    inline Vector2<Real>::Vector2() :
        VectorBase()
    {
    }


    template<class Real>
    inline Vector2<Real>::Vector2(const Real v[]) :
        VectorBase(v)
    {
    }


    template<class Real>
    inline Vector2<Real>::Vector2(const Real& vx, const Real& vy) :
        VectorBase()
    {
        x() = vx;
        y() = vy;
    }


    /**
     * Returns a perpendicular vector.
     */
    template<class Real>
    inline Vector2<Real> Vector2<Real>::perp() const
    {
        Vector2 w(*this);
        w[0] = VectorBase::operator[](1);
        w[1] = -VectorBase::operator[](0);
        return w;
    }


    /**
     * Returns a unit-length perpendicular vector.
     */
    template<class Real>
    inline Vector2<Real> Vector2<Real>::unitPerp() const
    {
        Vector2<Real> w = perp();
        w.normalize();

        return w;
    }


    /**
     * Performs in-place orthonormalization of the input-vectors, based on vector 'u',
     * using Gramm-Schmidt orthonormalization.
     */
    template<class Real>
    void Vector2<Real>::orthonormalize(Vector2<Real>& u, Vector2<Real>& v)
    {
        u.normalize();
        v = (v - v.vectorProjection(u));
        v.normalize();
    }


    /**
     * Computes a orthonormal basis based on the input vector 'v'.
     *
     * \param u First output vector
     * \param v Input vector upon which orthonormal basis is calculated.
     * \param unitLengthV Hint to indicate whether 'u' is already of unit-length, and hence does not require
     *        normalization.
     */
    template<class Real>
    void Vector2<Real>::generateOrthonormalBasis(Vector2<Real>& u, Vector2<Real>& v, bool unitLengthV)
    {
        if (!unitLengthV)
        {
            v.normalize();
        }

        u = v.perp();
    }
}

#endif
