//
//  Tensor.hpp
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

#pragma once

#include <vector>

template <class T>
class Tensor {
public:
  /*
   * Empty constructor initialized empty tensor
   */
  Tensor() : begin(nullptr), end(nullptr), value(nullptr), isSubtensor(false)
  {
    // do nothing
  };
  
  /*
   * Constructor with dimensions sizes
   * data is initialized by default T()
   */
  Tensor(const std::vector<int> &dimSizes) : begin(nullptr), end(nullptr), dimSizes(dimSizes),
    value(nullptr), isSubtensor(false)
  {
    if (dimSizes.size() == 0) {
      value = new T();
    } else {
      // create sub-tensor
      std::vector<int> tail(dimSizes.begin() + 1, dimSizes.end());
      begin = new Tensor<T>[dimSizes[0]];
      end = begin + dimSizes[0];
      for (int i = 0; i < dimSizes[0]; i++) {
        begin[i] = Tensor(tail);
      }
      
    }
  }
  /*
   * Copy -constructor, should copy entire tensor data
   */
  Tensor(const Tensor &rhs)
  {
    *this = rhs;
  }
  /*
   * Move -constructor
   */
  Tensor(Tensor &&rhs)
  {
    begin = rhs.begin;
    end = rhs.end;
    rhs.begin = nullptr;
    rhs.end = nullptr;
    
    value = rhs.value;
    rhs.value = nullptr;
    
    isSubtensor = rhs.isSubtensor;
    
    // does move
    dimSizes = std::move(rhs.dimSizes);
  }
  
  /*
   * Copy -assignment , should copy entire tensor data
   */
  Tensor &operator=(const Tensor &rhs)
  {
    // does copy
    dimSizes = rhs.dimSizes;
    
    isSubtensor = rhs.isSubtensor;
    
    if (rhs.value) { // scalar
      value = new T();
      memcpy(value, rhs.value, sizeof(T));
      begin = nullptr;
      end = nullptr;
    } else if (rhs.begin) { // dim > 0, make a deep copy
      // allocate array
      begin = new Tensor<T>[dimSizes[0]];
      end = begin + dimSizes[0];
      // call copy recursively
      for (int i = 0; i < dimSizes[0]; i++) {
        begin[i] = rhs.begin[i];
      }
      value = nullptr;
    }
    
    return *this;
  }
  
  /*
   * Move -assignment
   */
  Tensor &operator=(Tensor &&rhs)
  {
    std::swap(begin, rhs.begin);
    rhs.begin = nullptr;
    std::swap(end, rhs.end);
    rhs.end = nullptr;
    std::swap(value, rhs.value);
    rhs.value = nullptr;
    std::swap(dimSizes, rhs.dimSizes);
    
    isSubtensor = rhs.isSubtensor;
    
    return *this;
  }
  
  ~Tensor()
  {
    std::cout << "destroy " << std::hex << this << std::endl;
    if (begin && !isSubtensor) {
      delete [] begin;
      begin = nullptr;
      end = nullptr;
    }
    
    if (value) {
      delete value;
      value = nullptr;
    }
  }

  /*
   * Returns dimensions sizes of tensor
   */
  std::vector<int> dim() const
  {
    return dimSizes;
  }
  
  /*
   * Returns sub -tensor with indices in first dimension [low ,
   high)
   * !!! All changes applied further to sub -tensor would be
   applied to this tensor as well and vice versa as well ,
   in other words they would share their content!!!
   */
  Tensor operator()(int low , int high) {
    assert(low >= 0 && high >= 0 && value == nullptr);
    
    if (low == high) {
      return this->operator()(low);
    }
    
    std::vector<int> subDimSizes(dimSizes);
    subDimSizes[0] = high - low;
    return Tensor<T>(begin + low, begin + high, subDimSizes);
  }
  
  /*
   * Returns sub -tensor with index in first dimension pos
   * !!! All changes applied further to sub -tensor would be
   applied to this tensor as well and vice versa as well ,
   in other words they would share their content!!!
   */
  Tensor operator()(int pos) {
    assert(pos >= 0 && value == nullptr);
    assert(pos < end - begin);
    // return subtensor
    return begin[pos];
  }
  /*
   * Returns value indexed by {pos[0], pos[1], pos[2]... pos[
   dim - 1]}
   * pos.size() should be equal to dim
   */
  T& operator [](std::vector <int> pos) {
    // pos.size() should be equal to dim
    assert(pos.size() == dimSizes.size());
    
    if (value) {
      return *value;
    } else { // dim > 1
      Tensor subTensor = (*this)(pos[0]);
      return subTensor[std::vector<int>(pos.begin() + 1, pos.end())];
    }
  }
  
private:
  Tensor(Tensor<T> *begin, Tensor<T> *end, const std::vector<int> &dimSizes) : begin(begin), end(end)
  {
    isSubtensor = true;
    this->dimSizes = std::move(dimSizes);
  }
  
  std::vector<int> dimSizes;
  // tensor data
  Tensor<T> *begin;
  Tensor<T> *end;
  
  T *value;
  bool isSubtensor;
};
