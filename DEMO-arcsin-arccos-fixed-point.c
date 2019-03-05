#include <math.h>

#define NO_TESTCASE_ANTI_ROTATE_FIXED_POINT

#include "CORDIC-anti-rotate-fixed-point.c" /* uglily includes other source file here */

#define P 0.607253

double
arccos_fixed_point(double val)
{
  double x = .0, y = P;
  return anti_rotate_y_fixed_point(&x, &y, val, 0/* ignore P */);
}

double
arcsin_fixed_point(double val)
{
  double x = .0, y = P;
  return anti_rotate_x_fixed_point(&x, &y, val, 0/* ignore P */);
}

#ifndef NO_TESTCASE_ARCSIN_ARCCOS_FIXED_POINT
int
main(void)
{
  printf("arccos 0.5 = %lf\n", arccos_fixed_point(0.5));
  printf("arcsin 0.5 = %lf\n", arcsin_fixed_point(0.5));
}
#endif
