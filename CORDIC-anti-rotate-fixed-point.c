#include <math.h>
#include <stdio.h>

#define COEFF 93207 /* = 2^23 / 90 */
#define N 24
#define PI 3.141592654
#define limP 0.607253

static double P[N] = {
  /* #include "P-float.array" */
  44.9999999941242236900507123209536075592041,
  26.5650511736093122294732893351465463638306,
  14.0362434660937243080525149707682430744171,
  7.1250163479714636594053445151075720787048,
  3.5763343745303788345779594237683340907097,
  1.7899106080123554640692873363150283694267,
  0.8951737100941888902738696742744650691748,
  0.4476141708021068033040990030713146552444,
  0.2238105003393144887269272658159025013447,
  0.1119056770515950427480333928542677313089,
  0.0559528918864977339864275052150333067402,
  0.0279764526133507059058036503529365290888,
  0.0139882271404385312429941023992796544917,
  0.0069941136744396774360676971582506666891,
  0.0034970568502473904116956138210525750765,
  0.0017485284267521388965049844443910842529,
  0.0008742642135796250451099109746166959667,
  0.0004371321068152569586096056752921867883,
  0.0002185660534108090134828433770408651071,
  0.0001092830267058020836780721229963830865,
  0.0000546415133529507389561173658076853599,
  0.0000273207566764815833117597404511300851,
  0.0000136603783382415675380595551646756292,
  0.0000068301891691208803307857645725764684
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
 * CORDIC: anti-rotate the source point, keep iterating util sy == sdst_y.
 * @param sx      Pointer to the X variable of source point. when calc_new_pos == 1,
 *                it indicated the address to store the result.
 * @param sy      Pointer to the Y variable of source point. when calc_new_pos == 1,
 *                it indicated the address to store the result.
 * @param sdst_y  Destination Y coordinate of target point.
 * @param calc_new_pos Indicates whether to figure out the target point.
 * @return result angle (in degree).
 */
double
anti_rotate_y_fixed_point(double *sx, double *sy, double sdst_y, char calc_new_pos)
{
  int z = 0;
  int x, y, dst_y;
  int x_n, y_n;
  int i;
  
  x = *sx * COEFF; y = *sy * COEFF; dst_y = sdst_y / limP * COEFF;
  
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

/**
 * CORDIC: anti-rotate the source point, keep iterating util sx == sdst_x.
 * @param sx      Pointer to the X variable of source point. when calc_new_pos == 1,
 *                it indicated the address to store the result.
 * @param sy      Pointer to the Y variable of source point. when calc_new_pos == 1,
 *                it indicated the address to store the result.
 * @param sdst_x  Destination X coordinate of target point.
 * @param calc_new_pos Indicates whether to figure out the target point.
 * @return result angle (in degree).
 */
double
anti_rotate_x_fixed_point(double *sx, double *sy, double sdst_x, char calc_new_pos)
{
  int z = 0;
  int x, y, dst_x;
  int x_n, y_n;
  int i;
  
  x = *sx * COEFF; y = *sy * COEFF; dst_x = sdst_x / limP * COEFF;
  
  for(i=0; i<N; i++)
    {
      if( x > dst_x )
        {
          x_n = x - (y >> i);
          y_n = y + (x >> i);
          z -= atan_n[i];
        }
      else if( x < dst_x )
        {
          x_n = x + (y >> i);
          y_n = y - (x >> i);
          z += atan_n[i];
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

/*
 * Testcase entry
 */
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
