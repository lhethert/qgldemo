#ifndef GLDEMO_GLWIDGETIMPL_H
#define GLDEMO_GLWIDGETIMPL_H

#include <queue>

#include <QGLWidget>
#include <QString>
#include <QPoint>

#include <iostream>

#include "glwidget.h"

namespace GLDemo
{
    class Camera;
    class GLRenderer;

    /**
     * \brief Implementation of the GLWidget, extending the QGLWidget.
     */
    class GLWidgetImpl : public QGLWidget
    {
        Q_OBJECT
		
    public:
        GLWidgetImpl(GLWidget& widget);
        ~GLWidgetImpl();

        void  setScene(Scene* scene);
        void  setCamera(Camera* camera);

    protected:
        virtual void  initializeGL();
        virtual void  paintGL();
        virtual void  resizeGL(int width, int height);

        virtual bool  event(QEvent* event);
        virtual void  mousePressEvent(QMouseEvent* event);
        virtual void  mouseMoveEvent(QMouseEvent* event);
        virtual void  wheelEvent(QWheelEvent* event);

    private:
        GLWidget&         m_glWidget;
        GLRenderer*       m_renderer;
        Scene*            m_scene;
        QPoint            m_lastMousePos;
        QPoint            m_mousePosOnPress;

        //typedef std::queue<QueuedInteraction*> InteractionQueue;
        //InteractionQueue interactionQueue_;
        //QDateTime loadingTimeStarted_;
        //QTimer    loadingTimer_;
    };

}

#endif
