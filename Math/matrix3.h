#ifndef GLDEMO_MATRIX_3_H
#define GLDEMO_MATRIX_3_H

#include "matrixn.h"
#include "vector3.h"
#include "mathdefs.h"

namespace GLDemo
{

    /**
     * Defines a square matrix in 3 dimensions (3x3).
     * Supports the generation of a matrix from Axis/Angle
     */
    template<class Real>
    class Matrix3 : public MatrixN<Matrix3<Real>, Real, 3>
    {
        typedef MatrixN<Matrix3<Real>, Real, 3> MatrixBase;

    public:
        Matrix3();
        Matrix3(const Real m[], bool transposeM = false);
        Matrix3(const Matrix3&);

        //Algebraic operations specific to 3D
        using MatrixN<Matrix3<Real>, Real, 3>::operator*;
        Vector3<Real> operator*(const Vector3<Real>& v) const;

        //Geometric operations specific to 3D
        Real determinantImpl() const;
        Matrix3 adjointImpl() const;

        bool diagonalization (Matrix3& rot, Matrix3& diag) const;
        void orthonormalize ();

        Matrix3& fromAxisAngle(const Real& angle, const Vector3<Real>& axis);
        void toAxisAngle(Real& angle, Vector3<Real>& axis);

    private:
        Real cofactor(int i, int j) const;
    };

    typedef Matrix3<float> Matrix3f;
    typedef Matrix3<double> Matrix3d;


    /**
     * Initialises a new 3x3 matrix as the identity matrix.
     */
    template<class Real>
    Matrix3<Real>::Matrix3() :
        MatrixBase()
    {
    }


    /**
     * Create a new 3x3 matrix from the input array. If transposeM is set, create the new matrix as
     * the transpose of the original matrix.
     */
    template<class Real>
    Matrix3<Real>::Matrix3(const Real m[], bool transposeM) :
        MatrixBase(m, transposeM)
    {
    }


    /**
     *
     */
    template<class Real>
    Matrix3<Real>::Matrix3(const Matrix3<Real>& m) :
        MatrixBase(m)
    {
    }


