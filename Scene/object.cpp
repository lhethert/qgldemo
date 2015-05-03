#include <algorithm>

#include <QSharedPointer>

#include "object.h"


namespace GLDemo
{

    /**
     * \param instanceName The name of this object.
     */
    Object::Object(const QString& instanceName) :
        m_instanceName(instanceName),
        m_controllers()
    {
    }


    /**
     *
     */
    Object::Object(const Object& object) :
        m_instanceName(object.m_instanceName),
        m_controllers(object.m_controllers)
    {

    }


    /**
     *
     */
    Object::~Object()
    {
        m_controllers.clear();
    }


    /**
     * \param controller The controller to add to the object.
     */
    void Object::addController(PtrController& controller)
    {
        m_controllers.push_front(controller);
    }


    /**
     * \param controller The controller to remove from the object.
     */
    void Object::removeController(PtrController& controller)
    {
        m_controllers.erase(std::remove(m_controllers.begin(), m_controllers.end(), controller));
    }


    /**
     *
     */
    void Object::updateControllers(double time)
    {
        // Not yet implemented. This will iterate over all controllers and invoke the
        // update function on them.
    }
}

