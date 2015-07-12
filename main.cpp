#include <iostream>

#include <QApplication>
#include <QTimer>
#include <QObject>

#include "Scene/scene.h"
#include "Scene/camera.h"
#include "Scene/cubemesh.h"
#include "Scene/meshinstance.h"
#include "Renderer/glwidget.h"
#include "Renderer/lambertshader.h"

using namespace GLDemo;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Hard code the creation of our scene for now. Can move this to a file format eventually.
    Scene scene;
    Camera* camera = new Camera();
    //camera->setCameraView(Vector3f(0, 0, 10), Vector3f(0, 1, 0), Vector3f(0, 0, 0));
    camera->setCameraView(Vector3f(10, 0, 0), Vector3f(0, 1, 0), Vector3f(0, 0, 0));
    camera->setFieldOfView(45.0);

    PtrMesh cubeMesh(new CubeMesh("Cube"));
    PtrShader shader1(new LambertShader());
    PtrShader shader2(new LambertShader());
    PtrShader shader3(new LambertShader());
    static_cast<LambertShader*>(shader2.data())->setColor(QColor(0, 0, 255, 255));
    static_cast<LambertShader*>(shader3.data())->setColor(QColor(0, 255, 0, 255));
    MeshInstance* meshInstances[12];
    for (int i = 0; i < 6; ++i)
    {
        meshInstances[i] = new MeshInstance(QString("Cube Instance %1").arg(i), cubeMesh);
        meshInstances[i]->setShader(shader1);
        scene.getRootNode().addChild(*meshInstances[i]);
    }
    for (int i = 6; i < 12; ++i)
    {
        meshInstances[i] = new MeshInstance(QString("Cube Instance %1").arg(i), cubeMesh);
        meshInstances[i]->setShader(shader2);
        scene.getRootNode().addChild(*meshInstances[i]);
    }
    float distance = 5.0f;
    meshInstances[0]->getLocalTransformation().setTranslation(Vector3f(distance,0,0));
    meshInstances[1]->getLocalTransformation().setTranslation(Vector3f(-distance,0,0));
    meshInstances[2]->getLocalTransformation().setTranslation(Vector3f(0,distance,0));
    meshInstances[3]->getLocalTransformation().setTranslation(Vector3f(0,-distance,0));
    meshInstances[4]->getLocalTransformation().setTranslation(Vector3f(0,0,distance));
    meshInstances[5]->getLocalTransformation().setTranslation(Vector3f(0,0,-distance));
    meshInstances[0]->setShader(shader3);
    meshInstances[4]->setShader(shader2);
    distance = 20.0f;
    meshInstances[6]->getLocalTransformation().setTranslation(Vector3f(distance,0,0));
    meshInstances[7]->getLocalTransformation().setTranslation(Vector3f(-distance,0,0));
    meshInstances[8]->getLocalTransformation().setTranslation(Vector3f(0,distance,0));
    meshInstances[9]->getLocalTransformation().setTranslation(Vector3f(0,-distance,0));
    meshInstances[10]->getLocalTransformation().setTranslation(Vector3f(0,0,distance));
    meshInstances[11]->getLocalTransformation().setTranslation(Vector3f(0,0,-distance));

    scene.getRootNode().addChild(*camera);

    GLWidget* widget = new GLWidget();
    widget->setScene(&scene);
    widget->setCamera(camera);
    scene.getRootNode().updateGeometricState(0.0, true);
    QTimer::singleShot(0, widget, SLOT(show()));
    return app.exec();
}
