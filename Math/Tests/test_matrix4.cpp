#include <iostream>

#include <QString>
#include <QObject>
#include <QtTest/QtTest>

#include "Math/mathdefs.h"
#include "Math/matrix4.h"
#include "Math/matrix3.h"
#include "Math/matrix2.h"


namespace GLDemo
{

    /**
     * \internal
     */
    class TestMatrix4 : public QObject
    {
        Q_OBJECT

        Matrix4f m_matrix1;
        Matrix4f m_matrix2;
        Matrix4f m_matrixInvertible;

    private slots:
        /**
         * Initiate the test case
         */
        void  initTestCase()
        {
            float data1[16] = {
                1, 2, 3, 4,
                5, 6, 7, 8,
                9, 10, 11, 12,
                13, 14, 15, 16
            };
            float data2[16] = {
                1, 1, 1, 1,
                2, 2, 2, 2,
                3, 3, 3, 3,
                4, 4, 4, 4
            };
            float data3[16] = {
                2, 3, 1, 5,
                1, 0, 3, 1,
                0, 2, -3, 2,
                0, 2, 3, 1
            };
            m_matrix1 = Matrix4f(data1, false);
            m_matrix2 = Matrix4f(data2, false);
            m_matrixInvertible = Matrix4f(data3, false);
        }

        /**
         *
         */
        void testDefaultConstructor()
        {
            Matrix4f m;
            for (int i = 0; i < 16; ++i)
            {
                QVERIFY(Math<float>::FEqual(m[i], 0.0f));
            }
        }


        /**
         *
         */
        void testIdentity()
        {
            Matrix4f m(Matrix4f::createIdentity());
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    if (i == j)
                    {
                        QVERIFY(Math<float>::FEqual(m(i,j), 1.0f));
                    }
                    else
                    {
                        QVERIFY(Math<float>::FEqual(m(i,j), 0.0f));
                    }
                }
            }
        }


        /**
         * Test the indexing operators, knowing that the data is in column-major order.
         */
        void testIndexOperators()
        {
            QVERIFY( Math<float>::FEqual(m_matrix1(0,0), 1.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(1,0), 2.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(2,0), 3.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(3,0), 4.0f) );

            QVERIFY( Math<float>::FEqual(m_matrix1(0,1), 5.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(1,1), 6.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(2,1), 7.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(3,1), 8.0f) );

            QVERIFY( Math<float>::FEqual(m_matrix1(0,2), 9.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(1,2), 10.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(2,2), 11.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(3,2), 12.0f) );

            QVERIFY( Math<float>::FEqual(m_matrix1(0,3), 13.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(1,3), 14.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(2,3), 15.0f) );
            QVERIFY( Math<float>::FEqual(m_matrix1(3,3), 16.0f) );
        }


        /**
         * Test matrix multiplication.
         */
        void  testMultiply()
        {
            Matrix4f result = m_matrix1 * m_matrix2;
            QVERIFY( Math<float>::FEqual(result(0,0), (1 + 5 + 9 + 13)) );
            QVERIFY( Math<float>::FEqual(result(0,1), (2 + 10 + 18 + 26)) );
            QVERIFY( Math<float>::FEqual(result(0,2), (3 + 15 + 27 + 39)) );
            QVERIFY( Math<float>::FEqual(result(0,3), (4 + 20 + 36 + 52)) );
            QVERIFY( Math<float>::FEqual(result(1,0), (2 + 6 + 10 + 14)) );
            QVERIFY( Math<float>::FEqual(result(1,1), (4 + 12 + 20 + 28)) );
            QVERIFY( Math<float>::FEqual(result(1,2), (6 + 18 + 30 + 42)) );
            QVERIFY( Math<float>::FEqual(result(1,3), (8 + 24 + 40 + 56)) );
            QVERIFY( Math<float>::FEqual(result(2,0), (3 + 7 + 11 + 15)) );
            QVERIFY( Math<float>::FEqual(result(2,1), (6 + 14 + 22 + 30)) );
            QVERIFY( Math<float>::FEqual(result(2,2), (9 + 21 + 33 + 45)) );
            QVERIFY( Math<float>::FEqual(result(2,3), (12 + 28 + 44 + 60)) );
            QVERIFY( Math<float>::FEqual(result(3,0), (4 + 8 + 12 + 16)) );
            QVERIFY( Math<float>::FEqual(result(3,1), (8 + 16 + 24 + 32)) );
            QVERIFY( Math<float>::FEqual(result(3,2), (12 + 24 + 36 + 48)) );
            QVERIFY( Math<float>::FEqual(result(3,3), (16 + 32 + 48 + 64)) );
        }


        /**
         * Test the comparison operator.
         */
        void  testComparison()
        {
            QVERIFY(m_matrix1 == m_matrix1);
            QVERIFY(m_matrix2 != m_matrix1);
        }


        /**
         *
         */
        void  testDeterminant()
        {
            float det = m_matrixInvertible.determinant();
            std::cout << det << std::endl;
            QVERIFY( Math<float>::FEqual(det, 1.0f));
        }


        /**
         * Test matrix multiplication.
         */
        void  testInverse()
        {
            Matrix4f expResult( Matrix4f::createIdentity() );
            Matrix4f result = m_matrixInvertible * m_matrixInvertible.inverse();
            QVERIFY(result == expResult);
        }

    };
}

QTEST_MAIN(GLDemo::TestMatrix4)
#include "test_matrix4.moc"
