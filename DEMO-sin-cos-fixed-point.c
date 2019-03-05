
#define NO_TESTCASE_ROTATE_FIXED_POINT

#include "CORDIC-rotate-fixed-point.c" /* uglily includes other source file here */

double
cos_fixed_point(double angle)
{
  double x = .0, y = 1.0;
  rotate_fixed_point(&x, &y, -angle);
  return y; /* sin(90deg - angle) = cos(angle) */
}

double
sin_fixed_point(double angle)
{
  double x = .0, y = 1.0;
  rotate_fixed_point(&x, &y, -angle);
  return x; /* cos(90deg - angle) = sin(angle) */
}

#ifndef NO_TESTCASE_SIN_COS_FIXED_POINT
int
main(void)
{
  printf("cos 45deg = %lf\n", cos_fixed_point(45.0));
  printf("sin 30deg = %lf\n", sin_fixed_point(30.0));
}
#endif
