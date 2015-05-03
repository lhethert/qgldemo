#ifndef GLDEMO_VERTEX_H
#define GLDEMO_VERTEX_H

#include "Math/vector3.h"
#include "Math/vector2.h"

namespace GLDemo
{

    /**
     * \brief Represents a vertex in 3D space.
     * \note It is imperative that absolutely no virtual functions are used in these classes.
     *       having anything in the vtable whatsoever will destroy any chance of using these
     *       objects in the rendering API
     */
    class Vertex
    {
    public:
//        Vertex() : m_position() {}

//        Vertex(float px, float py, float pz,
//               float nx, float ny, float nz,
//               float u, float v) :
//            m_position(px, py, pz)
//        {
//        }

//        Vertex(const Vector3f& position, const Vector3f& normal, const Vector2f& texcoords) :
//            m_position(position)
//        {
//        }

//        Vector3f m_position;

        Vertex() : m_position(), m_normal(), m_texcoords() {}

        Vertex(float px, float py, float pz,
               float nx, float ny, float nz,
               float u, float v) :
            m_position(px, py, pz),
            m_normal(nx, ny, nz),
            m_texcoords(u, v)
        {
        }

        Vertex(const Vector3f& position, const Vector3f& normal, const Vector2f& texcoords) :
            m_position(position),
            m_normal(normal),
            m_texcoords(texcoords)
        {
        }

        Vector3f m_position;
        Vector3f m_normal;
        Vector2f m_texcoords;
    };

}

#endif //VERTEX_H
