#ifndef GLDEMO_MATRIX_N_H
#define GLDEMO_MATRIX_N_H

#include <cassert>
#include <iostream>

#include "mathdefs.h"


namespace GLDemo
{

    /**
     * Defines an NxN square matrix where each element is of the datatype 'Real'. Matrices are stored
     * in Column-Major order.
     *
     * Implements the Curiously Recurring Template Pattern; subclasses
     * to provide their types as a template parameter as well as their intended size. This allows functionality
     * common to all square matrices to be encapsulated by this class, and any pieces of varying functionality
     * to be implemented as fake virtual functions.
     *
     * No actual virtual functions are used; subclasses are only forced to implement 'IMPL' functions
     * called by the baseclass template functions, such as AdjointImpl and DeterminantImpl.
     */
    template<class Derived, class Real, int N>
    class MatrixN
    {
    public:
        MatrixN();
        MatrixN(const Real m[], bool transposeM = false);
        MatrixN(const MatrixN&);

        // Alternative constructor for creating an identiy matrix
        static Derived createIdentity();

        //Access operations
        Real  operator[](int i) const        { assert(i < TOTAL_ELEMENTS); return m_components[i]; }
        Real& operator[](int i)              { assert(i < TOTAL_ELEMENTS); return m_components[i]; }
        Real  operator()(int i, int j) const { assert(i < N && j < N); return m_components[i + j * N]; }
        Real& operator()(int i, int j)       { assert(i < N && j < N); return m_components[i + j * N]; }

        const Real* toPointer() const  { return (Real*)this; }
        Real* toPointer()              { return (Real*)this; }

        //Comparison operations
        bool operator==(const MatrixN& m) const;
        bool operator< (const MatrixN& m) const;
        bool operator<=(const MatrixN& m) const;
        bool operator> (const MatrixN& m) const;
        bool operator>=(const MatrixN& m) const;
        bool operator!=(const MatrixN& m) const;

        //Algebraic operations
        Derived operator+(const Derived& m) const;
        Derived operator-(const Derived& m) const;
        Derived operator*(const Derived& m) const;
        Derived operator*(const Real& s) const;

        Derived operator/(const Real& s) const;
        Derived operator-() const;

        Derived& operator+=(const Derived& m);
        Derived& operator-=(const Derived& m);
        Derived& operator*=(const Real& s);
        Derived& operator/=(const Real& s);

        void toRowEschelonForm ();
        void toReducedRowEschelonForm ();
        void toIdentity();
        void toZero();

        // Geometric operations
        Derived inverse() const;
        Derived transpose() const;
        Derived transposeTimes(const Derived& m) const;
        Derived timesTranspose(const Derived& m) const;

        Real trace() const;

        // "Virtual" functions (CRTP)
        Real determinant() const { return static_cast<const Derived*>(this)->determinantImpl(); }
        Derived adjoint() const { return static_cast<const Derived*>(this)->adjointImpl(); }
        Real frobeniusNorm() const;
        Real frobeniusNormSquared() const;

        // Handedness operations
        void rowMajor (Real* out) const;

        // Friend functions for commutative operations
        friend Derived operator* (const Real& s, const Derived& m) { return m * s; }
        friend std::ostream& operator<< (std::ostream& stream, const MatrixN<Derived, Real, N>& m)
        {
            stream << "{ " << std::endl;
            for (int i = 0; i < N; ++i)
            {
                for (int j = 0; j < N; ++j)
                {
                    stream << m(i,j) << ",\t";
                }
                stream << std::endl;
            }
            stream << "}" << std::endl;

            return stream;
        }

        static const int TOTAL_ELEMENTS = N * N;

    private:
        Real m_components[TOTAL_ELEMENTS];
    };


    /**
     * Constructs a new identity matrix.
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::createIdentity()
    {
        Derived result;
        for (int i = 0; i < TOTAL_ELEMENTS; i += N + 1)
        {
            result[i] = (Real)1.0;
        }
        return result;
    }


    /**
     * Initialises a new NxN matrix as a null (zero) matrix.
     */
    template<class Derived, class Real, int N>
    inline MatrixN<Derived, Real, N>::MatrixN()
    {
        std::memset(m_components, 0, sizeof(Real) * TOTAL_ELEMENTS);
    }


