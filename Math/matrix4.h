#ifndef GLDEMO_MATRIX_4_H
#define GLDEMO_MATRIX_4_H

#include "matrixn.h"
#include "matrix3.h"
#include "vector4.h"
#include "vector3.h"
#include "mathdefs.h"

namespace GLDemo
{

    /**
     * Defines a square matrix in 4 dimensions (4x4).
     * Supports the generation of a matrix from Axis/Angle
     */
    template<class Real>
    class Matrix4 : public MatrixN<Matrix4<Real>, Real, 4>
    {
        typedef MatrixN<Matrix4<Real>, Real, 4> MatrixBase;

    public:
        Matrix4();
        Matrix4(const Real m[], bool transposeM = false);
        Matrix4(const Matrix4&);

        //Algebraic operations specific to 4D
        using MatrixN<Matrix4<Real>, Real, 4>::operator*;
        Vector4<Real> operator*(const Vector4<Real>& v) const;

        //Geometric operations specific to 4D
        Real determinantImpl() const;
        Matrix4 adjointImpl() const;

        Matrix4& fromAxisAngle(const Real& angle, const Vector3<Real>& axis);
        void toAxisAngle(Real& angle, Vector3<Real>& axis);

        void makePerspectiveProjectionFOV(const Real& fovY, const Real& aspectRatio, const Real& near, const Real& far);
        void makeOrthographicProjection(const Real& left, const Real& right, const Real& top, const Real& bottom, const Real& near, const Real& far);
        void makeReflection(const Vector3<Real>& axis);

        void polarDecomposition(Matrix3<Real>& scale, Matrix3<Real>& rotation, Vector3<Real>& translation) const;

    private:
        Real cofactor(int i, int j) const;
    };

    typedef Matrix4<float> Matrix4f;
    typedef Matrix4<double> Matrix4d;


    /**
     * Initialises a new 4x4 matrix as the identity matrix.
     */
    template<class Real>
    Matrix4<Real>::Matrix4() :
        MatrixBase()
    {
    }


    /**
     * Create a new 4x4 matrix from the input array. If transposeM is set, create the new matrix as
     * the transpose of the original matrix.
     */
    template<class Real>
    Matrix4<Real>::Matrix4(const Real m[], bool transposeM) :
        MatrixBase(m, transposeM)
    {
    }


    /**
     *
     */
    template<class Real>
    Matrix4<Real>::Matrix4(const Matrix4<Real>& m) :
        MatrixBase(m)
    {
    }


    /**
     * Pre-multiplies the specified vector with this matrix. The matrix is
     * assumed to be a column-vector, not a row-vector.
     *
     * \param v The vector to pre-multiply with this matrix.
     */
    template<class Real>
    Vector4<Real> Matrix4<Real>::operator* (const Vector4<Real>& v) const
    {
        Vector4<Real> result;
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                result[row] += (*this)(row, col) * v[col];
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
    inline Real Matrix4<Real>::determinantImpl() const
    {
        return cofactor(0,0) * (*this)[0] +
               cofactor(0,1) * (*this)[4] +
               cofactor(0,2) * (*this)[8] +
               cofactor(0,3) * (*this)[12];
    }


    /**
     * The adjoint matrix (or 'classical adjoint' / 'adjugate' matrix) is the transpose of the matrix of
     * cofactors.
     */
    template<class Real>
    Matrix4<Real> Matrix4<Real>::adjointImpl() const
    {
        Matrix4<Real> result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result(i, j) = cofactor(i, j);
            }
        }

