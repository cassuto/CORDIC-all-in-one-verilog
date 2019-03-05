#include <math.h>

#define NO_TESTCASE_ANTI_ROTATE_FIXED_POINT

#include "CORDIC-anti-rotate-fixed-point.c" /* uglily includes other source file here */

double
arctan_fixed_point(double val)
{
  double x = 1.0;
  return anti_rotate_y_fixed_point(&x, &val, 0.0, 0/* ignore P */);
}

#ifndef NO_TESTCASE_ARCTAN_FIXED_POINT
int
main(void)
{
  printf("arctan 1 = %lf\n", arctan_fixed_point(1.0));
  printf("arctan 0.55735 = %lf\n", arctan_fixed_point(0.55735));
}
#endif
