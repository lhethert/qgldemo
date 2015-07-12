#include <iostream>

#include <QString>
#include <QObject>
#include <QtTest/QtTest>

#include "Math/mathdefs.h"
#include "Math/matrix3.h"


namespace GLDemo
{

    /**
     * \internal
     */
    class TestMatrix3 : public QObject
    {
        Q_OBJECT

        Matrix3f m_matrixInvertible;

    private slots:
        /**
         * Initiate the test case
         */
        void  initTestCase()
        {
            float data1[9] = {
                2, 2, 2,
                1, 0, 0,
                0, 0, 1
            };
            m_matrixInvertible = Matrix3f(data1, false);
        }

        /**
         *
         */
        void testDefaultConstructor()
        {
            Matrix3f m;
            for (int i = 0; i < 9; ++i)
            {
                QVERIFY(Math<float>::FEqual(m[i], 0.0f));
            }
        }


        /**
         *
         */
        void  testDeterminant()
        {
            float det = m_matrixInvertible.determinant();
            QVERIFY( Math<float>::FEqual(det, -2.0f));
        }


        /**
         * Test matrix multiplication.
         */
        void  testInverse()
        {
            Matrix3f expResult( Matrix3f::createIdentity() );
            Matrix3f result = m_matrixInvertible * m_matrixInvertible.inverse();
            QVERIFY(result == expResult);
        }


        /**
         *
         */
        void  testFromAxisAngle1()
        {
            // 30 degree rotation about the z axis
            const float root3on2 = Math<float>::Sqrt(3) * 0.5;
            float results[9] = {
                root3on2, -0.5, 0,
                0.5, root3on2, 0,
                0, 0, 1
            };

            Matrix3f mResults(results);
            Matrix3f rotation;
            rotation.fromAxisAngle(-Math<float>::PI / 6, Vector3f(0.0f, 0.0f, 1.0f));
            QVERIFY(mResults == rotation);
        }


        /**
         *
         */
        void  testFromAxisAngle2()
        {
            // 30 degree rotation about the z axis
            const float root3on2 = Math<float>::Sqrt(3) * 0.5;
            float results[9] = {
                root3on2, 0, 0.5,
                0, 1, 0,
                -0.5, 0, root3on2
            };

            Matrix3f mResults(results);
            Matrix3f rotation;
            rotation.fromAxisAngle(-Math<float>::PI / 6, Vector3f(0.0f, 1.0f, 0.0f));
            QVERIFY(mResults == rotation);
        }


        /**
         *
         */
        void  testFromAxisAngle3()
        {
            // 30 degree rotation about the z axis
            const float root3on2 = Math<float>::Sqrt(3.0f) * 0.5f;
            float results[9] = {
                1.0f, 0, 0,
                0, root3on2, -0.5f,
                0, 0.5f, root3on2
            };

            Matrix3f mResults(results);
            Matrix3f rotation;
            rotation.fromAxisAngle(-Math<float>::PI / 6, Vector3f(1.0f, 0.0f, 0.0f));
            QVERIFY(mResults == rotation);
        }

    };
}

QTEST_MAIN(GLDemo::TestMatrix3)
#include "test_matrix3.moc"
