#ifndef GLDEMO_OBJECT_H
#define GLDEMO_OBJECT_H

#include <list>

#include <QString>

#include "controller.h"

namespace GLDemo
{

    /**
     * \brief Represents a named object.
     *
     * Objects are simple named entities which exist in some context. Generally,
     * the names are used to uniquely identify the object. Objects are also able
     * to have \a Controller objects associated with them, which are capable of
     * manipulating their properties at regular intervals.
     */
    class Object
    {
    public:
        Object(const QString& instanceName);
        Object(const Object& object);

        virtual ~Object();

        const QString& instanceName() const       { return m_instanceName; }
        void setInstanceName(const QString& name) { m_instanceName = name; }

        void addController(PtrController& controller);
        void removeController(PtrController& controller);
        void updateControllers(double time);

        virtual Object* clone() const = 0;

        QString m_instanceName;
        std::list<PtrController > m_controllers;
    };

}


#endif //OBJECT_H
