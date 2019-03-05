#include <math.h>

#define NO_TESTCASE_ANTI_ROTATE_FIXED_POINT

#include "CORDIC-anti-rotate-fixed-point.c" /* uglily includes other source file here */

double
vector_mod_fixed_point(double x, double y)
{
  anti_rotate_y_fixed_point(&x, &y, 0.0, 1);
  return x;
}

/*
 * Testcase entry
 */
#ifndef NO_TESTCASE_VECTOR_MOD_FIXED_POINT
int
main(void)
{
  printf("mod{(1, 1)} = %lf\n", vector_mod_fixed_point(1.0, 1.0));
  printf("mod{(1.73205080, 1)} = %lf\n", vector_mod_fixed_point(1.73205080, 1.0));
}
#endif
