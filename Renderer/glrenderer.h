#ifndef GLDEMO_GLRENDERER_H
#define GLDEMO_GLRENDERER_H

#include "renderer.h"

class QPaintDevice;

namespace GLDemo
{
    class Camera;
    class Scene;
    class GLRendererImpl;

    /**
     * \brief Renders a scene using OpenGL
     */
    class GLRenderer : public Renderer
    {
    public:
        enum VertexAttributeLocation
        {
            Position = 0,
            Normal = 1
        };

        GLRenderer(QPaintDevice& device);
        virtual ~GLRenderer();

        void            setCamera(Camera* camera);
        Camera*         getCamera();
        const Camera*   getCamera() const;

        bool   initialize();
        bool   renderScene(Scene& scene);

        bool   resize(int width, int height);
        int    getWidth() const;
        int    getHeight() const;

        virtual bool process(const MeshInstance& instance);

    private:
        GLRendererImpl*  m_pImpl;
    };

}

#endif

