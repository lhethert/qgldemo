#include "mesh.h"

namespace GLDemo
{
    /**
     * \param name The unique name of this mesh. Note that meshes are uniquely
     *             identified by their name, so in order to ensure that the data
     *             is rendered correctly, the names should be unique.
     *
     * Creates a new instance of a mesh with the specified unique name.
     */
    Mesh::Mesh(const QString& name) :
        Object(name),
        m_vertices(),
        m_elements()
    {
    }


    /**
     * Creates a copy of the mesh. This will perform a deep copy of the data, including
     * all vertices and elements.
     */
    Mesh::Mesh(const Mesh& mesh) :
        Object(mesh),
        m_vertices(mesh.m_vertices),
        m_elements()
    {
        const std::list<ElementList>& meshElems = mesh.m_elements;
        for (std::list<ElementList>::const_iterator eIter = meshElems.begin(); eIter != meshElems.end(); ++eIter)
        {
            m_elements.push_front(*eIter);
        }
    }


    /**
     *
     */
    Mesh::~Mesh()
    {
    }

}
