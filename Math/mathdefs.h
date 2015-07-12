#ifndef GLDEMO_MATHDEFS_H
#define GLDEMO_MATHDEFS_H

#include <cfloat>
#include <cmath>
#include <limits>

namespace GLDemo
{

    /**
     * \brief General purpose Math class designed to provide general purpose math
     *        functions so that we can swap between single / double precision if necessary.
     */
    template<class Real>
    class Math
    {
    public:
        // Global functions. Most of these just forward to the standard library functions
        // for now, but it leaves room for optimisations in the future if needed.
        static bool FEqual(const Real&, const Real&);
        static Real FAbs(const Real&);
        static Real Pow(const Real&, const Real& power);
        static Real Sqrt(const Real&);
        static Real Cbrt(const Real&);

        // Trigonometry functions, again, most of these just forward on to the standard library
        // functions, but we can potentially optimise these later if needed.
        static Real Sin(const Real&);
        static Real Cos(const Real&);
        static Real Tan(const Real&);
        static Real ASin(const Real&);
        static Real ACos(const Real&);
        static Real ATan(const Real&);
        static Real ATan2(const Real&);

        // Funtions for computing the roots of equations.
        static bool QuadraticRealRoots(const Real& a, const Real& b, const Real& c, Real roots[]);
        static bool CubicRealRoots(const Real& a, const Real& b, const Real& c, const Real& d, Real roots[]);

        static const Real Epsilon();

        // General purpose constants for convenience.
        static const float PI;
        static const float TWO_PI;
        static const float HALF_PI;
        static const float INV_PI;
        static const float INV_TWO_PI;
        static const float MAX_REAL;
    };

    template<class Real>
    inline const Real Math<Real>::Epsilon()
    {
        return 0.000001;
    }

    template<class Real> const float Math<Real>::PI = 3.141592654f;
    template<class Real> const float Math<Real>::TWO_PI = PI * 2;
    template<class Real> const float Math<Real>::HALF_PI = PI / 2;


    /**
     * Determine whether two floating-point values are approximately equal using the epslion
     * tolerance.
     */
    template<class Real> inline bool Math<Real>::FEqual(const Real& a, const Real& b)
    {
        return std::fabs(a - b) <= std::fmax((Real)1.0, std::fmax(std::fabs(a), std::fabs(b)) ) * Math<Real>::Epsilon();
    }


    template<class Real>
    inline Real Math<Real>::FAbs(const Real& a)
    {
        return std::fabs(a);
    }


    template<class Real>
    inline Real Math<Real>::Pow(const Real& a, const Real& b)
    {
        return std::pow(a, b);
    }


    template<class Real>
    inline Real Math<Real>::Sqrt(const Real& a)
    {
        return std::sqrt(a);
    }


    template<class Real>
    inline Real Math<Real>::Cbrt(const Real& a)
    {
        return std::pow(a, 1/3);
    }



    template<class Real>
    inline Real Math<Real>::Sin(const Real& a)
    {
        return std::sin(a);
    }


    template<class Real>
    inline Real Math<Real>::Cos(const Real& a)
    {
        return std::cos(a);
    }


    template<class Real>
    inline Real Math<Real>::Tan(const Real& a)
    {
        return std::tan(a);
    }


    template<class Real>
    inline Real Math<Real>::ASin(const Real& a)
    {
        return std::asin(a);
    }


    template<class Real>
    inline Real Math<Real>::ACos(const Real& a)
    {
        return std::acos(a);
    }


    template<class Real>
    inline Real Math<Real>::ATan(const Real& a)
    {
        return std::atan(a);
    }


    template<class Real>
    inline Real Math<Real>::ATan2(const Real& a)
    {
        return std::atan2(a);
    }


    /**
     * Solves a quadratic equation of the format, 'a*x^2 + b*x + c', determining any real roots
     * The function will test the discriminant to determine whether any complex roots exist,
     * and will return TRUE only if two real roots (both of which may be identical) can be
     * obtained.
     * The function does not perform any complex arithmetic, and will not return any complex roots.
     */
    template<class Real>
    bool Math<Real>::QuadraticRealRoots(const Real &a, const Real &b, const Real &c, Real roots[])
    {
        bool twoRealRoots = true;

        Real discriminant = Pow(b, 2) - 4 * a * c;

        //Test the discriminant to determin the number and type of roots
        if (discriminant >= 0)
        {
            //Discriminant > 0 means there are two distinct real roots
            //Discriminant == 0 means there are two coincidental real roots
            roots[0] = (-b + Sqrt(discriminant) ) / (2 * a);
            roots[1] = (-b - Sqrt(discriminant) ) / (2 * a);
        }
        else
        {
            //Discriminant < 0 means there are two complex roots
            return false;
        }

        return true;
    }


    /**
     * Solves a cubic equation of the format, 'a*x^3 + b*x^2 + c*x + d = 0', determining any real roots
     * The function will test the discriminant to determine whether any complex roots exist,
     * and will return TRUE only if three real roots (two or more of which may be identical) can be
     * obtained.
     * The function does not perform any complex arithmetic, and will not return any complex roots.
     *
     * Algorithm obtained from URL: http://terpconnect.umd.edu/~nsw/ench250/cubiceq.htm
     */
    template<class Real>
    bool Math<Real>::CubicRealRoots(const Real& a, const Real& b, const Real& c, const Real& d, Real roots[])
    {
        bool threeRealRoots = true;

        //Calculate intermediate values p and q
        Real p = (3 * c / a - Pow(b / a, 2)) / 3;
        Real q = (2 * Pow(b / a, 3) - 9 * b * c / a / a + 27 * d / a ) / 27;

        //Calculate discriminant
        Real discriminant = Pow(p / 3, 3) + Pow(q / 2, 2);

        Real y1, y2, y3;
        y1 = y2 = y3 = 0;

        //Depending on the sign of the discriminant, follow different strategy to find the roots
        if (discriminant >= 0)
        {
            //For discriminant > 0, there are one real and two complex roots
            //For discriminant = 0, there are three real roots, at least two of which are equal

            //Calculate components u and v
            Real u = Cbrt(-q / 2 + Sqrt(discriminant));
            Real v = Cbrt(-q / 2 - Sqrt(discriminant));

            //Find the three transformed roots
            y1 = u + v;

            //assuming that the only way we can obtain the two below (without implementing complex logic)
            //is to test for u == v, and resolve a shortened version of the equation.
            //y2 = -(u + v) / 2 + i (u - v) * sqrt(3) / 2;
            //y3 = -(u+v)/2 - i (u-v)*sqrt(3)/2
            if (FEqual(u, v))
            {
                y2 = -(u + v) / 2;
                y3 = -(u + v) / 2;
            }
            else
            {
                threeRealRoots = false;
            }
        }
        else
        {
            //Alternately, for discriminant < 0, there are 3 distinct real roots. A trigonometric formulation is preferable.
            Real phi = ACos(-q / 2 / Sqrt(Pow(FAbs(p), 3) / 27) );

            y1 =  2 * sqrt(FAbs(p) / 3) * cos(phi / 3);
            y2 = -2 * sqrt(FAbs(p) / 3) * cos((phi + PI) / 3);
            y3 = -2 * sqrt(FAbs(p) / 3) * cos((phi - PI) / 3);
        }

        //Finally, find the three roots
        roots[0] = y1 - b / a / 3;

        if (threeRealRoots)
        {
            roots[1] = y2 - b / a / 3;
            roots[2] = y3 - b / a / 3;
        }

        return threeRealRoots;
    }

}

#endif
