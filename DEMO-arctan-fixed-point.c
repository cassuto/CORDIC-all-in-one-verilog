#include <math.h>

#define NO_TESTCASE_ANTI_ROTATE_FIXED_POINT

#include "CORDIC-anti-rotate-fixed-point.c" /* uglily includes other source file here */

double
arctan_fixed_point(double val)
{
  double x = 1.0;
  return anti_rotate_y_fixed_point(&x, &val, 0.0, 0/* ignore P */);
}

/*
 * Testcase entry
 */
#ifndef NO_TESTCASE_ARCTAN_FIXED_POINT
int
main(void)
{
/*
  printf("arctan 1 = %lf\n", arctan_fixed_point(1.0));
  printf("arctan 0.55735 = %lf\n", arctan_fixed_point(0.55735));
*/
  double avg_diff=0;
  int count=0;
  for(double v=0; v<89; v+=0.1) {
    double d=v/180*M_PI;
    double diff=v-arctan_fixed_point(tan(d));
    diff=diff<0?-diff:diff;
    if(diff>=0.1) {
      printf("Error. deg=%lf: tan=%lf diff:%lf\n", v, tan(d), diff);
    }
    avg_diff+=diff;
    ++count;
  }
  printf("arctan_fixed_point(), average diff=%lf\n", avg_diff/count);
}
#endif
