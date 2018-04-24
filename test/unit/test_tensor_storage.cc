#include "gtest/gtest.h"
#include "tensor.h"

/*
 * Unit tests of the data storage in the tensor class, implemented in TENSOR.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(TensorStorage, DefaultTensorStoredAlong0_1) {
  Tensor t;
  vector<int> s = t.getStorage();
  ASSERT_EQ(2, s.size());
  EXPECT_EQ(0, s[0]);
  EXPECT_EQ(1, s[1]);
}

TEST(TensorStorage, ChangeStorageTo1_0) {
  Tensor t;
  t.setStorage({1,0});
  vector<int> s = t.getStorage();
  ASSERT_EQ(2, s.size());
  EXPECT_EQ(1, s[0]);
  EXPECT_EQ(0, s[1]);
}

TEST(TensorStorage, CreateTensorWithSpecificStorage) {
  Tensor t(3,64,{2,0,1});
  EXPECT_EQ(3, t.rank());
  EXPECT_EQ(64, t.size());
  vector<int> s = t.getStorage();
  ASSERT_EQ(3, s.size());
  EXPECT_EQ(2, s[0]);
  EXPECT_EQ(0, s[1]);
  EXPECT_EQ(1, s[2]);
}

TEST(TensorStorage, CreateTensorWithDefaultStorage) {
  Tensor t(3,64);
  EXPECT_EQ(3, t.rank());
  EXPECT_EQ(64, t.size());
  vector<int> s = t.getStorage();
  ASSERT_EQ(3, s.size());
  EXPECT_EQ(0, s[0]);
  EXPECT_EQ(1, s[1]);
  EXPECT_EQ(2, s[2]);
}

TEST(TensorStorage, ChangeStorageTo2_1_0GetSliceX_X_0) {
  Tensor t(3,2); // Default storage (0,1,2)
  cdouble data[4];
  data[0] = 0.0; data[1] = 1.0; data[2] = 2.0; data[3] = 3.0;
  t.setSlice({-1,-1,0}, data);
  data[0] = 4.0; data[1] = 5.0; data[2] = 6.0; data[3] = 7.0;
  t.setSlice({-1,-1,1}, data);
  t.setStorage({2,1,0});
  t.getSlice({-1,-1,0}, data);
  EXPECT_EQ(0.0, data[0]);
  EXPECT_EQ(1.0, data[1]);
  EXPECT_EQ(2.0, data[2]);
  EXPECT_EQ(3.0, data[3]);
}

TEST(TensorStorage, ChangeStorageTo2_1_0SetSliceChangeTo1_2_0GetSlice) {
  Tensor t(3,2,{2,1,0});
  cdouble data[4];
  data[0] = 0.0; data[1] = 1.0; data[2] = 2.0; data[3] = 3.0;
  t.setSlice({-1,-1,0}, data);
  data[0] = 4.0; data[1] = 5.0; data[2] = 6.0; data[3] = 7.0;
  t.setSlice({-1,-1,1}, data);
  t.setStorage({1,2,0});
  t.getSlice({-1,-1,0}, data);
  EXPECT_EQ(0.0, data[0]);
  EXPECT_EQ(1.0, data[1]);
  EXPECT_EQ(2.0, data[2]);
  EXPECT_EQ(3.0, data[3]);
}



}