#include <math.h>
#include <stdio.h>

#define COEFF 186413 /* = 2^24 / 90 */
#define N 24
#define PI 3.141592654

static double P[N] = {
  #include "P-float.array"
};
static int atan_n[N] = {
  #include "arctan-fixed.array"
};

/**
 * CORDIC: rotate the source point by specific angle in degree.
 * @param sx      Pointer to the X variable of source point, where to
 *                store the result as well.
 * @param sy      Pointer to the Y variable of source point, where to
 *                store the result as well.
 * @param sz      Target rotation angle.
 */
void
rotate_fixed_point(double *sx, double *sy, double sz)
{
  int x, y, z;
  int x_n, y_n, z_n;
  int i;
  
  x = *sx * COEFF; y = *sy * COEFF; z = sz * COEFF;
  
  for(i=0; i<N; i++)
    {
      if( z > 0 )
        {
          x_n = x - (y >> i);
          y_n = y + (x >> i);
          z_n = z - atan_n[i];
        }
      else if( z < 0 )
        {
          x_n = x + (y >> i);
          y_n = y - (x >> i);
          z_n = z + atan_n[i];
        }
      else
        break;
      x = x_n;
      y = y_n;
      z = z_n;
    }
  --i;
  *sx = (double)x/COEFF * P[i];
  *sy = (double)y/COEFF * P[i];
  
#ifndef NO_TESTCASE_ROTATE_FIXED_POINT
  printf("(iteration count: %d)\n", i+1);
#endif
}

/*
 * Testcase entry
 */
#ifndef NO_TESTCASE_ROTATE_FIXED_POINT
int
main(void)
{
  double sx = 2.0, sy = 2.0;
  rotate_fixed_point(&sx, &sy, 45.0);
  
  printf("(2.0, 2.0) -- left 45 deg -- > result: (%lf, %lf)\n", sx, sy);
  
  return 0;
}
#endif
