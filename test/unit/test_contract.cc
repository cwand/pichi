#include "pichi/contraction.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the contraction logic defined in CONTRACTION.CC
 */

using namespace pichi;
using namespace std;

namespace {


TEST(Contract, NoneSingleTensor) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0; data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t;
  contract(t1,{},t);
  EXPECT_EQ(2, t.getRank());
  EXPECT_EQ(2, t.getSize());
  cdouble res_data[4]; t.getSlice({-1,-1},res_data);
  EXPECT_EQ(3.0, res_data[0]); EXPECT_EQ(3.0, res_data[1]);
  EXPECT_EQ(3.0, res_data[2]); EXPECT_EQ(-1.0, res_data[3]);
}

TEST(Contract, aa) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0; data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t;
  contract(t1,{{0,1}},t);
  cdouble res_data[1]; t.getSlice({0},res_data);
  EXPECT_EQ(2.0, res_data[0]);
}

TEST(Contract, aabb) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0},data);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = 0.0;
  t1.setSlice({-1,-1,1,0},data);
  data[0] = 3.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = 2.0;
  t1.setSlice({-1,-1,0,1},data);
  data[0] = 1.0; data[1] = 2.0;
  data[2] = 3.0; data[3] = -4.0;
  t1.setSlice({-1,-1,1,1},data);

  Tensor t;
  contract(t1,{{0,1},{2,3}},t);
  cdouble rdata[1]; t.getSlice({0}, rdata);

  EXPECT_EQ(-1.0, rdata[0]);
}

TEST(Contract, aabc) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0},data);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = 0.0;
  t1.setSlice({-1,-1,1,0},data);
  data[0] = 3.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = 2.0;
  t1.setSlice({-1,-1,0,1},data);
  data[0] = 1.0; data[1] = 2.0;
  data[2] = 3.0; data[3] = -4.0;
  t1.setSlice({-1,-1,1,1},data);

  Tensor t3;
  contract(t1,{{0,1}},t3);

  ASSERT_EQ(2,t3.getRank());
  ASSERT_EQ(2,t3.getSize());

  t3.getSlice({-1,-1}, data);

  EXPECT_EQ(2.0, data[0]);
  EXPECT_EQ(2.0, data[1]);
  EXPECT_EQ(5.0, data[2]);
  EXPECT_EQ(-3.0, data[3]);

}

TEST(Contract, abcd_ab) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0},data);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = 0.0;
  t1.setSlice({-1,-1,1,0},data);
  data[0] = 3.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = 2.0;
  t1.setSlice({-1,-1,0,1},data);
  data[0] = 1.0; data[1] = 2.0;
  data[2] = 3.0; data[3] = -4.0;
  t1.setSlice({-1,-1,1,1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t4;
  contract(t1,t2,{{0,0},{1,1}},t4);

  ASSERT_EQ(2,t4.getRank());
  ASSERT_EQ(2,t4.getSize());

  t4.getSlice({-1,-1}, data);

  EXPECT_EQ(20.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]),1.0e-20);

  EXPECT_EQ(13.0, real(data[1]));
  EXPECT_NEAR(0.0, imag(data[1]),1.0e-20);

  EXPECT_EQ(17.0, real(data[2]));
  EXPECT_NEAR(0.0, imag(data[1]),1.0e-20);

  EXPECT_EQ(33.0, real(data[3]));
  EXPECT_NEAR(0.0, imag(data[1]),1.0e-20);
}

TEST(Contract, ab_ba) {
  Tensor t1(2,3);
  cdouble data[9];
  data[0] = 2.0; data[1] = 3.0; data[2] = 3.0;
  data[3] = -1.0; data[4] = 2.0; data[5] = -4.0;
  data[6] = 2.0; data[7] = -5.0; data[8] = -2.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,3);
  data[0] = 2.0; data[1] = -2.0; data[2] = 5.0;
  data[3] = -5.0; data[4] = -1.0; data[5] = -1.0;
  data[6] = 3.0; data[7] = 3.0; data[8] = -3.0;
  t2.setSlice({-1,-1},data);

  Tensor tr;
  contract(t1,t2,{{0,1},{1,0}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(7.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]), 1.0e-20);
}

TEST(Contract, ab_ab) {
  Tensor t1(2,3);
  cdouble data[9];
  data[0] = 2.0; data[1] = 3.0; data[2] = 3.0;
  data[3] = -1.0; data[4] = 2.0; data[5] = -4.0;
  data[6] = 2.0; data[7] = -5.0; data[8] = -2.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,3);
  data[0] = 2.0; data[1] = -2.0; data[2] = 5.0;
  data[3] = -5.0; data[4] = -1.0; data[5] = -1.0;
  data[6] = 3.0; data[7] = 3.0; data[8] = -3.0;
  t2.setSlice({-1,-1},data);

  Tensor tr;
  contract(t1,t2,{{0,0},{1,1}},tr);

  ASSERT_EQ(0,tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(17.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]), 1.0e-20);
}

