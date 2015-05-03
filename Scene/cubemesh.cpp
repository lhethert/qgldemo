#include "cubemesh.h"	
#include "vertex.h"

namespace GLDemo
{
    /**
     * \param name The unique name of this mesh.
     *
     * Creates a new cube mesh with the specified (unique) name.
     */
    CubeMesh::CubeMesh(const QString& name) :
        Mesh(name)
    {
        createVertices();
        createIndices();
    }


    /**
     *
     */
    CubeMesh::~CubeMesh()
    {
    }


    /**
     * Return a copy of this mesh data.
     */
    CubeMesh* CubeMesh::clone() const
    {
        return new CubeMesh(*this);
    }


    /**
     * Create the vertices of the cube.
     */
    void CubeMesh::createVertices()
    {	
        m_vertices.reserve(8);

        Vector3f positions[8] = {
            Vector3f(-1.0, -1.0, -1.0),
            Vector3f( 1.0, -1.0, -1.0),
            Vector3f(-1.0,  1.0, -1.0),
            Vector3f( 1.0,  1.0, -1.0),
            Vector3f(-1.0, -1.0,  1.0),
            Vector3f( 1.0, -1.0,  1.0),
            Vector3f(-1.0,  1.0,  1.0),
            Vector3f( 1.0,  1.0,  1.0),
        };

        Vector3f normals[6] = {
            Vector3f(1, 0, 0),
            Vector3f(-1, 0, 0),
            Vector3f(0, 1, 0),
            Vector3f(0, -1, 0),
            Vector3f(0, 0, 1),
            Vector3f(0, 0, -1)
        };

        Vector2f texCoords(0.0, 0.0);

        // Make sure the nodes of each face are duplicated.
        m_vertices.push_back(Vertex(positions[0], normals[5], texCoords));
        m_vertices.push_back(Vertex(positions[1], normals[5], texCoords));
        m_vertices.push_back(Vertex(positions[3], normals[5], texCoords));
        m_vertices.push_back(Vertex(positions[2], normals[5], texCoords));

        m_vertices.push_back(Vertex(positions[1], normals[0], texCoords));
        m_vertices.push_back(Vertex(positions[5], normals[0], texCoords));
        m_vertices.push_back(Vertex(positions[7], normals[0], texCoords));
        m_vertices.push_back(Vertex(positions[3], normals[0], texCoords));

        m_vertices.push_back(Vertex(positions[0], normals[1], texCoords));
        m_vertices.push_back(Vertex(positions[2], normals[1], texCoords));
        m_vertices.push_back(Vertex(positions[6], normals[1], texCoords));
        m_vertices.push_back(Vertex(positions[4], normals[1], texCoords));

        m_vertices.push_back(Vertex(positions[4], normals[4], texCoords));
        m_vertices.push_back(Vertex(positions[6], normals[4], texCoords));
        m_vertices.push_back(Vertex(positions[7], normals[4], texCoords));
        m_vertices.push_back(Vertex(positions[5], normals[4], texCoords));

        m_vertices.push_back(Vertex(positions[0], normals[3], texCoords));
        m_vertices.push_back(Vertex(positions[4], normals[3], texCoords));
        m_vertices.push_back(Vertex(positions[5], normals[3], texCoords));
        m_vertices.push_back(Vertex(positions[1], normals[3], texCoords));

        m_vertices.push_back(Vertex(positions[3], normals[2], texCoords));
        m_vertices.push_back(Vertex(positions[7], normals[2], texCoords));
        m_vertices.push_back(Vertex(positions[6], normals[2], texCoords));
        m_vertices.push_back(Vertex(positions[2], normals[2], texCoords));
    }


    /**
     * Create the indices of the cube.
     */
    void CubeMesh::createIndices()
    {
        ElementList& pCol = addElementList(ElementList::TRI_LIST);
        std::vector<unsigned>& indices = pCol.getIndices();

        int offset = 0;
        for (int i = 0; i < 6; ++i)
        {
            offset = i * 4;
            indices.push_back(offset);
            indices.push_back(offset + 2);
            indices.push_back(offset + 1);
            indices.push_back(offset + 3);
            indices.push_back(offset + 2);
            indices.push_back(offset);
        }
    }

}

