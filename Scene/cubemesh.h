#ifndef GLDEMO_CUBE_MESH_H
#define GLDEMO_CUBE_MESH_H

#include <QString>

#include "mesh.h"

namespace GLDemo
{

    /**
     * \brief Simple mesh representing a cube.
     */
    class CubeMesh : public Mesh
    {
    public:
        CubeMesh(const QString& name);
        ~CubeMesh();

        virtual CubeMesh* clone() const;

    private:
        void createVertices();
        void createIndices();
    };

}

#endif //CUBE_MESH_H
