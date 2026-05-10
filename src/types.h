#ifndef METODE_NEWTON_CONT_TYPES_H
#define METODE_NEWTON_CONT_TYPES_H

#include <adept_arrays.h>

template<int N>
using aVectorN = adept::FixedArray<double, true, N>;

template<int N>
using VectorN = adept::FixedArray<double, false, N>;



#endif // METODE_NEWTON_CONT_TYPES_H
