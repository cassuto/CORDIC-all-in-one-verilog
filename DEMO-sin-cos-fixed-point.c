
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

/*
 * Testcase entry
 */
#ifndef NO_TESTCASE_SIN_COS_FIXED_POINT
int
main(void)
{
/*
  printf("cos 45deg = %lf\n", cos_fixed_point(45.0));
  printf("sin 39.7918deg = %lf\n", sin_fixed_point(39.7918));
*/
  double avg_diff=0;
  int count=0;
  for(double v=0; v<90; v+=0.1) {
    double d=v/180*M_PI;
    double diff=sin(d)-sin_fixed_point(v);
    diff=diff<0?-diff:diff;
    if(diff>=0.1) {
      printf("Error. deg=%lf: sin=%lf diff:%lf\n", v, sin(d), diff);
    }
    avg_diff+=diff;
    ++count;
  }
  printf("sin_fixed_point(), average diff=%lf\n", avg_diff/count);
}
#endif
