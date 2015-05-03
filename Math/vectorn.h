#ifndef GLDEMO_VECTORN_H
#define GLDEMO_VECTORN_H

#include <iostream>
#include <cassert>

#include "mathdefs.h"

namespace GLDemo
{

    /**
     * \brief The VectorN class encapsulates a vector in N dimensions. It is templated so that it can be
     * used with any numeric type. It also implements the Curiously Recurring Template Pattern,
     * in order to allow specific types (Vector2 / Vector3 etc) to inherit from this class without
     * having to define general vector arithmetic or virtual functions.
     */
    template<class Derived, class Real, int N>
    class VectorN
    {
    public:
        VectorN();
        VectorN(const VectorN& v);
        VectorN(const Real* v);

        Real  operator[](int i) const { assert(i < N); return m_components[i]; }
        Real& operator[](int i)       { assert(i < N); return m_components[i]; }

        Real*       toPointer()       { return (Real*)this; }
        const Real* toPointer() const { return (Real*)this; }

        //Comparison Operators
        bool operator== (const Derived& v) const;
        bool operator<  (const Derived& v) const;
        bool operator<= (const Derived& v) const;
        bool operator>  (const Derived& v) const;
        bool operator>= (const Derived& v) const;
        bool operator!= (const Derived& v) const;

        //Algebraic Operations
        Derived operator+ (const Derived& v) const;
        Derived operator- (const Derived& v) const;
        Derived operator* (const Real& s) const;
        Derived operator/ (const Real& s) const;
        Derived operator- () const;

        Derived& operator+= (const Derived& v);
        Derived& operator-= (const Derived& v);
        Derived& operator*= (const Real& s);
        Derived& operator/= (const Real& s);

        Derived& operator= (const Derived& v);

        //Geometric Operations
        Real length() const;
        Real squaredLength() const;
        Real dot(const VectorN& v) const;
        Real normalize();

        Real scalarProjection(const VectorN& v) const;
        Derived vectorProjection(const Derived& v, bool unitLengthV) const;
        Derived unitVector() const;

        void    toZero();

        friend Derived operator* (const Real& s, const Derived& v) { return v * s; }
        friend std::ostream& operator<<(std::ostream& stream, const Derived& v)
        {
            stream << "{ ";
            for (int i = 0; i < N; ++i)
            {
                stream << v[i] << " ";
            }
            return stream << " }" << std::endl;
        }

    private:
        Real m_components[N];
    };


    template<class Derived, class Real, int N>
    VectorN<Derived, Real, N>::VectorN()
    {
        std::memset(m_components, 0, sizeof(Real) * N);
    }


    template<class Derived, class Real, int N>
    VectorN<Derived, Real, N>::VectorN(const VectorN& v)
    {
        std::memcpy(m_components, v.toPointer(), sizeof(Real) * N);
    }


    template<class Derived, class Real, int N>
    VectorN<Derived, Real, N>::VectorN(const Real v[])
    {
        std::memcpy(m_components, v, sizeof(Real) * N);
    }



    template<class Derived, class Real, int N>
    inline bool VectorN<Derived, Real, N>::operator== (const Derived& v) const
    {
        for (int i = 0; i < N; ++i)
        {
            if (!Math<Real>::FEqual(m_components[i], v[i]))
            {
                return false;
            }
        }

        return true;
    }


    /**
     * Comparison performed by comparing all components of the vector as though they were
     * a large array of unsigned-bytes
     */
    template<class Derived, class Real, int N>
    inline bool VectorN<Derived, Real, N>::operator<(const Derived& v) const
    {
        return (std::memcmp(m_components, v.toPointer(), sizeof(Real) * N) < 0);
    }


    /**
     * Comparison performed by comparing all components of the vector as though they were
     * a large array of unsigned-bytes
     */
    template<class Derived, class Real, int N>
    inline bool VectorN<Derived, Real, N>::operator<=(const Derived& v) const
    {
        return (std::memcmp(m_components, v.toPointer(), sizeof(Real) * N) <= 0);
    }


    /**
     * Comparison performed by comparing all components of the vector as though they were
     * a large array of unsigned-bytes
     */
    template<class Derived, class Real, int N>
    inline bool VectorN<Derived, Real, N>::operator>(const Derived& v) const
    {
        return (std::memcmp(m_components, v.toPointer(), sizeof(Real) * N) > 0);
    }


    /**
     * Comparison performed by comparing all components of the vector as though they were
     * a large array of unsigned-bytes
     */
    template<class Derived, class Real, int N>
    inline bool VectorN<Derived, Real, N>::operator>=(const Derived& v) const
    {
        return (std::memcmp(m_components, v.toPointer(), sizeof(Real) * N) >= 0);
    }


    template<class Derived, class Real, int N>
    inline bool VectorN<Derived, Real, N>::operator!= (const Derived& v) const
    {
        return !operator==(v);
    }


    template<class Derived, class Real, int N>
    inline Derived VectorN<Derived, Real, N>::operator+ (const Derived& v) const
    {
        Real result[N];
        for (int i = 0; i < N; ++i)
        {
            result[i] = m_components[i] + v[i];
        }

        return Derived(result);
    }


