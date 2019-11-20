#include <math.h>

#define NO_TESTCASE_ANTI_ROTATE_FIXED_POINT

#include "CORDIC-anti-rotate-fixed-point.c" /* uglily includes other source file here */

double
arccos_fixed_point(double val)
{
  double x = .0, y = 1.0;
  return anti_rotate_y_fixed_point(&x, &y, val, 0/* ignore P */);
}

double
arcsin_fixed_point(double val)
{
  double x = 1.0, y = .0;
  return anti_rotate_y_fixed_point(&x, &y, val, 0/* ignore P */);
}

/*
 * Testcase entry
 */
#ifndef NO_TESTCASE_ARCSIN_ARCCOS_FIXED_POINT
int
main(void)
{
/*
  printf("arccos 0.48 = %lf\n", arccos_fixed_point(0.64));
  printf("arcsin 0.64 = %lf\n", arcsin_fixed_point(0.44));
*/
  for(double v=0; v<90; v+=0.1) {
    double d=v/180*M_PI;
    double diff=v-arcsin_fixed_point(sin(d));
    diff=diff<0?-diff:diff;
    if(diff>=0.1) {
      printf("Error. deg=%lf: cos=%lf diff:%lf\n", v, sin(d), diff);
    }
  }
}
#endif
