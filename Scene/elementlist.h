#ifndef GLDEMO_PRIMITIVE_COLLECTION_H
#define GLDEMO_PRIMITIVE_COLLECTION_H

#include <vector>
#include <cassert>

#include <QGLFunctions>

namespace GLDemo
{

    /**
     * \brief Represents a collection of primitives of a particular type. Multiple
     *        primitive collections make up a mesh.
     */
    class ElementList
    {
    public:
        enum ElementType
        {
            POINTS      = GL_POINTS,
            LINE_LIST   = GL_LINES,
            TRI_LIST    = GL_TRIANGLES
        };


        /**
         * \param type The type of element list to create.
         *
         * Creates an empty element list of the specified type.
         */
        ElementList(ElementType type) :
            m_indices(),
            m_primitiveType(type)
        {
        }


        /**
         * \param indices The indices of the vertices for each of the elements in the list
         * \param pType   The type of element list to create.
         *
         * Creates a new element list of the specified type from the input array.
         */
        ElementList(const std::vector<int>& indices, ElementType pType) :
            m_primitiveType(pType)
        {
            m_indices.resize(indices.size());
            std::copy(indices.begin(), indices.end(), m_indices.begin());
        }


        /**
         * Creates a deep copy of the element list.
         */
        ElementList(const ElementList& pCol) :
            m_indices(pCol.m_indices),
            m_primitiveType(pCol.m_primitiveType)
        {
        }

        std::vector<unsigned>& getIndices() { return m_indices; }

        ElementType  getElementType() const    { return m_primitiveType; }
        void setElementType(ElementType pType) { m_primitiveType = pType; }

    private:
        std::vector<unsigned> m_indices;
        ElementType           m_primitiveType;
    };

}

#endif //PRIMITIVE_COLLECTION_H
