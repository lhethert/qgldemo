#include <iostream>

#include <QString>
#include <QObject>
#include <QtTest/QtTest>

#include "Math/mathdefs.h"
#include "Math/matrix3.h"
#include "Math/matrix4.h"
#include "Scene/transformation.h"


namespace GLDemo
{

    /**
     * \internal
     */
    class TestTransform : public QObject
    {
        Q_OBJECT

    private slots:
        /**
         * Initiate the test case
         */
        void  initTestCase()
        {
        }

        /**
         *
         */
        void testDefaultConstructor()
        {
            Transformation t;
            Matrix3f identity;
            identity.toIdentity();
            QVERIFY(t.getRotation() == identity);
            QVERIFY(t.getTranslation() == Vector3f());
            QVERIFY(t.getScale() == Vector3f(1.0f,1.0f,1.0f));
            QVERIFY(Math<float>::FEqual(t.getUniformScale(), 1.0f));
        }


        /**
         *
         */
        void testRotation()
        {
            Transformation t;
            Matrix3f rotation;
            rotation.fromAxisAngle(Math<float>::PI / 6, Vector3f(0.0, 0.0, 1.0f));
            t.setRotation(rotation);
            QVERIFY(t.getRotation() == rotation);
        }


        /**
         *
         */
        void testUniformScale()
        {
            Transformation t;
            t.setUniformScale(2.0f);
            QVERIFY(t.getUniformScale() == 2.0f);
            QVERIFY(t.getScale() == Vector3f(2.0f, 2.0f, 2.0f));
        }


        /**
         *
         */
        void testNonUniformScale()
        {
            Transformation t;
            t.setScale(Vector3f(1.0f, 2.0f, 3.0f));
            QVERIFY(t.getScale() == Vector3f(1.0f, 2.0f, 3.0f));
        }


        /**
         *
         */
        void testTranslation()
        {
            Transformation t;
            t.setTranslation(Vector3f(1.0f, 2.0f, 3.0f));
            QVERIFY(t.getTranslation() == Vector3f(1.0f, 2.0f, 3.0f));
        }


