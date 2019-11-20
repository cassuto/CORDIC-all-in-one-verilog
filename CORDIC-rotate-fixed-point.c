#include <math.h>
#include <stdio.h>

#define COEFF 93207 /* = 2^23 / 90 */
#define N 24
#define PI 3.141592654

static double P[N] = {
  /* #include "P-float.array" */
  0.7071067811865474617150084668537601828575,
  0.6324555320336757713306496953009627759457,
  0.6135719910778962837838435007142834365368,
  0.6088339125177524291387953780940733850002,
  0.6076482562561682509993943313020281493664,
  0.6073517701412960434481647098436951637268,
  0.6072776440935261366149688910809345543385,
  0.6072591122988928447057332959957420825958,
  0.6072544793325624912228022367344237864017,
  0.6072533210898752864537186724191997200251,
  0.6072530315291344571448917122324928641319,
  0.6072529591389449477034645497042220085859,
  0.6072529410413972650317759871541056782007,
  0.6072529365170102888527026152587495744228,
  0.6072529353859135170523586566559970378876,
  0.6072529351031393796134238982631359249353,
  0.6072529350324458174981145930360071361065,
  0.6072529350147723992137116511003114283085,
  0.6072529350103540446426109156163875013590,
  0.6072529350092494837554113473743200302124,
  0.6072529350089733712891870709427166730165,
  0.6072529350089043154170553862059023231268,
  0.6072529350088871069601736962795257568359,
  0.6072529350088827770903776581690181046724
};
static int atan_n[N] = {
  /* #include "arctan-fixed.array" */
  4194314,
  2476048,
  1308276,
  664101,
  333339,
  166832,
  83436,
  41720,
  20860,
  10430,
  5215,
  2607,
  1303,
  651,
  325,
  162,
  81,
  40,
  20,
  10,
  5,
  2,
  1,
  0
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