    template<class Derived, class Real, int N>
    inline Derived VectorN<Derived, Real, N>::operator- (const Derived& v) const
    {
        Real result[N];
        for (int i = 0; i < N; ++i)
        {
            result[i] = m_components[i] - v[i];
        }

        return Derived(result);
    }


    template<class Derived, class Real, int N>
    inline Derived VectorN<Derived, Real, N>::operator* (const Real& s) const
    {
        Real result[N];
        for (int i = 0; i < N; ++i)
        {
            result[i] = m_components[i] * s;
        }

        return Derived(result);
    }


    template<class Derived, class Real, int N>
    inline Derived VectorN<Derived, Real, N>::operator/ (const Real& s) const
    {
        Real result[N];
        for (int i = 0; i < N; ++i)
        {
            result[i] = m_components[i] / s;
        }

        return Derived(result);
    }


    template<class Derived, class Real, int N>
    inline Derived VectorN<Derived, Real, N>::operator- () const
    {
        Real result[N];
        for (int i = 0; i < N; ++i)
        {
            result[i] = -m_components[i];
        }

        return Derived(result);
    }





    template<class Derived, class Real, int N>
    inline Derived& VectorN<Derived, Real, N>::operator+= (const Derived& v)
    {
        for (int i = 0; i < N; ++i)
        {
            m_components[i] += v[i];
        }

        return *static_cast<Derived*>(this);
    }


    template<class Derived, class Real, int N>
    inline Derived& VectorN<Derived, Real, N>::operator-= (const Derived& v)
    {
        for (int i = 0; i < N; ++i)
        {
            m_components[i] -= v[i];
        }

        return *static_cast<Derived*>(this);
    }


    template<class Derived, class Real, int N>
    inline Derived& VectorN<Derived, Real, N>::operator*= (const Real& s)
    {
        for (int i = 0; i < N; ++i)
        {
            m_components[i] *= s;
        }

        return *static_cast<Derived*>(this);
    }


    template<class Derived, class Real, int N>
    inline Derived& VectorN<Derived, Real, N>::operator/= (const Real& s)
    {
        for (int i = 0; i < N; ++i)
        {
            m_components[i] /= s;
        }

        return *static_cast<Derived*>(this);
    }


    template<class Derived, class Real, int N>
    inline Derived& VectorN<Derived, Real, N>::operator= (const Derived& v)
    {
        for (int i = 0; i < N; ++i)
        {
            m_components[i] = v.m_components[i];
        }

        return *static_cast<Derived*>(this);
    }


    template<class Derived, class Real, int N>
    inline Real VectorN<Derived, Real, N>::length() const
    {
        Real total = Math<Real>::Pow(m_components[0], 2);
        for (int i = 1; i < N; ++i)
        {
            total += Math<Real>::Pow(m_components[i], 2);
        }
        return Math<Real>::Sqrt(total);
    }


    template<class Derived, class Real, int N>
    inline Real VectorN<Derived, Real, N>::squaredLength() const
    {
        Real total = Math<Real>::Pow(m_components[0], 2);
        for (int i = 1; i < N; ++i)
        {
            total += Math<Real>::Pow(m_components[i], 2);
        }
        return total;
    }


    template<class Derived, class Real, int N>
    inline Real VectorN<Derived, Real, N>::dot(const VectorN& v) const
    {
        Real total = m_components[0] * v[0];
        for (int i = 1; i < N; ++i)
        {
            total += m_components[i] * v[i];
        }
        return total;
    }


    /**
     * Normalize the vector (cause it to become unit-length). Returns the length used to normalize the vector.
     */
    template<class Derived, class Real, int N>
    inline Real VectorN<Derived, Real, N>::normalize()
    {
        Real origLength = length();
        if (Math<Real>::FEqual(origLength, (Real)0.0))
        {
            return (Real)0.0;
        }

        *this /= origLength;
        return origLength;
    }


    template<class Derived, class Real, int N>
    inline Real VectorN<Derived, Real, N>::scalarProjection(const VectorN& v) const
    {
        return dot(v / v.length());
    }



    template<class Derived, class Real, int N>
    inline Derived VectorN<Derived, Real, N>::vectorProjection(const Derived& v, bool unitLengthV) const
    {
        Derived w(v);
        if (!unitLengthV)
        {
            w.normalize();
        }

        return w * dot(w);
    }


    template<class Derived, class Real, int N>
    void VectorN<Derived, Real, N>::toZero()
    {
        memset(m_components, 0, sizeof(Real) * N);
    }


    /**
     *
     */
    template<class Derived, class Real, int N>
    Derived VectorN<Derived, Real, N>::unitVector() const
    {
        Real origLength = length();
        if (Math<Real>::FEqual(origLength, (Real)0.0))
        {
            return *static_cast<const Derived*>(this);
        }

        return *static_cast<const Derived*>(this) / origLength;
    }

}

#endif
