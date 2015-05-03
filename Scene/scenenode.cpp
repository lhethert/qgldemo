#include "scenenode.h"
#include "helpers.h"

namespace GLDemo
{

    SceneNode::SceneNode() :
        SpatialEntity(),
        m_children()
    {
    }


    SceneNode::SceneNode(const SceneNode& node) :
        SpatialEntity(node),
        m_children()
    {
        for (std::vector<SpatialEntity*>::const_iterator i = node.m_children.begin(); i < node.m_children.end(); ++i)
        {
            SpatialEntity* childNode = (*i)->clone();
            addChild(*childNode);
        }
    }



    SceneNode::SceneNode(const QString& name) :
        SpatialEntity(name),
        m_children()
    {
    }


    SceneNode::~SceneNode()
    {
        std::for_each(m_children.begin(), m_children.end(), DeleteObject());
    }


    /**
     * \param child  The entity to add as a child of this scene node.
     * \note The SceneNode will take ownership of the child, as a SceneNode can
     * have only a single parent.
     */
    void SceneNode::addChild(SpatialEntity& child)
    {
        child.setParent(this);
        m_children.push_back(&child);
    }


    /**
     * \param index  The index of the child to return.
     */
    SpatialEntity& SceneNode::getChild(int index)
    {
        return *m_children[index];
    }


    /**
     * \param entity The entity to detach from this node. Ownership is returned
     * to the caller.
     * \param reference to the entity that was detached.
     */
    SpatialEntity& SceneNode::detachChild(SpatialEntity& entity)
    {
        m_children.erase(std::remove(m_children.begin(), m_children.end(), &entity));
        return entity;
    }


    /**
     * \param index The index of the child to detach. Ownership is returned to the
     * caller.
     * \return The entity that was detached.
     */
    SpatialEntity& SceneNode::detachChildAt(int index)
    {
        std::vector<SpatialEntity*>::iterator childIter = (m_children.begin() + index);
        SpatialEntity* child = *childIter;
        m_children.erase(childIter);
        return *child;
    }


    /**
     * Overrides the draw method in order to forward it to its children.
     */
    bool SceneNode::draw(Renderer* renderer)
    {
        // If no shader is present, simply draw children
        bool success = true;
        for (std::vector<SpatialEntity*>::const_iterator i = m_children.begin(); success && i < m_children.end(); ++i)
        {
            success = (*i)->draw(renderer);
        }

        return success;
    }


    /**
     * Clones the SceneNode, returning an identical copy.
     */
    SceneNode* SceneNode::clone() const
    {
        SceneNode* newNode = new SceneNode(*this);
        return newNode;
    }


    /**
     * Overrides the function to update the world data of itself first, then each of its
     * children.
     */
    void SceneNode::updateWorldData(double time)
    {
        SpatialEntity::updateWorldData(time);

        for (std::vector<SpatialEntity*>::iterator i = m_children.begin(); i < m_children.end(); ++i)
        {
            (*i)->updateGeometricState(time, false);
        }
    }

}
