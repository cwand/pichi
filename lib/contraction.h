#ifndef PICHI_CONTRACTION_H
#define PICHI_CONTRACTION_H

#include "tensor.h"
#include <unordered_map>
#include <queue>

namespace pichi {

/* ************************************************************************
 *
 * This file declares the Contraction class.
 *
 * A contraction is a unary or binary tensor operation, in which one or more
 * indices on the input tensors are contracted in order to create a single
 * output tensor which has fewer combined indices. An simple example is
 * matrix-matrix multiplication, which is the contraction of a single index
 * on two rank two tensors, creating a new rank two tensor. Another example
 * is the trace operation, which is a unary operation on a rank 2 tensor.
 *
 * To contract tensors using this class, they have to be added to the
 * collection with a name. After this there are three contraction
 * operations one can perform:
 *
 * 1) Simply tracing a single rank 2 tensor and getting the result returned.
 * 2) Contracting all indices on two tensors in the collection, also
 * resulting in an output number.
 * 3) Contracting some indices on two tensors in the collection, creating a
 * new tensor from the result. The result is added to the collection, so it
 * can be used for more contractions later. This gives no output.
 *
 * The input tensors in the collection can be reused after a contraction in
 * all three cases. They are not modified (other than a possible internal
 * restructure of the data, which wont affect the actual tensor or
 * contractions).
 *
 * We tell the class which indices are contracted in a list of pairs of
 * integers. If we have in our collection a rank 3 tensor 'A' and a rank 5
 * tensor 'B', then the list {{0,1},{2,4}} means that we want to contract the
 * first index of A with the second index of B, as well as the third index of
 * A with the fifth index of B. This corresponds to the contraction
 *
 *    A_abc B_daefc
 *
 * The result of this contraction is a rank 4 tensor, which is then inserted
 * into the collection. The same structure is used when contracting tensors
 * to numbers.
 *
 * ***********************************************************************/

class Contraction {

public:

  /*
   * Add a tensor to the collection with a given name.
   */
  void addTensor(char name, Tensor& tensor);

  /*
   * Trace a matrix. The tensor in question needs to be rank 2 for this to work.
   */
  void contract(char tensor);

  /*
   * Contract all indices on two tensors of equal rank. The resulting number
   * is returned in the end.
   * The input list of contracted indices must be the same length as the
   * tensor ranks.
   */
  void contract(char tensor1, char tensor2,
                std::vector<std::pair<int, int>> idx);

  /*
   * Contract a number of indices on two tensors in the collection. The
   * resulting tensor is added to the collection with a given name.
   */
  void contract(char tensor1, char tensor2,
                std::vector<std::pair<int,int>> idx, char tensor_out);

  cdouble getResult() const { return result; }

private:

  // The collection of tensors
  std::unordered_map<char, Tensor> tensors;

  // Detect whether a slice needs to be transposed before matrix multiplication.
  int detectTranspose(const std::vector<int>& slice1,
                      const std::vector<int>& slice2);

  cdouble result;

};

}

#endif //PICHI_CONTRACTION_H
