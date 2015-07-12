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
         * operator()(row, col).
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
         *
         */
        void  testTranspose()
        {
            float data1T[16] = {
                1, 5, 9, 13,
                2, 6, 10, 14,
                3, 7, 11, 15,
                4, 8, 12, 16
            };
            Matrix4f expResult(data1T);
            Matrix4f result = m_matrix1.transpose();
            QVERIFY(expResult == result);
        }


        /**
         *
         */
        void  testTimesTranspose()
        {
            Matrix4f result = m_matrix1.timesTranspose(m_matrix2);
            QVERIFY( Math<float>::FEqual(result(0,0), (1 + 5 + 9 + 13)) );
            QVERIFY( Math<float>::FEqual(result(1,0), (2 + 10 + 18 + 26)) );
            QVERIFY( Math<float>::FEqual(result(2,0), (3 + 15 + 27 + 39)) );
            QVERIFY( Math<float>::FEqual(result(3,0), (4 + 20 + 36 + 52)) );
            QVERIFY( Math<float>::FEqual(result(0,1), (2 + 6 + 10 + 14)) );
            QVERIFY( Math<float>::FEqual(result(1,1), (4 + 12 + 20 + 28)) );
            QVERIFY( Math<float>::FEqual(result(2,1), (6 + 18 + 30 + 42)) );
            QVERIFY( Math<float>::FEqual(result(3,1), (8 + 24 + 40 + 56)) );
            QVERIFY( Math<float>::FEqual(result(0,2), (3 + 7 + 11 + 15)) );
            QVERIFY( Math<float>::FEqual(result(1,2), (6 + 14 + 22 + 30)) );
            QVERIFY( Math<float>::FEqual(result(2,2), (9 + 21 + 33 + 45)) );
            QVERIFY( Math<float>::FEqual(result(3,2), (12 + 28 + 44 + 60)) );
            QVERIFY( Math<float>::FEqual(result(0,3), (4 + 8 + 12 + 16)) );
            QVERIFY( Math<float>::FEqual(result(1,3), (8 + 16 + 24 + 32)) );
            QVERIFY( Math<float>::FEqual(result(2,3), (12 + 24 + 36 + 48)) );
            QVERIFY( Math<float>::FEqual(result(3,3), (16 + 32 + 48 + 64)) );
        }


        /**
         *
         */
        void  testTransposeTimes()
        {
            Matrix4f result = m_matrix1.transposeTimes(m_matrix2);
            QVERIFY( Math<float>::FEqual(result(0,0), (1 + 2 + 3 + 4)) );
            QVERIFY( Math<float>::FEqual(result(0,1), (2 + 4 + 6 + 8)) );
            QVERIFY( Math<float>::FEqual(result(0,2), (3 + 6 + 9 + 12)) );
            QVERIFY( Math<float>::FEqual(result(0,3), (4 + 8 + 12 + 16)) );
            QVERIFY( Math<float>::FEqual(result(1,0), (5 + 6 + 7 + 8)) );
            QVERIFY( Math<float>::FEqual(result(1,1), (10 + 12 + 14 + 16)) );
            QVERIFY( Math<float>::FEqual(result(1,2), (15 + 18 + 21 + 24)) );
            QVERIFY( Math<float>::FEqual(result(1,3), (20 + 24 + 28 + 32)) );
            QVERIFY( Math<float>::FEqual(result(2,0), (9 + 10 + 11 + 12)) );
            QVERIFY( Math<float>::FEqual(result(2,1), (18 + 20 + 22 + 24)) );
            QVERIFY( Math<float>::FEqual(result(2,2), (27 + 30 + 33 + 36)) );
            QVERIFY( Math<float>::FEqual(result(2,3), (36 + 40 + 44 + 48)) );
            QVERIFY( Math<float>::FEqual(result(3,0), (13 + 14 + 15 + 16)) );
            QVERIFY( Math<float>::FEqual(result(3,1), (26 + 28 + 30 + 32)) );
            QVERIFY( Math<float>::FEqual(result(3,2), (39 + 42 + 45 + 48)) );
            QVERIFY( Math<float>::FEqual(result(3,3), (52 + 56 + 60 + 64)) );
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


        /**
         *
         */
        void  testMultVector()
        {
            Vector4f expResult(1, 2, 3, 4);
            Vector4f v(1,0,0,0);
            v = m_matrix1 * v;
            QVERIFY(expResult == v);
        }


        /**
         *
         */
        void  testSubtraction()
        {
            float data[16] = {
                0, 1, 2, 3,
                3, 4, 5, 6,
                6, 7, 8, 9,
                9, 10, 11, 12
            };
            Matrix4f expResult(data);
            QVERIFY(expResult == (m_matrix1 - m_matrix2));
        }


        /**
         *
         */
        void  testAddition()
        {
            float data[16] = {
                2, 3, 4, 5,
                7, 8, 9, 10,
                12, 13, 14, 15,
                17, 18, 19, 20
            };
            Matrix4f expResult(data);
            QVERIFY(expResult == (m_matrix1 + m_matrix2));
        }

    };
}

QTEST_MAIN(GLDemo::TestMatrix4)
#include "test_matrix4.moc"
