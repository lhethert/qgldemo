#ifndef GLDEMO_RENDERER_H
#define GLDEMO_RENDERER_H

namespace GLDemo
{
    class MeshInstance;
    class Scene;

    /**
     * \brief Interface for processing SceneNodes for rendering.
     */
    class Renderer
    {
    public:
        /**
         * Process a MeshInstance.
         */
        virtual bool process(const MeshInstance& instance) = 0;
    };

}

#endif