        return result.transpose();
    }


    /**
     * Loads the matrix as a rotation matrix to rotate about the specified axis by the specified angle.
     *
     * \param angle The angle to rotate about the axis
     * \param axis  The axis of rotation
     */
    template<class Real>
    Matrix4<Real>& Matrix4<Real>::fromAxisAngle(const Real& angle, const Vector3<Real>& axis)
    {
        Real arr[16] = {
            0.0, axis.Z(), -axis.Y(), 0.0,
            -axis.Z(), 0.0, axis.X(), 0.0,
            axis.Y(), -axis.X(), 0.0 , 0.0,
            0.0, 0.0, 0.0, 0.0
        };
        Matrix4<Real> A(arr, false);

        Real c = 1 - Math<Real>::Cos(angle);
        Real s = Math<Real>::Sin(angle);

        MatrixBase::toIdentity();
        (*this) += A * s + (A * A) * c;

        return *this;
    }


    /**
     * Converts this matrix into axis-angle notation, assuming it has a rotation component.
     * (not implemented).
     */
    template<class Real>
    void Matrix4<Real>::toAxisAngle(Real& angle, Vector3<Real>& v)
    {
        // Not implemented.
        assert(false);
    }



    /**
     * Computes the cofactor of element M(i,j) by evaluating the Minor formed by eliminating row i and column
     * j from the matrix.
     */
    template<class Real>
    Real Matrix4<Real>::cofactor(int i, int j) const
    {
        Matrix3<Real> minor;
        int countX = 0;

        for (int x = 0; x < 4; ++x)
        {
            int countY = 0;
            if (x != i)
            {
                for (int y = 0; y < 4; ++y)
                {
                    if (y != j)
                    {
                        minor(countX, countY++) = (*this)(x,y);
                    }
                }
                countX++;
            }
        }

        return minor.determinant() * Math<Real>::Pow((Real)-1.0, i + j + (Real)2.0);
    }


    /**
     * \param fovY         Field of view in the Y dimension, in degrees
     * \param aspectRatio  The required aspect ratio of the projection
     * \param zNear        Distance from the eye to the near clip plane
     * \param zFar         Distance from the eye to the far clip plane.
     *
     * Makes a perspective projection matrix with the desired field-of-view, aspect ratio, near and far values.
     * Matrix is formatted for a left-handed coordinate system.
     */
    template<class Real>
    void Matrix4<Real>::makePerspectiveProjectionFOV(const Real& fovY, const Real& aspectRatio, const Real& zNear, const Real& zFar)
    {
        Real f = 1.0f / Math<Real>::Tan((fovY * M_PI / 180.0) * Real(0.5f));
        Real nearMinusFar = (zNear - zFar);

        MatrixBase::toZero();
        Matrix4<Real>& m = *this;
        m(0,0) = f / aspectRatio;
        m(1,1) = f;
        m(2,2) = (zFar + zNear) / nearMinusFar;
        m(2,3) = (2.0f * zFar * zNear) / nearMinusFar;
        m(3,2) = -1.0f;
    }


    /**
     * Create an orthographic projection matrix (not yet implemented).
     */
    template<class Real>
    void Matrix4<Real>::makeOrthographicProjection(const Real& left, const Real& right, const Real& top, const Real& bottom, const Real& near, const Real& far)
    {
        assert(false);
    }


    /**
     * Reflect the matrix about an axis (not implemented).
     */
    template<class Real>
    void Matrix4<Real>::makeReflection(const Vector3<Real>& axis)
    {
        assert(false);
    }


    /**
     * \param scale       Matrix in which to store the computed scale
     * \param rotation    Matrix in which to store the computed rotation
     * \param translation Matrix in which to store the computed translation
     *
     * Uses polar decomposition to deduce the various components of this matrix, assuming
     * that it is a 4x4 affine transformation. This function is expensive, so it is
     * recommended to store and maintain the original components wherever possible.
     */
    template<class Real>
    void Matrix4<Real>::polarDecomposition(Matrix3<Real>& scale, Matrix3<Real>& rotation, Vector3<Real>& translation) const
    {
        translation[0] = (*this)(0,3);
        translation[1] = (*this)(1,3);
        translation[2] = (*this)(2,3);

        Matrix3<Real> zeroM;
        Matrix3<Real> input3;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                input3(i,j) = (*this)(i,j);
            }
        }

        rotation.toZero();
        Matrix3<Real> temp = input3;
        int count = 0;

        while (!Math<Real>::FEqual((temp - rotation).frobeniusNormSquared(), (Real)0.0))
        {
            rotation = temp;
            temp = (Real)0.5 * (rotation + rotation.transpose().inverse());
            ++count;
        }

        rotation = temp;
        //TODO: fix this comparison function to perform correct floating point comparison
        if (rotation.determinant() < (Real)0.0)
        {
            temp(0,0) = (Real)-1.0;
            temp(1,1) = (Real)-1.0;
            temp(2,2) = (Real)-1.0;

            rotation = rotation * temp;
        }

        rotation.orthonormalize();
        scale = rotation.inverse() * input3;
    }

}


#endif
