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
            std::cout << det << std::endl;
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

    };
}

QTEST_MAIN(GLDemo::TestMatrix3)
#include "test_matrix3.moc"
