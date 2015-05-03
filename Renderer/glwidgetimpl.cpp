#include <Qt>
#include <QEvent>
#include <QMouseEvent>

#include "Scene/transformation.h"
#include "Scene/camera.h"
#include "glrenderer.h"
#include "glwidgetimpl.h"

namespace GLDemo
{
    /**
     *
     */
    GLWidgetImpl::GLWidgetImpl(GLWidget& widget) :
        QGLWidget(&widget),
        m_glWidget(widget),
        m_renderer(0),
        m_scene(0)
    {
        m_renderer = new GLRenderer(*this);
    }


    /**
     *
     */
    GLWidgetImpl::~GLWidgetImpl()
    {
        delete m_renderer;
    }


    /**
     * Silently absorbs any tooltip events so that tool tips don't
     * pop up over the scene and force unnecessary updates.
     */
    bool  GLWidgetImpl::event(QEvent* event)
    {
        if (event->type() == QEvent::ToolTip)
        {
            event->accept();
            return true;
        }
        return QGLWidget::event(event);
    }


    /**
     *
     */
    void GLWidgetImpl::initializeGL()
    {
        if (!m_renderer->initialize())
        {
            // This should never happen, but just in case it does, here's a handy printline.
            // TODO: More effective error handling here.
            std::cout << "ERROR: Failed to initialize GL renderer" << std::endl;
        }
    }


    /**
     *
     */
    void GLWidgetImpl::paintGL()
    {
        if (!m_scene)
        {
            std::cout << "ERROR: No scene has been assigned to the widget." << std::endl;
            return;
        }

        // Display the loading screen if our render is loading a model,
        // is busy, or another widget is updating shared data somewhere.
        if (!m_renderer->renderScene(*m_scene))
        {
            std::cout << "ERROR: Failed to render the scene" << std::endl;
        }
    }


    /**
     *
     */
    void GLWidgetImpl::resizeGL(int width, int height)
    {
        assert(m_renderer);
        bool success = m_renderer->resize(width, height);
        assert(success);
        Q_UNUSED(success);
    }


    /**
     *
     */
    void  GLWidgetImpl::setScene(Scene* scene)
    {
        m_scene = scene;
    }


    /**
     *
     */
    void  GLWidgetImpl::setCamera(Camera* camera)
    {
        m_renderer->setCamera(camera);
    }


    /**
     *
     */
    void GLWidgetImpl::mousePressEvent(QMouseEvent *event)
    {
        event->accept();
        m_lastMousePos = event->pos();
        m_mousePosOnPress = m_lastMousePos;
    }


    /**
     *
     */
    void GLWidgetImpl::mouseMoveEvent(QMouseEvent *event)
    {
        event->accept();

        int dx = event->x() - m_lastMousePos.x();    // Positive to the right
        int dy = event->y() - m_lastMousePos.y();    // Positive downwards

        // We only allow specific combinations of mouse buttons and keyboard modifiers.
        // We don't use & because we only want to trap the event if only the left
        // button is down.
        if (event->buttons() == Qt::LeftButton)
        {
            if (event->modifiers() == Qt::ControlModifier)
            {
                Camera* camera = m_renderer->getCamera();
                if (!camera)
                {
                    return;
                }

                // Rotation speed is dependent on the viewport size.
                float xDist = static_cast<float>(dx) / width();
                float yDist = static_cast<float>(dy) / height();

                // Get our world vectors for the camera orientation.
                Vector3f  cameraPosition;
                Vector3f  viewVector;
                Vector3f  upVector;
                Vector3f  acrossVector;
                camera->calcWorldVectors(cameraPosition, viewVector, upVector, acrossVector);

                // NOTE: Vertical rotation is currently blorken. It appears to be rotating
                // about the wrong axis, despite everything here appearing to show the
                // correct value.
                const Vector3f  target(camera->getLookAt());
                Matrix3f rotateHorizontal, rotateVertical;
                rotateHorizontal.fromAxisAngle(xDist * M_PI, Vector3f(0,1,0));
                rotateVertical.fromAxisAngle(yDist * M_PI, -acrossVector);
                cameraPosition = rotateVertical * (cameraPosition - target) + target;
                cameraPosition = rotateHorizontal * (cameraPosition - target) + target;

                // Re-align our vectors so make them orthogonal
                const Vector3f backward( cameraPosition - target );
                acrossVector = backward.unitCross(Vector3f(0,1,0));
                upVector = acrossVector.unitCross(backward);

                // Update the camera view.
                camera->setCameraView(cameraPosition, upVector, target);
                updateGL();
            }
            else if (event->modifiers() == Qt::ShiftModifier)
            {
            }
            else if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))
            {
            }
        }

        m_lastMousePos = event->pos();
    }


    /**
     *
     */
    void GLWidgetImpl::wheelEvent(QWheelEvent *event)
    {
        event->accept();

        // We only allow specific combinations of mouse buttons and keyboard modifiers.
        // See the class detailed documentation for the full list, but the ones relevant
        // for mouse wheel events are:
        if (event->modifiers() == Qt::NoModifier)
        {
        }
        else if (event->modifiers() == Qt::ShiftModifier)
        {
        }
        else if (event->modifiers() == Qt::ControlModifier)
        {
        }
        else if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))
        {
        }
    }

}

