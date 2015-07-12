#include <iostream>

#include <QString>
#include <QObject>
#include <QtTest/QtTest>

#include "Math/mathdefs.h"
#include "Math/matrix2.h"


namespace GLDemo
{

    /**
     * \internal
     */
    class TestMatrix2 : public QObject
    {
        Q_OBJECT

        Matrix2f m_matrixInvertible;

    private slots:
        /**
         * Initiate the test case
         */
        void  initTestCase()
        {
            float data1[4] = {
                1, 2,
                3, 4
            };
            m_matrixInvertible = Matrix2f(data1, false);
        }

        /**
         *
         */
        void testDefaultConstructor()
        {
            Matrix2f m;
            for (int i = 0; i < 4; ++i)
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
            Matrix2f expResult( Matrix2f::createIdentity() );
            Matrix2f result = m_matrixInvertible * m_matrixInvertible.inverse();
            QVERIFY(result == expResult);
        }

    };
}

QTEST_MAIN(GLDemo::TestMatrix2)
#include "test_matrix2.moc"
