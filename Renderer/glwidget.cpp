#include <iostream>

#include <QGLContext>
#include <QVBoxLayout>

#include "glwidget.h"
#include "glwidgetimpl.h"

namespace GLDemo
{

    /**
     *
     */
    GLWidget::GLWidget(QWidget* parent) :
        QWidget(parent),
        m_pImpl(0)
    {
        QVBoxLayout* layout = new QVBoxLayout();
        setLayout(layout);
        m_pImpl = new GLWidgetImpl(*this);
        layout->addWidget(m_pImpl);
    }


    /**
     *
     */
    GLWidget::~GLWidget()
    {
    }


    /**
     * \param scene The new scene the widget should render.
     *
     * The old scene is \em not deleted, since the widget does not take ownership of
     * the scene.
     */
    void  GLWidget::setScene(Scene* scene)
    {
        m_pImpl->setScene(scene);
    }


    /**
     * \param camera The camera representing the viewpoint from which the scene is to
     * be rendered.
     */
    void  GLWidget::setCamera(Camera* camera)
    {
        m_pImpl->setCamera(camera);
    }


    /**
     *
     */
    QSize GLWidget::sizeHint() const
    {
        return QSize(400, 400);
    }

}

