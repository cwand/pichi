#include "gtest/gtest.h"
#include "pichi/tensor.h"

/*
 * Unit tests of the data storage in the tensor class, implemented in TENSOR.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(TensorStorage, DefaultTensorStoredAlong0_1) {
  Tensor t;
  vector<int> s = t.getStorage();
  ASSERT_EQ(0, s.size());
}

TEST(TensorStorage, CreateTensorWithSpecificStorage) {
  Tensor t(3,64,{2,0,1});
  EXPECT_EQ(3, t.getRank());
  EXPECT_EQ(64, t.getSize());
  vector<int> s = t.getStorage();
  ASSERT_EQ(3, s.size());
  EXPECT_EQ(2, s[0]);
  EXPECT_EQ(0, s[1]);
  EXPECT_EQ(1, s[2]);
}

TEST(TensorStorage, CreateTensorWithDefaultStorage) {
  Tensor t(3,64);
  EXPECT_EQ(3, t.getRank());
  EXPECT_EQ(64, t.getSize());
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

TEST(TensorStorage, ErrorOnStorageVectorWrongSize) {
  EXPECT_THROW(Tensor(3,64,{1,0}), invalid_argument);

  Tensor t(3,64);
  EXPECT_THROW(t.setStorage({1,0}), invalid_argument);
}

TEST(TensorStorage, ErrorOnRepeatedIndex) {
  EXPECT_THROW(Tensor(3,64,{1,1,0}), invalid_argument);

  Tensor t(3,64);
  EXPECT_THROW(t.setStorage({1,1,0}), invalid_argument);
}

TEST(TensorStorage, ErrorOnInvalidIndex) {
  EXPECT_THROW(Tensor(3,64,{1,-1,0}), invalid_argument);

  Tensor t(3,64);
  EXPECT_THROW(t.setStorage({1,3,0}), invalid_argument);
}


TEST(TensorResize, ResizeRank3TensorToRank2) {
  Tensor t(3,2,{2,1,0});
  cdouble data[4] = {1.0,2.0,3.0,4.0};
  t.setSlice({-1,-1,0}, data);

  t.resize(2,3);
  ASSERT_EQ(2, t.getRank());
  ASSERT_EQ(3, t.getSize());
  ASSERT_EQ(0, t.getStorage()[0]);
  ASSERT_EQ(1, t.getStorage()[1]);

  cdouble data2[9] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0};
  t.getSlice({-1,-1},data2);
  for (auto i : data2)
    EXPECT_EQ(0.0, i);


}

TEST(TensorResize, ResizeRank2TensorToRank3WithStorage) {
  Tensor t(2,3);
  cdouble data[9] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0};
  t.setSlice({-1,-1}, data);

  t.resize(3,2,{2,0,1});
  ASSERT_EQ(3, t.getRank());
  ASSERT_EQ(2, t.getSize());
  ASSERT_EQ(2, t.getStorage()[0]);
  ASSERT_EQ(0, t.getStorage()[1]);
  ASSERT_EQ(1, t.getStorage()[2]);


  cdouble data2[4] = {1.0,2.0,3.0,4.0};
  t.getSlice({-1,-1,0},data2);
  for (auto i : data2)
    EXPECT_EQ(0.0, i);
  cdouble data3[4] = {1.0,2.0,3.0,4.0};
  t.getSlice({-1,-1,1},data3);
  for (auto i : data3)
    EXPECT_EQ(0.0, i);


}


}
