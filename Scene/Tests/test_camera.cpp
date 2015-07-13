#include <iostream>

#include <QString>
#include <QObject>
#include <QtTest/QtTest>

#include "Math/mathdefs.h"
#include "Math/matrix3.h"
#include "Math/matrix4.h"
#include "Scene/transformation.h"
#include "Scene/camera.h"


namespace GLDemo
{

    /**
     * \internal
     */
    class TestCamera : public QObject
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
            Camera camera;
        }


        /**
         *
         */
        void testLookAt1()
        {
            Vector3f posIn(0.0f, 0.0f, 10.0f);
            Vector3f upIn(0.0f, 1.0f, 0.0f);
            Vector3f targetIn(0.0f, 0.0f, 0.0f);
            Camera camera;
            camera.setCameraView(posIn, upIn, targetIn);
            camera.updateGeometricState(0.0, true);

            Vector3f expAcross(1.0f, 0.0f, 0.0f);
            Vector3f expUp(0.0f, 1.0f, 0.0f);
            Vector3f expView(0.0f, 0.0f, -1.0f);
            Matrix4f rotation;
            rotation.toIdentity();
            rotation(0,0) = expAcross.x();
            rotation(0,1) = expAcross.y();
            rotation(0,2) = expAcross.z();
            rotation(1,0) = expUp.x();
            rotation(1,1) = expUp.y();
            rotation(1,2) = expUp.z();
            rotation(2,0) = -expView.x();
            rotation(2,1) = -expView.y();
            rotation(2,2) = -expView.z();
            Matrix4f translation;
            translation.toIdentity();
            translation(0,3) = -posIn.x();
            translation(1,3) = -posIn.y();
            translation(2,3) = -posIn.z();
            Matrix4f expViewMatrix = rotation * translation;
            Matrix4f camMatrix;
            camera.toViewMatrix(camMatrix);
            QVERIFY(camMatrix == expViewMatrix);

            Vector3f posOut;
            Vector3f acrossOut;
            Vector3f upOut;
            Vector3f viewOut;
            camera.calcWorldVectors(posOut, viewOut, upOut, acrossOut);
            QVERIFY(posOut == posIn);
            QVERIFY(acrossOut == Vector3f(1.0f, 0.0f, 0.0f));
            QVERIFY(upOut == upIn);
            QVERIFY(viewOut == Vector3f(0.0f, 0.0f, -1.0f));
        }


        /**
         *
         */
        void testLookAt2()
        {
            Matrix3f rotation;
            rotation.fromAxisAngle(Math<float>::PI / 6, Vector3f(0.0f, 1.0f, 0.0f));
            Matrix3f rotationInv = rotation.inverse();
            Vector3f translation(1.0f, 2.0f, 3.0f);

            // Use the inverse rotation matrix here - our rotation matrix
            // indicates what our camera orientation is, but the inverse
            // is what our view matrix needs to be.
            Matrix4f expRotMatrix;
            expRotMatrix.toIdentity();
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    expRotMatrix(i,j) = rotationInv(i,j);
                }
            }
            Matrix4f expTrMatrix;
            expTrMatrix.toIdentity();
            expTrMatrix(0,3) = -translation.x();
            expTrMatrix(1,3) = -translation.y();
            expTrMatrix(2,3) = -translation.z();
            Matrix4f expMatrix = expRotMatrix * expTrMatrix;

            Camera cam;
            cam.getLocalTransformation().setRotation(rotation);
            cam.getLocalTransformation().setTranslation(translation);
            cam.updateGeometricState(0.0, true);
            Matrix4f viewMatrix;
            cam.toViewMatrix(viewMatrix);
            QVERIFY(viewMatrix == expMatrix);
        }
    };
}

QTEST_MAIN(GLDemo::TestCamera)
#include "test_camera.moc"