TEST(Contract, ab_ab_Tensor1Transposed) {
  Tensor t1(2,3,{1,0});
  cdouble data[9];
  data[0] = 2.0; data[1] = 3.0; data[2] = 3.0;
  data[3] = -1.0; data[4] = 2.0; data[5] = -4.0;
  data[6] = 2.0; data[7] = -5.0; data[8] = -2.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,3);
  data[0] = 2.0; data[1] = -2.0; data[2] = 5.0;
  data[3] = -5.0; data[4] = -1.0; data[5] = -1.0;
  data[6] = 3.0; data[7] = 3.0; data[8] = -3.0;
  t2.setSlice({-1,-1},data);

  Tensor tr;
  contract(t1,t2,{{0,0},{1,1}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(17.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]), 1.0e-20);
}

TEST(Contract, ab_ab_Tensor2Transposed) {
  Tensor t1(2,3);
  cdouble data[9];
  data[0] = 2.0; data[1] = 3.0; data[2] = 3.0;
  data[3] = -1.0; data[4] = 2.0; data[5] = -4.0;
  data[6] = 2.0; data[7] = -5.0; data[8] = -2.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,3,{1,0});
  data[0] = 2.0; data[1] = -2.0; data[2] = 5.0;
  data[3] = -5.0; data[4] = -1.0; data[5] = -1.0;
  data[6] = 3.0; data[7] = 3.0; data[8] = -3.0;
  t2.setSlice({-1,-1},data);

  Tensor tr;
  contract(t1,t2,{{0,0},{1,1}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(17.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]), 1.0e-20);
}

TEST(Contract, ab_ab_BothTransposed) {
  Tensor t1(2,3,{1,0});
  cdouble data[9];
  data[0] = 2.0; data[1] = 3.0; data[2] = 3.0;
  data[3] = -1.0; data[4] = 2.0; data[5] = -4.0;
  data[6] = 2.0; data[7] = -5.0; data[8] = -2.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,3,{1,0});
  data[0] = 2.0; data[1] = -2.0; data[2] = 5.0;
  data[3] = -5.0; data[4] = -1.0; data[5] = -1.0;
  data[6] = 3.0; data[7] = 3.0; data[8] = -3.0;
  t2.setSlice({-1,-1},data);

  Tensor tr;
  contract(t1,t2,{{0,0},{1,1}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(17.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]), 1.0e-20);
}

TEST(Contract, ab_bc_ca) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  Tensor t4,tr;
  contract(t1,t2,{{1,0}},t4);
  contract(t4,t3,{{0,1},{1,0}},tr);

  ASSERT_EQ(0,tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(70.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]), 1.0e-20);
}

TEST(Contract, ab_bc_Tensor1Transposed) {
  Tensor t1(2,2,{1,0});
  cdouble data[4];
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3;
  contract(t1,t2,{{1,0}},t3);

  ASSERT_EQ(2, t3.getRank());
  ASSERT_EQ(2, t3.getSize());

  t3.getSlice({-1,-1},data);

  EXPECT_EQ(-4.0, data[0]);
  EXPECT_EQ(-6.0, data[1]);
  EXPECT_EQ(-10.0, data[2]);
  EXPECT_EQ(-15.0, data[3]);
}

TEST(Contract, ab_bc_Tensor2Transposed) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2,{1,0});
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3;
  contract(t1,t2,{{1,0}},t3);

  ASSERT_EQ(2, t3.getRank());
  ASSERT_EQ(2, t3.getSize());

  t3.getSlice({-1,-1},data);

  EXPECT_EQ(-4.0, data[0]);
  EXPECT_EQ(-6.0, data[1]);
  EXPECT_EQ(-10.0, data[2]);
  EXPECT_EQ(-15.0, data[3]);
}

TEST(Contract, ab_bc_BothTransposed) {
  Tensor t1(2,2,{1,0});
  cdouble data[4];
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2,{1,0});
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3;
  contract(t1,t2,{{1,0}},t3);

  ASSERT_EQ(2, t3.getRank());
  ASSERT_EQ(2, t3.getSize());

  t3.getSlice({-1,-1},data);

  EXPECT_EQ(-4.0, data[0]);
  EXPECT_EQ(-6.0, data[1]);
  EXPECT_EQ(-10.0, data[2]);
  EXPECT_EQ(-15.0, data[3]);
}

TEST(Contract, ab_cb_ac) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  Tensor t4,tr;
  contract(t1,t2,{{1,1}},t4);
  contract(t4,t3,{{0,0},{1,1}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(-41.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]), 1.0e-20);
}

TEST(Contract, ab_ac_cb) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  Tensor t4,tr;
  contract(t1,t2,{{0,0}},t4);
  contract(t4,t3,{{0,1},{1,0}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(70.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]), 1.0e-20);

}

