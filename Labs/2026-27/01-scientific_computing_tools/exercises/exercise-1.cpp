#include <Eigen/Dense>
#include <iostream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;


int main() {

  MatrixXd tri = MatrixXd::Zero(100, 100);
  tri.diagonal().setConstant(2);
  tri.diagonal(-1).setConstant(-1);
  tri.diagonal(1).setConstant(1);
  cout << "tri = " << endl << tri << endl;
  auto product = tri * tri.transpose();
  double induced_norm_2 = sqrt(product.eigenvalues().real().maxCoeff());
  cout << "induced 2-norm = " << induced_norm_2 << endl;

  cout << "induced 2-norm of tri = "
       << tri.jacobiSvd().singularValues().maxCoeff() << endl;


  auto v = VectorXd::Constant(50, 1);
  cout << "v = " << endl << v << endl;

  auto matrix_vector_product = tri.bottomRightCorner(50, 50) * v;
  cout << "Matrix-vector product = " << endl
       << matrix_vector_product << endl;

  auto scalar_product = v.dot(matrix_vector_product);
  cout << "Scalar product = " << scalar_product << endl;
  return 0;
}