    /**
     * Create a new NxN matrix from the input array. If transposeM is set, create the new matrix as
     * the transpose of the original matrix.
     */
    template<class Derived, class Real, int N>
    MatrixN<Derived, Real, N>::MatrixN(const Real m[], bool transposeM)
    {
        if (transposeM)
        {
            //copy transpose
            for (int i = 0; i < N; ++i)
            {
                for (int j = 0; j < N; ++j)
                {
                    m_components[i + j * N] = m[i * N + j];
                }
            }
        }
        else
        {
            //no transpose, just copy as-if binary array
            std::memcpy(m_components, m, sizeof(Real) * TOTAL_ELEMENTS);
        }
    }


    /**
     * Creates a new copy of a MatrixN.
     */
    template<class Derived, class Real, int N>
    inline MatrixN<Derived, Real, N>::MatrixN(const MatrixN<Derived, Real, N>& m)
    {
        std::memcpy(m_components, m.toPointer(), sizeof(Real) * TOTAL_ELEMENTS);
    }


    /**
     *
     */
    template<class Derived, class Real, int N>
    inline bool MatrixN<Derived, Real, N>::operator==(const MatrixN<Derived, Real, N>& m) const
    {
        // Make sure we do correct floating point comparison here.
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            if (!Math<Real>::FEqual(m_components[i], m[i]))
            {
                return false;
            }
        }

