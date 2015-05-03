#include "spatialentity.h"

namespace GLDemo
{

    /**
     * Creates a new spatial entity with the default name.
     */
    SpatialEntity::SpatialEntity() :
        Object("Unnamed SpatialEntity"),
        m_parent(0),
        m_tLocal(),
        m_tWorld()
    {
    }


    /**
     * \param Name  The unique name for this spatial entity.
     */
    SpatialEntity::SpatialEntity(const QString& name) :
        Object(name),
        m_parent(0),
        m_tLocal(),
        m_tWorld()
    {
    }


    /**
     *
     */
    SpatialEntity::SpatialEntity(const SpatialEntity& entity) :
        Object(entity.instanceName()),
        m_parent(entity.m_parent),
        m_tLocal(entity.m_tLocal),
        m_tWorld(entity.m_tWorld)
    {
    }


    /**
     *
     */
    SpatialEntity::~SpatialEntity()
    {
    }


    /**
     * \param time  Time the update was triggered. Can be used by subclasses to perform
     *              simulation specific activities.
     * \param initiatedUpdate If true, this indicates that the geometric update was triggered
     *        from this node. If this is the case, we need to update the bounding boxes of all
     *        parent nodes in the tree after updating the world date of all child nodes.
     */
    void SpatialEntity::updateGeometricState(double time, bool initiatedUpdate)
    {
        updateWorldData(time);
        // TODO: UpdateWorldBound();

        if (initiatedUpdate)
        {
            // TODO: PropagateBoundToRoot();
        }
    }


    /**
     * \param renderer  The renderer to use to draw this entity.
     *
     * Draw this entity. Subclasses can override this function in order to invoke the
     * specific functions on the renderer that they require in order to render themselves.
     */
    bool SpatialEntity::draw(Renderer* renderer)
    {
        // Essentially uses the visitor design pattern.
        return true;
    }


    /**
     * \param time  The time at which this update takes place
     *
     * Updates this object's world data - i.e. its world transformation is combined with
     * its local transformation data. Subclasses can override this to do more specific
     * tasks, like update their child elements (in the case of the SceneNode class).
     */
    void SpatialEntity::updateWorldData(double time)
    {
        updateControllers(time);

        // If a parent entity exists, combine our transform matrix with its world matrix.
        if (m_parent)
        {
            m_tWorld.combine(m_parent->m_tWorld, m_tLocal);
            //m_tWorld.combine(m_tLocal, m_parent->m_tWorld);
        }
        else
        {
            m_tWorld = m_tLocal;
        }
    }

}
