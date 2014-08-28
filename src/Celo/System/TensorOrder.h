#ifndef TENSORORDER_H
#define TENSORORDER_H

namespace Celo {

/**
  @brief To get the static dimensionnality of objects

  Meaning of \c res
   - 0 -> scalar
   - 1 -> vector
   - 2 -> matrix
   - ...
   - -1 -> dynamic tensor order (not fixed during the compilation)
*/
template<class T> struct TensorOrder { static const int res = 0; };

}

#endif // TENSORORDER_H