        return true;
    }


    /**
     *
     */
    template<class Derived, class Real, int N>
    inline bool MatrixN<Derived, Real, N>::operator<(const MatrixN<Derived, Real, N>& m) const
    {
        return (std::memcmp(m_components, m.ToPointer(), sizeof(Real) * TOTAL_ELEMENTS) < 0);
    }


    /**
     *
     */
    template<class Derived, class Real, int N>
    inline bool MatrixN<Derived, Real, N>::operator<=(const MatrixN<Derived, Real, N>& m) const
    {
        return (std::memcmp(m_components, m.ToPointer(), sizeof(Real) * TOTAL_ELEMENTS) <= 0);
    }


    /**
     *
     */
    template<class Derived, class Real, int N>
    inline bool MatrixN<Derived, Real, N>::operator>(const MatrixN<Derived, Real, N>& m) const
    {
        return (std::memcmp(m_components, m.ToPointer(), sizeof(Real) * TOTAL_ELEMENTS) > 0);
    }


    /**
     *
     */
    template<class Derived, class Real, int N>
    inline bool MatrixN<Derived, Real, N>::operator>=(const MatrixN<Derived, Real, N>& m) const
    {
        return (std::memcmp(m_components, m.ToPointer(), sizeof(Real) * TOTAL_ELEMENTS) >= 0);
    }



    /**
     *
     */
    template<class Derived, class Real, int N>
    inline bool MatrixN<Derived, Real, N>::operator!=(const MatrixN<Derived, Real, N>& m) const
    {
        return !operator==(m);
    }



    /**
     *
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::operator+ (const Derived& m) const
    {
        Real result[TOTAL_ELEMENTS];
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            result[i] = m_components[i] + m[i];
        }

        return Derived(result, false);
    }


    /**
     *
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::operator- (const Derived& m) const
    {
        Real result[TOTAL_ELEMENTS];
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            result[i] = m_components[i] - m[i];
        }

        return Derived(result, false);
    }


    /**
     * Multiply this matrix with another. Multiplication is performed in 'column vector'
     * format (i.e. pre-multiplied), such that each cell value is determined by multiplying
     * the row of this matrix with the corresponding column of the provided matrix.
     *
     * \param m The matrix to premultiply with this matrix.
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::operator* (const Derived& m) const
    {
        Derived result;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                for (int k = 0; k < N; ++k)
                {
                    result[i + j * N] += (m_components[i + k * N] * m[k + j * N]);
                }
            }
        }

        return result;
    }


    /**
     * Multiply this matrix by a scalar value.
     *
     * \param s  The scalar value to multiply with.
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::operator* (const Real& s) const
    {
        Real result[TOTAL_ELEMENTS];
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            result[i] = m_components[i] * s;
        }

        return Derived(result, false);
    }


    /**
     * Divide this matrix by a scalar value.
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::operator/ (const Real& s) const
    {
        Real result[TOTAL_ELEMENTS];
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            result[i] = m_components[i] / s;
        }

        return Derived(result, false);
    }


    /**
     * Returns this matrix multiplied by the scalar -1.
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::operator- () const
    {
        Real result[TOTAL_ELEMENTS];
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            result[i] = -m_components[i];
        }

        return Derived(result, false);
    }


    /**
     * Add another matrix's values to this matrix.
     *
     * \param m The matrix being added to this matrix.
     * \return A reference to this matrix.
     */
    template<class Derived, class Real, int N>
    Derived& MatrixN<Derived, Real, N>::operator+= (const Derived& m)
    {
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            m_components[i] += m[i];
        }

        return *static_cast<Derived*>(this);
    }


    /**
     * Subtract another matrix's values from this matrix.
     *
     * \param m The matrix being added to this matrix.
     * \return A reference to this matrix.
     */
    template<class Derived, class Real, int N>
    Derived& MatrixN<Derived, Real, N>::operator-= (const Derived& m)
    {
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            m_components[i] -= m[i];
        }

        return *static_cast<Derived*>(this);
    }


    /**
     * Multiply this matrix by a scalar value.
     *
     * \param s The scalar value to multiply this matrix by.
     * \return A reference to this matrix.
     */
    template<class Derived, class Real, int N>
    Derived& MatrixN<Derived, Real, N>::operator*= (const Real& s)
    {
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            m_components[i] *= s;
        }

        return *static_cast<Derived*>(this);
    }


    /**
     * Divide this matrix by a scalar value.
     *
     * \param s The scalar value to divide this matrix by.
     * \return A reference to this matrix.
     */
    template<class Derived, class Real, int N>
    Derived& MatrixN<Derived, Real, N>::operator/= (const Real& s)
    {
        for (int i = 0; i < TOTAL_ELEMENTS; ++i)
        {
            m_components[i] /= s;
        }

        return *static_cast<Derived*>(this);
    }


    /**
     * Converts this matrix into row eschelon form. This is useful for computing
     * things like eigenvalues or in some cases determinants.
     */
    template<class Derived, class Real, int N>
    void MatrixN<Derived, Real, N>::toRowEschelonForm ()
    {
        int i = 0, j = 0;
        while (i < N && j < N)
        {
            int maxRow = i;

            // Find the row which has max value for the jth column
            for (int k = i + 1; k < N; ++k)
            {
                if (Math<Real>::FAbs((*this)(k,j)) > Math<Real>::FAbs((*this)(maxRow,j)))
                {
                    maxRow = k;
                }
            }

            // Do nothing if the highest value for the lower triangular section of this column is 0; move to the next column
            if (!Math<Real>::FEqual((*this)(maxRow,j),(Real)0.0))
            {
                // If maxRow has changed, swap the rows
                if (maxRow != i)
                {
                    for (int u = 0; u < N; ++u)
                    {
                        Real tVal = (*this)(i, u);
                        (*this)(i, u) = (*this)(maxRow, u);
                        (*this)(maxRow, u) = tVal;
                    }
                }

                // Divide maxRow by the relevant coefficient, (*this)(maxRow, j), to cause it to equal 1.
                Real scalar = (*this)(i,j);
                for (int u = 0; u < N; ++u) //u = j?
                {
                    (*this)(i, u) /= scalar;
                }

                // Subtract multiples of i from all other rows in the matrix
                for (int u = i + 1; u < N; ++u)
                {
                    Real factor = -(*this)(u,j);
                    for (int v = 0; v < N; ++v)
                    {
                        (*this)(u, v) += (factor * (*this)(i, v));
                    }
                }

                ++i;
            }
            ++j;
        }

    }


    /**
     * Converts this matrix into reduced row eschelon form. This is useful for computing things
     * like eigenvalues, solving linear equations or in some cases computing the determinant.
     */
    template<class Derived, class Real, int N>
    void MatrixN<Derived, Real, N>::toReducedRowEschelonForm ()
    {
        int i = 0, j = 0;
        while (i < N && j < N)
        {
            int maxRow = i;

            //find the row which has max value for the jth column
            for (int k = i + 1; k < N; ++k)
            {
                if (Math<Real>::FAbs((*this)(k,j)) > Math<Real>::FAbs((*this)(maxRow,j)))
                {
                    maxRow = k;
                }
            }

            //do nothing if the highest value for the lower triangular section of this column is 0; move to the next column
            if (!Math<Real>::FEqual((*this)(maxRow,j),(Real)0.0))
            {
                //if maxRow has changed, swap the rows
                if (maxRow != i)
                {
                    for (int u = 0; u < N; ++u)
                    {
                        Real tVal = (*this)(i, u);
                        (*this)(i, u) = (*this)(maxRow, u);
                        (*this)(maxRow, u) = tVal;
                    }
                }

                //divide maxRow by the relevant coefficient, (*this)(maxRow, j), to cause it to equal 1.
                Real scalar = (*this)(i,j);
                for (int u = 0; u < N; ++u) //u = j?
                {
                    (*this)(i, u) /= scalar;
                }

                //subtract multiples of i from all other rows in the matrix
                for (int u = 0; u < N; ++u)
                {
                    if (u != i)
                    {
                        Real factor = -(*this)(u,j);
                        for (int v = 0; v < N; ++v)
                        {
                            (*this)(u, v) += (factor * (*this)(i, v));
                        }

                    }
                }

                ++i;
            }
            ++j;
        }

    }


    /**
     * Convert this matrix into the identity matrix.
     */
    template<class Derived, class Real, int N>
    inline void MatrixN<Derived, Real, N>::toIdentity ()
    {
        std::memset(m_components, 0, sizeof(Real) * TOTAL_ELEMENTS);
        for (int i = 0; i < TOTAL_ELEMENTS; i += N + 1)
        {
                m_components[i] = (Real)1.0;
        }
    }


    /**
     * Set this matrix to the zero matrix.
     */
    template<class Derived, class Real, int N>
    inline void MatrixN<Derived, Real, N>::toZero ()
    {
        std::memset(m_components, 0, sizeof(Real) * TOTAL_ELEMENTS);
    }


    /**
     * Compute the inverse of this matrix and return it. Does not modify
     * this matrix.
     *
     * \return The inverse of this matrix.
     */
    template<class Derived, class Real, int N>
    inline Derived MatrixN<Derived, Real, N>::inverse() const
    {
        // Compute the determinant, then divide the adjoint matrix
        // by the determinant.
        Real det = determinant();
        assert(!Math<Real>::FEqual(det, 0.0f));
        return adjoint() / det;
    }


    /**
     * Compute the transpose of this matrix and return it. Does not modify
     * this matrix.
     *
     * \return The transpose of this matrix.
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::transpose() const
    {
        Derived transpose;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                transpose[i + j * N] = m_components[i * N + j];
            }
        }

        return transpose;
    }


    /**
     * Convenience function for computing the transpose of this matrix then
     * immediately multiplying it by the supplied matrix.
     *
     * \param m The matrix to multiply after transposing
     * \return The transpose of this matrix multiplied by \a m.
     * \sa timesTranspose()
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::transposeTimes(const Derived& m) const
    {
        Derived transposeThis;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                transposeThis[i + j * N] = m_components[i * N + j];
            }
        }

        return transposeThis * m;
    }


    /**
     * Convenience function for multiplying this matrix with another, then
     * computing the transpose.
     *
     * \param m The matrix to multiply by after transposing.
     * \return The transpose of the result of this * m
     * \sa transposeTimes
     */
    template<class Derived, class Real, int N>
    Derived MatrixN<Derived, Real, N>::timesTranspose(const Derived& m) const
    {
        Derived transposeM;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                transposeM[i + j * N] = m[i * N + j];
            }
        }

        return (*static_cast<const Derived*>(this)) * transposeM;
    }


    /**
     * \return The trace (sum of the diagonal) of this matrix.
     */
    template<class Derived, class Real, int N>
    Real MatrixN<Derived, Real, N>::trace() const
    {
        Real trace = 0;
        for (int i = 0; i < TOTAL_ELEMENTS; i += N + 1)
        {
            trace += m_components[i];
        }

        return trace;
    }


    /**
     * \return The frobenius norm of the matrix. This is primarily used in
     * polar decomposition.
     */
    template<class Derived, class Real, int N>
    Real MatrixN<Derived, Real, N>::frobeniusNorm () const
    {
        Real norm = (Real)0.0;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                int x = i + j * N;
                norm += m_components[x] * m_components[x];
            }
        }

        return Math<Real>::Sqrt(norm);
    }


    /**
     * \return The square of the frobenius norm of the matrix. This is primarily used in
     * polar decomposition.
     */
    template<class Derived, class Real, int N>
    Real MatrixN<Derived, Real, N>::frobeniusNormSquared() const
    {
        Real norm = (Real)0.0;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                int x = i + j * N;
                norm += m_components[x] * m_components[x];
            }
        }

        return norm;
    }


    /**
     * Copy the contents of this matrix into the supplied \a out
     * array, converting the data from column-major order to row-major
     * order.
     *
     * \param out Pointer to an array of floating-point data (already allocated).
     */
    template<class Derived, class Real, int N>
    void MatrixN<Derived, Real, N>::rowMajor(Real* out) const
    {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                out[i * N + j] = m_components[i + j * N];
            }
        }
    }

}


#endif
