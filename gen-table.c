#include <math.h>
#include <stdio.h>
  
#define COEFF 93207 /* = 2^23 / 90 */
#define N 24
#define PI 3.14159265358979323846

#define GEN_P_FLOAT 1
#define GEN_ATAN_FLOAT 1
#define GEN_ATAN_FIXED 1

double
calc_p(int n)
{
  double sum=1.0;
  for(int i=0; i<=n; i++)
    {
      sum *= 1/sqrt(1+pow(2, -2*i));
    }
  return sum;
}

static FILE *
open_output(const char *file)
{
  printf("Writing to file %s\n", file);
  return fopen(file, "w");
}

int
main(void)
{
  FILE *fp;
  
#if GEN_P_FLOAT
  if( !(fp = open_output("./P-float.table")) )
    return 1;
  for(int i=0 ; i<N; i++)
    {
      fprintf(fp, "%d %.40lf\n", i, calc_p(i));
    }
  fclose(fp);
  
  if( !(fp = open_output("./P-float.array")) )
    return 1;
  for(int i=0 ; i<N; i++)
    {
      fprintf(fp, "%.40lf,\n", calc_p(i));
    }
  fclose(fp);
#endif

#if GEN_ATAN_FLOAT
  if( !(fp = open_output("./arctan-float.table")) )
    return 1;
  for(int i=0 ; i<N; i++)
    {
      fprintf(fp, "%d %.40lf\n", i, atan(1.0/(1<<i)) / PI * 180.0);
    }
  fclose(fp);
  
  if( !(fp = open_output("./arctan-float.array")) )
    return 1;
  for(int i=0 ; i<N; i++)
    {
      fprintf(fp, "%.40lf,\n", atan(1.0/(1<<i)) / PI * 180.0);
    }
  fclose(fp);
#endif

#if GEN_ATAN_FIXED
  if( !(fp = open_output("./arctan-fixed.table")) )
    return 1;
  for(int i=0 ; i<N; i++)
    {
      fprintf(fp, "%d %d\n", i, (int)(atan(1.0/(1<<i)) / PI * 180.0 * COEFF));
    }
  fclose(fp);
  
  if( !(fp = open_output("./arctan-fixed.array")) )
    return 1;
  for(int i=0 ; i<N; i++)
    {
      fprintf(fp, "%d,\n", (int)(atan(1.0/(1<<i)) / PI * 180.0 * COEFF));
    }
  fclose(fp);
#endif

  return 0;
}
