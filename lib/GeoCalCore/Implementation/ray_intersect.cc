#include "ray_intersect.h"
#include "geocal_matrix.h"
using namespace GeoCal;

void RayIntersect::two_look_vector_intersect
(const CartesianFixed& P1,
 const CartesianFixedLookVector& L1,
 const CartesianFixed& P2,
 const CartesianFixedLookVector& L2,
 boost::shared_ptr<CartesianFixed>& P, double& Dist)
{
  // Given a line 1 as P1 + t1 * u1 for scalar t1 and a second line
  // 2 as P2 + t2 * u2, then the distance between the closest points 
  // on the lines is given by (P21) . (u1 x u2 ) / | u1 x u2 |
  // (where P21 is P2 - P1).
  //
  // We want to solve
  //
  // q2 - q1 = p2 + t2 * u2 - p1 - t1 * u1 = k * (u2 x u1)
  //
  // With the variables:
  //  p21 = p2 - p1
  //  m = u2 x u1
  //  m2 = dot(m, m)
  //  r = p21 x m / m2
  //
  // Then we have the solution:
  //  t1 = dot(r, u2)
  //  t2 = dot(r, u1)
  //
  // If we want the point closest to both lines, then we can average
  // q1 and q2 (this gives the point half way between each).
 
  boost::array<double, 3> u1(L1.direction());
  boost::array<double, 3> u2(L2.direction());
  boost::array<double, 3> m;
  cross(u2, u1, m);
  double m2 = dot(m, m);
  boost::array<double, 3> p21;
  p21[0] = P2.position[0] - P1.position[0];
  p21[1] = P2.position[1] - P1.position[1];
  p21[2] = P2.position[2] - P1.position[2];
  boost::array<double, 3> r;
  cross(p21, m, r);
  r[0] /= m2;
  r[1] /= m2;
  r[2] /= m2;
  double t1 = dot(r, u2);
  double t2 = dot(r, u1);
  boost::array<double, 3> qavg;
  qavg[0] = ((P1.position[0] + t1 * u1[0]) + (P2.position[0] + t2 * u2[0])) / 2;
  qavg[1] = ((P1.position[1] + t1 * u1[1]) + (P2.position[1] + t2 * u2[1])) / 2;
  qavg[2] = ((P1.position[2] + t1 * u1[2]) + (P2.position[2] + t2 * u2[2])) / 2;
  P = P1.create(qavg);
  // Divide by 2 to give distance of qavg to q1 and q2.
  Dist = fabs(dot(p21, m) / sqrt(m2)) / 2;
}

