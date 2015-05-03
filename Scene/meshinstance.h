#ifndef GLDEMO_GEOMETRIC_ENTITY_H
#define GLDEMO_GEOMETRIC_ENTITY_H

#include "Renderer/shader.h"
#include "spatialentity.h"
#include "vertex.h"
#include "mesh.h"

namespace GLDemo
{
    class Renderer;

    /**
     * \brief Represents an instance of a mesh in a scene, found at a
     * particular location in world space.
     */
    class MeshInstance : public SpatialEntity
    {
    public:
        MeshInstance(const QString& name, PtrMesh& mesh);
        MeshInstance(const MeshInstance& ge);
        ~MeshInstance();

        PtrMesh&       getMesh()       { return m_mesh; }
        const PtrMesh& getMesh() const { return m_mesh; }

        PtrShader& getShader()             { return m_shader; }
        const PtrShader& getShader() const { return m_shader; }
        void setShader(PtrShader& shader)  { m_shader = shader; }

        virtual MeshInstance* clone() const;

    protected:
        virtual void updateWorldData(double time);
        virtual bool draw(Renderer* renderer);

        PtrMesh   m_mesh;
        PtrShader m_shader;
    };
}


#endif //GEOMETRIC_ENTITY_H
