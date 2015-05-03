#ifndef GLDEMO_SCENE_H
#define GLDEMO_SCENE_H

#include <QSharedPointer>

#include "scenenode.h"

namespace GLDemo
{
    class Renderer;

    /**
     * \brief Top level scene class containing our root-level scene node.
     *
     * Currently doesn't do very much, but will later house things like the
     * background color and various other scene-level properties.
     */
    class Scene
    {
    public:
        Scene() {}

        SceneNode& getRootNode() { return m_rootNode; }

    private:
        SceneNode m_rootNode;
    };

}

#endif