TEST(Contract, ab_ca_cb) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  Tensor t4,tr;
  contract(t1,t2,{{0,1}},t4);
  contract(t4,t3,{{0,1},{1,0}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(-21.0, real(data[0]));
  EXPECT_NEAR(0.0, imag(data[0]), 1.0e-20);
}

TEST(Contract, abc_bad_dc) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1}, data);

  Tensor t4,tr;
  contract(t1,t2,{{0,1},{1,0}},t4);
  contract(t4,t3,{{0,1},{1,0}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(-28.0, data[0].real());
  EXPECT_NEAR(0.0, data[0].imag(), 1.0e-20);

}

TEST(Contract, abc_abd_cd) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1}, data);

  Tensor t4,tr;
  contract(t1,t2,{{0,0},{1,1}},t4);
  contract(t4,t3,{{0,0},{1,1}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(-48.0, data[0].real());
  EXPECT_NEAR(0.0, data[0].imag(), 1.0e-20);

}

TEST(Contract, abc_abd_cd2) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1}, data);

  Tensor t4,tr;
  contract(t1,t2,{{1,1},{0,0}},t4);
  contract(t4,t3,{{0,0},{1,1}},tr);

  ASSERT_EQ(0, tr.getRank());

  tr.getSlice({0},data);

  EXPECT_EQ(-48.0, data[0].real());
  EXPECT_NEAR(0.0, data[0].imag(), 1.0e-20);

}

TEST(Contract, abcd_dcbe_ea) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1,0}, data);
  data[0] = 1.0; data[1] = -1.0;
  data[2] = 1.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,1}, data);
  data[0] = -2.0; data[1] = 2.0;
  data[2] = -5.0;  data[3] = 3.0;
  t1.setSlice({-1,-1,1,1}, data);

  Tensor t2(4,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1,0}, data);
  data[0] = -2.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -2.0;
  t2.setSlice({-1,-1,0,1}, data);
  data[0] = -1.0; data[1] = -1.0;
  data[2] = 3.0; data[3] = -3.0;
  t2.setSlice({-1,-1,1,1}, data);

  Tensor t3(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1}, data);

  Tensor t4,t5;
  contract(t1,t2,{{1,2},{2,1},{3,0}},t4);
  contract(t4,t3, {{0,1},{1,0}},t5);

  ASSERT_EQ(0, t5.getRank());

  cdouble datar[1];
  t5.getSlice({0},datar);
  EXPECT_EQ(-68.0, datar[0].real());
  EXPECT_NEAR(0.0, data[0].imag(), 1.0e-20);

}

TEST(Contract, abc_cba) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3;
  contract(t1,t2,{{0,2},{1,1},{2,0}},t3);

  ASSERT_EQ(0, t3.getRank());

  cdouble datar[1];
  t3.getSlice({0},datar);

  EXPECT_EQ(16.0, datar[0].real());
  EXPECT_NEAR(0.0, datar[0].imag(), 1.0e-20);

}

TEST(Contract, abc_cd_dba) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t2.setSlice({-1,-1}, data);

  Tensor t3(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t3.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t3.setSlice({-1,-1,1}, data);

  Tensor t4,t5;
  contract(t1,t2,{{2,0}},t4);
  contract(t4,t3,{{0,2},{1,1},{2,0}},t5);

  ASSERT_EQ(0, t5.getRank());

  cdouble datar[1];
  t5.getSlice({0}, datar);

  EXPECT_EQ(-75.0, datar[0].real());
  EXPECT_NEAR(0.0, datar[0].imag(), 1.0e-20);

}

TEST(Contract, abcd_dce_eba) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1,0}, data);
  data[0] = 1.0; data[1] = -1.0;
  data[2] = 1.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,1}, data);
  data[0] = -2.0; data[1] = 2.0;
  data[2] = -5.0;  data[3] = 3.0;
  t1.setSlice({-1,-1,1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3(3,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -2.0;
  t3.setSlice({-1,-1,1}, data);

  Tensor t4,t5;
  contract(t1,t2,{{2,1},{3,0}},t4);
  contract(t4,t3,{{0,2},{1,1},{2,0}},t5);

  ASSERT_EQ(0, t5.getRank());

  cdouble datar[1];
  t5.getSlice({0}, datar);

  EXPECT_EQ(17.0, datar[0].real());
  EXPECT_NEAR(0.0, datar[0].imag(), 1.0e-20);

}

TEST(Contract, aabc_bc) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1,0}, data);
  data[0] = 1.0; data[1] = -1.0;
  data[2] = 1.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,1}, data);
  data[0] = -2.0; data[1] = 2.0;
  data[2] = -5.0;  data[3] = 3.0;
  t1.setSlice({-1,-1,1,1}, data);


  Tensor t3(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1}, data);

  Tensor t4,t5;
  contract(t1,{{0,1}},t4);
  contract(t4,t3, {{0,0},{1,1}},t5);

  ASSERT_EQ(0, t5.getRank());

  cdouble datar[1];
  t5.getSlice({0},datar);
  EXPECT_EQ(3.0, datar[0]);

}


}
