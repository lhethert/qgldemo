#include "Renderer/renderer.h"
#include "meshinstance.h"

namespace GLDemo
{
    /**
     * \param name  The unique name of this mesh instance.
     * \param mesh  The shared mesh data associated with this instance.
     */
    MeshInstance::MeshInstance(const QString& name, PtrMesh& mesh) :
        SpatialEntity(name), 
        m_mesh(mesh)
    {
    }


    /**
     * Creates a copy of the mesh instance. Note that the shared mesh data
     * is not copied - it is reference from the new mesh instance.
     */
    MeshInstance::MeshInstance(const MeshInstance& ge) :
        SpatialEntity(ge), 
        m_mesh(ge.m_mesh)
    {
    }


    /**
     *
     */
    MeshInstance::~MeshInstance()
    {
    }


    /**
     * \return entity Returns a clone of this mesh instance.
     */
    MeshInstance* MeshInstance::clone() const
    {
        MeshInstance* entity = new MeshInstance(*this);
        return entity;
    }


    /**
     * For now, does not substantially differ from the SpatialEntity
     * implementation, but this may change depending on whether or not
     * shaders require transform data.
     */
    void MeshInstance::updateWorldData(double time)
    {
        SpatialEntity::updateWorldData(time);
    }


    /**
     * Override the draw command, using the visitor pattern to ensure that
     * the correct type of entity is rendered.
     */
    bool MeshInstance::draw(Renderer* renderer)
    {
        return renderer->process(*this);
    }

}

