#include <stan/math/prim/mat/meta/get.hpp>
#include <stan/math/prim/arr/meta/get.hpp>
#include <stan/math/prim/mat/meta/length.hpp>
#include <stan/math/prim/mat/meta/is_vector.hpp>
#include <stan/math/prim/mat/meta/is_vector_like.hpp>
#include <stan/math/prim/mat/fun/append_row.hpp>
#include <gtest/gtest.h>
#include <test/unit/math/rev/mat/fun/util.hpp>
#include <stan/math/prim/mat/fun/sum.hpp>
#include <stan/math/rev/mat/fun/sum.hpp>
#include <stan/math/rev/scal/fun/exp.hpp>
#include <stan/math/rev/mat/fun/typedefs.hpp>

using stan::math::sum;
using stan::math::append_row;
using stan::agrad::matrix_v;
using stan::agrad::vector_v;
using Eigen::MatrixXd;
using Eigen::VectorXd;

TEST(AgradRevMatrix, append_row_matrix) {
  matrix_v a(2,2);
  matrix_v a_exp(2,2);
  MatrixXd b(2,2);
  
  a << 2.0, 3.0,
       9.0, -1.0;
       
  b << 4.0, 3.0,
       0.0, 1.0;

  AVEC x;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      x.push_back(a(i,j));
      a_exp(i, j) = stan::agrad::exp(a(i, j));
    }
  }
  
  AVAR append_row_ab = sum(append_row(a_exp, b));

  VEC g = cgradvec(append_row_ab, x);
  
  size_t idx = 0;
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      EXPECT_FLOAT_EQ(std::exp(a(i, j).val()), g[idx++]);
}

TEST(AgradRevMatrix, append_row_row_vector) {
  vector_v a(3);
  vector_v a_exp(3);
  VectorXd b(3);
  
  a << 2.0, 3.0, 9.0;
       
  b << 4.0, 3.0, 0.0;

  AVEC x;
  for (int i = 0; i < 3; ++i) {
    x.push_back(a(i));
    a_exp(i) = stan::agrad::exp(a(i));
  }
  
  AVAR append_row_ab = sum(append_row(a_exp, b));

  VEC g = cgradvec(append_row_ab, x);
  
  size_t idx = 0;
  for (int i = 0; i < 3; i++)
    EXPECT_FLOAT_EQ(std::exp(a(i).val()), g[idx++]);
}