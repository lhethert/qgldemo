#ifndef GLDEMO_HELPERS_H
#define GLDEMO_HELPERS_H

#include <iostream>
#include <QString>

namespace GLDemo
{

    /**
     * Simple struct used for the deletion of objects in containers
     */
    struct DeleteObject
    {
        template <class T>
        void operator()(T *t) const
        {
            delete t;
        }
    };

    /**
     * Make it easy to print QString values to std::cout or other std streams.
     * Note that this function is marked inline so that it will be compiled into
     * any file that includes it. There is no associated .cpp file for that reason.
     */
    inline std::ostream& operator<<(std::ostream& os, const QString& t)
    {
        os << std::string(t.toLatin1().constData());
        return os;
    }
}

#endif //HELPERS_H
