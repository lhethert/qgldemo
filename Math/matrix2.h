#ifndef GLDEMO_MATRIX2_H
#define GLDEMO_MATRIX2_H

#include "matrixn.h"
#include "vector2.h"

namespace GLDemo
{

    /**
     * Defines a square matrix in 2 dimensions (2x2).
     */
    template<class Real>
    class Matrix2 : public MatrixN<Matrix2<Real>, Real, 2>
    {
        typedef MatrixN<Matrix2<Real>, Real, 2> MatrixBase;

    public:
        Matrix2();
        Matrix2(const Real m[], bool transposeM);
        Matrix2(const Matrix2&);

        //Algebraic operations specific to 2D
        using MatrixN<Matrix2<Real>, Real, 2>::operator*;
        Vector2<Real> operator*(const Vector2<Real>& v) const;

        //Geometric operations specific to 2D
        Real determinantImpl() const;
        Matrix2 adjointImpl() const;

        void toAngle (Real& angle) const;
        void diagonalization (Matrix2& rot, Matrix2& diag) const;
        void orthonormalize ();

    };

    typedef Matrix2<float> Matrix2f;
    typedef Matrix2<double> Matrix2d;


    /**
     * Initialises a new 2x2 matrix as a zero matrix.
     */
    template<class Real>
    Matrix2<Real>::Matrix2() :
        MatrixBase()
    {
    }


    /**
     * Create a new 2x2 matrix from the input array. If transposeM is set, create the new matrix as
     * the transpose of the original matrix.
     */
    template<class Real>
    Matrix2<Real>::Matrix2(const Real m[], bool transposeM) :
        MatrixBase(m, transposeM)
    {
    }


    /**
     *
     */
    template<class Real>
    Matrix2<Real>::Matrix2(const Matrix2<Real>& m) :
        MatrixBase(m)
    {
    }


    /**
     * Pre-multiplies a vector with this matrix. The vector is treated as a column
     * vector.
     *
     * \param v The vector to pre-multiply with this matrix.
     */
    template<class Real>
    Vector2<Real> Matrix2<Real>::operator* (const Vector2<Real>& v) const
    {
        Vector2<Real> result;
        for (int row = 0; row < 2; ++row)
        {
            for (int col = 0; col < 2; ++col)
            {
                result[row] += (*this)(row, col) * v[col];
            }
        }

        return result;
    }


    /**
     * Computes the determinant of this 2x2 matrix.
     */
    template<class Real>
    Real Matrix2<Real>::determinantImpl() const
    {
        return (*this)[0] * (*this)[3] - (*this)[2] * (*this)[1];
    }


    /**
     * Computes the adjoint matrix (transpose of the matrix of cofactors).
     */
    template<class Real>
    Matrix2<Real> Matrix2<Real>::adjointImpl() const
    {
        Matrix2<Real> result;
        result[0] = (*this)[3];
        result[1] = -(*this)[1];
        result[2] = -(*this)[2];
        result[3] = (*this)[0];
        return result;
    }


    /**
     *
     */
    template<class Real>
    void Matrix2<Real>::toAngle(Real& angle) const
    {
        angle = Math<Real>::ATan2((*this)[0], (*this)[1]);
    }


    /**
     * Performs Eigen-Decomposition on the matrix in order to Diagonalize it. The results are a
     * matrix 'rot', each of the columns of which are a normalized Eigenvector of the matrix, and a matrix
     * 'diag', which has the corresponding eigenvalues along the main diagonal.
     *
     * The equation A^-1 * M * A, where A is the matrix 'rot', will produce matrix Diag.
     */
    template<class Real>
    void Matrix2<Real>::diagonalization(Matrix2<Real>& rot, Matrix2<Real>& diag) const
    {
        //Eigenvalues are the roots of the equation obtained by expanding Det(M - IL),
        //which, in the case of a 2x2 matrix, produces the quadratic polynomial, L^2 - trace(M) * L + Det(M)
        Real roots[2];
        Real a = 1;
        Real b = -MatrixBase::trace();
        Real c = MatrixBase::determinant();

        //Find quadratic roots - these will be the eigenvalues (L)
        if (!Math<Real>::QuadraticRealRoots(a, b, c, roots))
        {
            return;
        }

        //Substitute the Eigenvalues back into (M - IL)V = 0 and solve for V to find the Eigenvectors.
        //By expanding the above equation, we find that Y = -a * x / b, where a and b are elements 0 and 1 of M
        //By setting x to 1, we can find the corresponding y value, then normalize the vector.
        for (int i = 0; i < 2; ++i)
        {
            Vector2<Real> v;
            v.X() = 1;
            v.Y() = -((*this)[0] - roots[i]) / (*this)[2];

            v.Normalize();

            rot[0 + i * 2] = v.X();
            rot[1 + i * 2] = v.Y();
        }

        diag = (rot.Inverse() * (*this) * rot);
    }


    /**
     * Assumes matrix is a rotation. Uses gram-schmidt orthonormalisation on the columns of the matrix.
     */
    template<class Real>
    void Matrix2<Real>::orthonormalize()
    {
        Vector2<Real> c1((*this)[0], (*this)[1]);
        Vector2<Real> c2((*this)[2], (*this)[3]);

        c1.Normalize();
        c2 = c2 - c2.VectorProjection(c1, true);
        c2.Normalize();

        (*this)[0] = c1[0];
        (*this)[1] = c1[1];
        (*this)[2] = c2[0];
        (*this)[3] = c2[1];
    }

}

#endif
