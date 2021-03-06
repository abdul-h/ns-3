// -*- c++ -*-

#include "ns3/core-module.h"

using namespace ns3;

namespace 
{

    static inline Ptr<Object>
    __dummy_function_to_force_template_instantiation (Ptr<Object> obj, TypeId typeId)
    {
        return obj->GetObject<Object> (typeId);
    }


    static inline void
    __dummy_function_to_force_template_instantiation_v2 ()
    {
        Time t1, t2, t3;
        t1 = t2 + t3;
        t1 = t2 - t3;
        TimeSquare tsq = t2*t3;
        Time tsqdiv = tsq/Seconds(1);
        Scalar scal = t2/t3;
        TimeInvert inv = scal/t3;
        t1 = scal*t1;
        t1 = t1/scal;
        t1 < t2;
        t1 <= t2;
        t1 == t2;
        t1 != t2;
        t1 >= t2;
        t1 > t2;
    }
    
}