        /**
         *
         */
        void testApply1()
        {
            // Create a transform and apply it to a vector
            Transformation t;
            Matrix3f rotation;
            rotation.fromAxisAngle(Math<float>::PI / 2, Vector3f(0.0, 0.0, 1.0f));
            t.setRotation(rotation);
            t.setTranslation(Vector3f(5.0f, 0.0f, 0.0f));
            Vector3f pos(0.0f, 0.0f, 0.0f);
            Vector3f result = t.apply(pos);

            // Confirm that the results are the same as those from a 4x4 matrix
            Matrix4f transformMatrix;
            transformMatrix.toIdentity();
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    transformMatrix(i,j) = rotation(i,j);
                }
            }
            transformMatrix(0,3) = 5.0f;
            Vector4f matrixResult = transformMatrix * Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
            QVERIFY(result == matrixResult.xyz());
        }


        /**
         *
         */
        void testApply2()
        {
            // Create a transform and apply it to a vector
            Transformation t;
            Matrix3f rotation;
            rotation.fromAxisAngle(Math<float>::PI / 2, Vector3f(0.0, 0.0, 1.0f));
            t.setRotation(rotation);
            Vector3f pos(5.0f, 0.0f, 0.0f);
            Vector3f result = t.apply(pos);

            // Confirm that the results are the same as those from a 4x4 matrix
            Matrix4f rotationMatrix;
            rotationMatrix.toIdentity();
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    rotationMatrix(i,j) = rotation(i,j);
                }
            }
            Vector4f matrixResult = rotationMatrix * Vector4f(5.0f, 0.0f, 0.0f, 1.0f);
            QVERIFY(result == matrixResult.xyz());
            QVERIFY(result == Vector3f(0.0, 5.0f, 0.0f));
        }


        /**
         *
         */
        void testApply3()
        {
            // Create a transform and apply it to a vector
            Transformation t;
            Matrix3f rotation;
            rotation.fromAxisAngle(Math<float>::PI / 2, Vector3f(0.0, 0.0, 1.0f));
            t.setRotation(rotation);
            t.setUniformScale(2.0f);
            Vector3f pos(5.0f, 0.0f, 0.0f);
            Vector3f result = t.apply(pos);

            // Confirm that the results are the same as those from a 4x4 matrix
            Matrix4f rotationMatrix;
            rotationMatrix.toIdentity();
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    rotationMatrix(i,j) = rotation(i,j);
                }
            }
            Matrix4f scaleMatrix;
            scaleMatrix.toIdentity();
            scaleMatrix(0,0) = 2.0f;
            scaleMatrix(1,1) = 2.0f;
            scaleMatrix(2,2) = 2.0f;
            Vector4f matrixResult = rotationMatrix * scaleMatrix * Vector4f(5.0f, 0.0f, 0.0f, 1.0f);
            QVERIFY(result == matrixResult.xyz());
            QVERIFY(result == Vector3f(0.0, 10.0f, 0.0f));
        }


        /**
         *
         */
        void testApply4()
        {
            // Create a transform and apply it to a vector
            Transformation t;
            Matrix3f rotation;
            rotation.fromAxisAngle(Math<float>::PI / 2, Vector3f(0.0, 0.0, 1.0f));
            t.setRotation(rotation);
            t.setUniformScale(2.0f);
            t.setTranslation(Vector3f(1.0f,1.0f,1.0f));
            Vector3f pos(5.0f, 0.0f, 0.0f);
            Vector3f result = t.apply(pos);

            // Confirm that the results are the same as those from a 4x4 matrix
            Matrix4f rotationMatrix;
            rotationMatrix.toIdentity();
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    rotationMatrix(i,j) = rotation(i,j);
                }
            }
            Matrix4f scaleMatrix;
            scaleMatrix.toIdentity();
            scaleMatrix(0,0) = 2.0f;
            scaleMatrix(1,1) = 2.0f;
            scaleMatrix(2,2) = 2.0f;
            Matrix4f translationMatrix;
            translationMatrix.toIdentity();
            translationMatrix(0,3) = 1.0f;
            translationMatrix(1,3) = 1.0f;
            translationMatrix(2,3) = 1.0f;
            Vector4f matrixResult = translationMatrix * rotationMatrix * scaleMatrix * Vector4f(5.0f, 0.0f, 0.0f, 1.0f);
            QVERIFY(result == matrixResult.xyz());
            QVERIFY(result == Vector3f(1.0, 11.0f, 1.0f));
        }


        /**
         *
         */
        void testApplyInverse()
        {
            // Create a transform and apply it to a vector
            Transformation t;
            Matrix3f rotation;
            rotation.fromAxisAngle(Math<float>::PI / 2, Vector3f(0.0, 0.0, 1.0f));
            t.setRotation(rotation);
            Vector3f pos(5.0f, 0.0f, 0.0f);
            Vector3f result = t.applyInverse(pos);

            // Confirm that the results are the same as those from a 4x4 matrix
            Matrix4f rotationMatrix;
            rotationMatrix.toIdentity();
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    rotationMatrix(i,j) = rotation(i,j);
                }
            }
            rotationMatrix = rotationMatrix.inverse();
            Vector4f matrixResult = rotationMatrix * Vector4f(5.0f, 0.0f, 0.0f, 1.0f);
            QVERIFY(result == matrixResult.xyz());
            QVERIFY(result == Vector3f(0.0, -5.0f, 0.0f));
        }


        /**
         *
         */
        void testToMatrix()
        {
            // Create a transform and apply it to a vector
            Transformation t;
            Matrix3f rotation;
            rotation.fromAxisAngle(Math<float>::PI / 6, Vector3f(0.0, 0.0, 1.0f));
            t.setRotation(rotation);
            t.setUniformScale(2.0f);
            t.setTranslation(Vector3f(1.0f, 2.0f, 3.0f));

            // Confirm that the results are the same as those from a 4x4 matrix
            Matrix4f rotationMatrix;
            rotationMatrix.toIdentity();
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    rotationMatrix(i,j) = rotation(i,j);
                }
            }
            rotationMatrix = rotationMatrix;
            Matrix4f scaleMatrix;
            scaleMatrix.toIdentity();
            scaleMatrix(0,0) = 2.0f;
            scaleMatrix(1,1) = 2.0f;
            scaleMatrix(2,2) = 2.0f;
            Matrix4f translationMatrix;
            translationMatrix.toIdentity();
            translationMatrix(0,3) = 1.0f;
            translationMatrix(1,3) = 2.0f;
            translationMatrix(2,3) = 3.0f;

            Matrix4f result;
            t.toMatrix(result);
            QVERIFY(result == (translationMatrix * rotationMatrix * scaleMatrix));
        }


        /**
         *
         */
        void testFromMatrix()
        {
            // Confirm that the results are the same as those from a 4x4 matrix
            Matrix3f rotationMatrix3x3;
            Matrix4f rotationMatrix4x4;
            rotationMatrix4x4.toIdentity();
            rotationMatrix3x3.fromAxisAngle(Math<float>::PI / 6, Vector3f(1.0f, 0.0f, 0.0f));
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    rotationMatrix4x4(i,j) = rotationMatrix3x3(i,j);
                }
            }

            Matrix4f scaleMatrix;
            scaleMatrix.toIdentity();
            scaleMatrix(0,0) = 2.0f;
            scaleMatrix(1,1) = 2.0f;
            scaleMatrix(2,2) = 2.0f;
            Matrix4f translationMatrix;
            translationMatrix.toIdentity();
            translationMatrix(0,3) = 1.0f;
            translationMatrix(1,3) = 2.0f;
            translationMatrix(2,3) = 3.0f;

            Matrix4f inputMatrix(translationMatrix * rotationMatrix4x4 * scaleMatrix);

            // Create a transform and apply it to a vector
            Transformation t;
            t.fromMatrix(inputMatrix);
            QVERIFY(t.getRotation() == rotationMatrix3x3);
            QVERIFY(t.getScale() == Vector3f(2.0f, 2.0f, 2.0f));
            QVERIFY(t.getTranslation() == Vector3f(1.0f, 2.0f, 3.0f));
        }


        /**
         *
         */
        void testCombine()
        {
            // Create our two transforms and combine them
            Transformation t1, t2;
            Matrix3f rotation;
            rotation.fromAxisAngle(Math<float>::PI / 6, Vector3f(1.0, 1.0, 1.0f).unitVector());
            t1.setRotation(rotation);
            t1.setUniformScale(2.0f);
            t1.setTranslation(Vector3f(5.0f,3.0f,1.0f));
            t2.setRotation(rotation);
            t2.setUniformScale(5.0f);
            t2.setTranslation(Vector3f(0.0f,5.0f,0.0f));
            Transformation result;
            result.combine(t1, t2);

            // Create two 4x4 matrices, so we can confirm our results
            Matrix4f r1, r2;
            r1.toIdentity();
            r2.toIdentity();
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    r1(i,j) = r2(i,j) = rotation(i,j);
                }
            }
            Matrix4f s1, s2;
            s1.toIdentity();
            s1(0,0) = 2.0f;
            s1(1,1) = 2.0f;
            s1(2,2) = 2.0f;
            s2.toIdentity();
            s2(0,0) = 5.0f;
            s2(1,1) = 5.0f;
            s2(2,2) = 5.0f;
            Matrix4f tr1, tr2;
            tr1.toIdentity();
            tr1(0,3) = 5.0f;
            tr1(1,3) = 3.0f;
            tr1(2,3) = 1.0f;
            tr2.toIdentity();
            tr2(1,3) = 5.0f;
            Matrix4f expM1 = tr1 * r1 * s1;
            Matrix4f expM2 = tr2 * r2 * s2;
            Matrix4f expResult = expM1 * expM2;
            Matrix4f mResult;
            result.toMatrix(mResult);
            QVERIFY(mResult == expResult);
        }

    };
}

QTEST_MAIN(GLDemo::TestTransform)
#include "test_transform.moc"
