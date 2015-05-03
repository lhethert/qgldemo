#ifndef GLDEMO_SCENENODE_H
#define GLDEMO_SCENENODE_H

#include "spatialentity.h"

#include <vector>
#include <algorithm>

namespace GLDemo
{
    class Renderer;


    /**
     * \brief Represents an node in a scene graph. Nodes can have
     *        child-nodes, where each child's coordinate system is
     *        relative to its parent's coordinate system.
     */
    class SceneNode : public SpatialEntity
    {
    public:
        SceneNode();
        SceneNode(const SceneNode& node);
        SceneNode(const QString& name);
        ~SceneNode();

        void addChild(SpatialEntity& child);

        SpatialEntity& getChild(int index);
        SpatialEntity& detachChild(SpatialEntity& p);
        SpatialEntity& detachChildAt(int index);

        virtual bool draw(Renderer* renderer);

        virtual SceneNode* clone() const;

    protected:
        virtual void updateWorldData(double time);

    private:
        std::vector<SpatialEntity*> m_children;
    };

}

#endif
