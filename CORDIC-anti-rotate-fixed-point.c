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

double
anti_rotate_y_fixed_point(double *sx, double *sy, double sdst_y, char calc_new_pos)
{
  int z = 0;
  int x, y, dst_y;
  int x_n, y_n;
  int i;
  
  x = *sx * COEFF; y = *sy * COEFF; dst_y = sdst_y * COEFF;
  
  for(i=0; i<N; i++)
    {
      if( y > dst_y )
        {
          x_n = x + (y >> i);
          y_n = y - (x >> i);
          z += atan_n[i];
        }
      else if( y < dst_y )
        {
          x_n = x - (y >> i);
          y_n = y + (x >> i);
          z -= atan_n[i];
        }
      else
        break;
      x = x_n;
      y = y_n;
    }
  --i;
  if(calc_new_pos)
    {
      *sx = (double)x/COEFF * P[i];
      *sy = (double)y/COEFF * P[i];
    }
#ifndef NO_TESTCASE_ANTI_ROTATE_FIXED_POINT
  printf("(iteration count: %d)\n", i+1);
#endif
  return (double)z/COEFF;
}

double
anti_rotate_x_fixed_point(double *sx, double *sy, double sdst_x, char calc_new_pos)
{
  int z = 0;
  int x, y, dst_x;
  int x_n, y_n;
  int i;
  
  x = *sx * COEFF; y = *sy * COEFF; dst_x = sdst_x * COEFF;
  
  for(i=0; i<N; i++)
    {
      if( x > dst_x )
        {
          x_n = x - (y >> i);
          y_n = y + (x >> i);
          z += atan_n[i];
        }
      else if( x < dst_x )
        {
          x_n = x + (y >> i);
          y_n = y - (x >> i);
          z -= atan_n[i];
        }
      else
        break;
      x = x_n;
      y = y_n;
    }
  --i;
  if(calc_new_pos)
    {
      *sx = (double)x/COEFF * P[i];
      *sy = (double)y/COEFF * P[i];
    }
#ifndef NO_TESTCASE_ANTI_ROTATE_FIXED_POINT
  printf("(iteration count: %d)\n", i+1);
#endif
  return (double)z/COEFF;
}

#ifndef NO_TESTCASE_ANTI_ROTATE_FIXED_POINT
int
main(void)
{
  double sx = 1.73205080, sy = 1.0;
  double z = anti_rotate_y_fixed_point(&sx, &sy, 0.0, 1);
  
  printf("(1.73205080, 1.0) -- y=0 --> result: angle = %lf deg, (%lf, %lf)\n", z, sx, sy);
  
  sx = 1.73205080; sy = 1.0;
  z = anti_rotate_x_fixed_point(&sx, &sy, 0.0, 1);
  
  printf("(1.73205080, 1.0) -- x=0 --> result: angle = %lf deg, (%lf, %lf)\n", z, sx, sy);
  
  return 0;
}
#endif
