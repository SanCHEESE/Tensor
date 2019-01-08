//
//  main.cpp
//  Tensor
//  A minimal template implementation of n-dimentional array.
//
//  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//  Version 2, December 2004
//
//  Copyright (C) 2018 Alexander Bochkarev  <otnasa@gmail.com>
//
//  Everyone is permitted to copy and distribute verbatim or modified
//  copies of this license document, and changing it is allowed as long
//  as the name is changed.
//
//  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.
//

#include <iostream>
#include "Tensor.hpp"

int main(int argc, const char * argv[]) {

  Tensor<float> t1 = Tensor<float>({5, 5, 5}); // create new tensor with shape 5x5x5
  Tensor<float> t2 = t1(3, 4); // shape 1x5x5, shares data with t1
  Tensor<float> t3 = t2; // shape 1x5x5, doesn’t share any data with t1 and t2 as copied
  Tensor<float> t4; // empty tensor
  t4 = t1(3); // shape 5x5, shares data with t1 and t2, move semantics here
  Tensor<float> t5 = t1(3)(4)(2, 5); // shape 3, shares data with t1, t2, t4
  t5[{0}] = 100.0; // t1[{3, 4, 3}], t2[{0, 4, 3}], t4[{4, 3}] are 100.0 as well, but t3[{0, 4, 3}] is 0.0

  float t1_343 = t1[{3, 4, 3}];
  float t2_043 = t2[{0, 4, 3}];
  assert(t1_343 == t2_043);

  t1[{3, 4, 4}] = -100.0; // t2[{0, 4, 4}], t4[{4, 4}], t5[{2}] are -100.0 as well, but t3[{0, 4, 4}] is 0.0

  float t2_044 = t2[{0, 4, 4}];
  float t4_44 = t4[{4, 4}];
  assert(t2_044 == t4_44);
  
  std::cout << "all works fine" << std::endl;

  return 0;
}
