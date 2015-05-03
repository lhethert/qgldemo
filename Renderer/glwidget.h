#ifndef GLDEMO_GLWIDGET_H
#define GLDEMO_GLWIDGET_H

#include <iostream>

#include <QWidget>
#include <QSize>

#include "Math/vector3.h"

class QString;

namespace GLDemo
{
    class GLWidgetImpl;
    class Scene;
    class Camera;

    /**
     * \brief Widget for interactive rendering of a Scene object using OpenGL
     * \note We use a QWidget with a QGLWidget embedded to work around a number
     *       of platform specific bugs encountered with Qt.
     */
    class GLWidget : public QWidget
    {
        Q_OBJECT

    public:
        GLWidget(QWidget* parent = 0);
        ~GLWidget();

        void  setScene(Scene* scene);
        void  setCamera(Camera* camera);

        virtual QSize sizeHint() const;

    signals:
        void   widthChanged(int width);
        void   heightChanged(int width);

    private:
        GLWidgetImpl*  m_pImpl;
    };

}

#endif