    /**
     *
     */
    template<class Real>
    Vector3<Real> Matrix3<Real>::operator* (const Vector3<Real>& v) const
    {
        Vector3<Real> result;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                result[i] += (*this)[i + j * 3] * v[j];
            }
        }

        return result;
    }


    /**
     * Called by Determinant function of base-class (CRTP). Uses expansion by minors to calculate
     * the determinant. Could implement this (possibly faster) by arranging the matrix in upper triangular form, then
     * calculating the trace.
     */
    template<class Real>
    inline Real Matrix3<Real>::determinantImpl() const
    {
        return cofactor(0,0) * (*this)[0] + cofactor(0,1) * (*this)[3] + cofactor(0,2) * (*this)[6];
    }


    /**
     * The adjoint matrix (or 'classical adjoint' / 'adjugate' matrix) is the transpose of the matrix of
     * cofactors.
     */
    template<class Real>
    Matrix3<Real> Matrix3<Real>::adjointImpl() const
    {
        Matrix3<Real> result;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                result(i, j) = cofactor(i, j);
            }
        }

        return result.transpose();
    }


    /**
     * Performs Eigen-Decomposition on the matrix in order to Diagonalize it. The results are a
     * matrix 'rot', each of the columns of which are a normalized Eigenvector of the matrix, and a matrix
     * 'diag', which has the corresponding eigenvalues along the main diagonal.
     *
     * The equation A^-1 * M * A, where A is the matrix 'rot', will produce matrix Diag.
     * \note Assumes the matrix is Symmetric.
     */
    template<class Real>
    bool Matrix3<Real>::diagonalization(Matrix3<Real>& rot, Matrix3<Real>& diag) const
    {
        //Eigenvalues are the roots of the equation obtained by expanding Det(M - IL),
        //which, in the case of a 2x2 matrix, produces the quadratic polynomial, L^2 - Trace(M) * L + Det(M)
        Real roots[3];
        Real a = 1;
        Real b = -MatrixBase::trace();
        Real c = (*this)(0,0) * (*this)(1,1) + (*this)(0,0) * (*this)(2,2) + (*this)(2,2) * (*this)(1,1) -
                 (*this)(0,1) * (*this)(1,0) - (*this)(1,2) * (*this)(2,1) - (*this)(0,2) * (*this)(2,0);
        Real d = -MatrixBase::determinant();

        //Find cubic real roots - these will be the eigenvalues (L)
        if (!Math<Real>::cubicRealRoots(a, b, c, d, roots))
        {
            return false;
        }

        //Substitute the Eigenvalues back into (M - IL)V = 0 and solve for V to find the Eigenvectors.
        //By expanding the above equation, we find that Y = -a * x / b, where a and b are elements 0 and 1 of M
        //By setting x to 1, we can find the corresponding y value, then normalize the vector.
        for (int r = 0; r < 3; ++r)
        {
            Matrix3<Real> temp(*this);
            temp[0] -= roots[r];
            temp[4] -= roots[r];
            temp[8] -= roots[r];

            //Cannot apply cramer's rule here, as the determinant is likely to equal zero, due to the symmetry. Instead, we must apply
            //elementary row operations to find the values

            //arrange rows into reduced row eschelon form, and eliminate variables one by one
            temp.toReducedRowEschelonForm();

            //test for zero rows. If there are 2 or more zero rows, then there are too many free variables to deduce an eigenvector
            Vector3<Real> v;
            if (Math<Real>::FEqual(temp[8], 0.0))
            {
                bool zeroZ = Math<Real>::FEqual(temp[7], 0.0);
                bool zeroY = Math<Real>::FEqual(temp[4], 0.0);

                if (zeroZ && zeroY)
                {
                    //two free variables. cannot solve
                    return false;
                }
                else if (zeroZ)
                {
                    v.Y() = (Real)0.0;
                    v.Z() = (Real)1.0;
                }
                else if (zeroY)
                {
                    v.Y() = (Real)1.0;
                    v.Z() = (Real)0.0;
                }
            }
            else
            {
                v.Z() = (Real)0.0;
            }

            v.X() = (-temp[3] * v.Y() - temp[6] * v.Z()) / temp[0];

            v.Normalize();

            rot[0 + r * 3] = v.X();
            rot[1 + r * 3] = v.Y();
            rot[2 + r * 3] = v.Z();
        }

        diag = (rot.Inverse() * (*this) * rot);

        return true;
    }


    /**
     * Assumes matrix is a rotation. Uses gram-schmidt orthonormalisation on the columns of the matrix.
     */
    template<class Real>
    void Matrix3<Real>::orthonormalize()
    {
        Vector3<Real> c1((*this)[0], (*this)[1], (*this)[2]);
        Vector3<Real> c2((*this)[3], (*this)[4], (*this)[5]);
        Vector3<Real> c3((*this)[6], (*this)[7], (*this)[8]);

        c1.normalize();
        c2 = c2 - c2.vectorProjection(c1, true);
        c2.normalize();
        c3 = c3 - c3.vectorProjection(c2, true) - c3.vectorProjection(c1, true);
        c3.normalize();

        (*this)[0] = c1[0];
        (*this)[1] = c1[1];
        (*this)[2] = c1[2];
        (*this)[3] = c2[0];
        (*this)[4] = c2[1];
        (*this)[5] = c2[2];
        (*this)[6] = c3[0];
        (*this)[7] = c3[1];
        (*this)[8] = c3[2];
    }


    /**
     * Loads the matrix as a rotation matrix to rotate about the specified axis by the specified angle.
     */
    template<class Real>
    Matrix3<Real>& Matrix3<Real>::fromAxisAngle(const Real& angle, const Vector3<Real>& axis)
    {
        Real arr[9] = {
            0.0, axis.z(), -axis.y(),
            -axis.z(), 0.0, axis.x(),
            axis.y(), -axis.x(), 0.0
        };
        Matrix3<Real> A(arr, false);

        Real c = 1 - Math<Real>::Cos(angle);
        Real s = Math<Real>::Sin(angle);

        MatrixBase::toIdentity();
        (*this) += A * s + (A * A) * c;

        return *this;
    }


    /**
     * Not implemented.
     */
    template<class Real>
    void Matrix3<Real>::toAxisAngle(Real& angle, Vector3<Real>& v)
    {
        assert(false);
    }


    /**
     * Computes the cofactor of element M(i,j) by evaluating the Minor formed by eliminating row i and column
     * j from the matrix.
     */
    template<class Real>
    Real Matrix3<Real>::cofactor(int i, int j) const
    {
        Real values[4];
        int count = 0;

        for (int x = 0; x < 3; ++x)
        {
            if (x != i)
            {
                for (int y = 0; y < 3; ++y)
                {
                    if (y != j)
                    {
                        values[count++] = (*this)(x,y);
                    }
                }
            }
        }

        return (values[0] * values[3] - values[1] * values[2]) * Math<Real>::Pow((Real)-1.0, i + j + (Real)2.0);
    }
}

#endif

