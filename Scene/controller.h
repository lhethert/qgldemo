#ifndef GLDEMO_CONTROLLER_H
#define GLDEMO_CONTROLLER_H

#include <QSharedPointer>

namespace GLDemo
{
    class Object;


    /**
     * \brief Controller class used for updating elements of a model.
     */
    class Controller
    {
    public:
        virtual ~Controller();

        virtual void update(double time) = 0;

    protected:
        Controller();
        Controller(const Controller&);
    };

    typedef QSharedPointer<Controller> PtrController;

}

#endif //CONTROLLER_H
