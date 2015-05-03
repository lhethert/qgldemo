#ifndef GLDEMO_MESH_H
#define GLDEMO_MESH_H

#include <list>
#include <vector>

#include <QString>
#include <QSharedPointer>

#include "object.h"
#include "vertex.h"
#include "elementlist.h"

namespace GLDemo
{

    /**
     * \brief Represents a mesh dataset containing vertices and elements that connect them.
     */
    class Mesh : public Object
    {
    public:
        Mesh(const QString& name);
        Mesh(const Mesh& mesh);
        ~Mesh();

        std::vector<Vertex>&     getVertices()     { return m_vertices; }
        std::list<ElementList>&  getElementLists() { return m_elements; }

        /**
         * \param type The type of element list to add
         * \return A reference to the newly created element list.
         */
        ElementList& addElementList(ElementList::ElementType type)
        {
            m_elements.push_front(ElementList(type));
            return m_elements.front();
        }

    protected:
        std::vector<Vertex>    m_vertices;
        std::list<ElementList> m_elements;
    };

    typedef QSharedPointer<Mesh> PtrMesh;

}


#endif //MESH_H
