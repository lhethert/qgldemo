#ifndef GLDEMO_SPATIAL_ENTITY_H
#define GLDEMO_SPATIAL_ENTITY_H

#include <QSharedPointer>

#include "transformation.h"
#include "object.h"

namespace GLDemo
{
    class Renderer;
    class SceneNode;


    /**
     * \brief Represents an object in 3D space; i.e. one with a spatial transformation
     *        associated with it.
     */
    class SpatialEntity : public Object
    {
    public:
        virtual ~SpatialEntity();

        void setWorldTransformation(const Transformation& t) { m_tWorld = t; }
        const Transformation& getWorldTransformation() const { return m_tWorld; }
        Transformation&       getWorldTransformation()       { return m_tWorld; }

        void setLocalTransformation(const Transformation& t) { m_tLocal = t; }
        const Transformation& getLocalTransformation() const { return m_tLocal; }
        Transformation&       getLocalTransformation()       { return m_tLocal; }

        SpatialEntity* getParent() const { return m_parent; }

        void updateGeometricState(double time, bool initiatedUpdate);

        virtual bool draw(Renderer* renderer);
        virtual SpatialEntity* clone() const = 0;

    protected:
        SpatialEntity();
        SpatialEntity(const QString& name);
        SpatialEntity(const SpatialEntity&);

        virtual void updateWorldData(double time);

        /**
         * \internal Only the SceneNode subclass should ever need to invoke this.
         */
        void setParent(SpatialEntity* p) { m_parent = p; }

    private:
        SpatialEntity* m_parent;
        Transformation m_tLocal;
        Transformation m_tWorld;

        // TODO: Add bounding entities here, once created
        //Bound m_bLocal;
        //Bound m_bWorld;

        friend class SceneNode;
    };

    typedef QSharedPointer<SpatialEntity> PtrSpatialEntity;
}

#endif //SPATIAL_ENTITY_H
