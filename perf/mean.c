#include <stdio.h>
#include <math.h>

float square(float x) {
	return x*x;
}

void print_usage() {
	printf("mean - compute mean on file and print on standard output\n");
	printf("mean [-v] ... [FILE] ...\n");
	printf("-v\n\t print variance and standard deviation also\n");
}

int main(int argc, char **argv) {
  float x;
  float xsum_t = 0;
  float xsqsum_t = 0;
  float xbar_t = 0;  
  float xsqbar_t = 0;
  float xstddev_t = 0;
  float xvar_t = 0;
  int t=1;
  int print_var = 0;

  if(argc > 1) {
	  if(strcmp(argv[1], "-v") == 0)
		  print_var = 1;
	  else {
		  printf("Invalid option %s\n", argv[1]);
		  print_usage();
		  return -1;
	  }
  }

  while(scanf("%f", &x) != EOF) {
	  // mean of t numbers
	  xsum_t += x;
	  xsqsum_t += x*x;

	  xbar_t = xsum_t/t;
	  
	  // mean of t squared numbers
	  xsqbar_t = xsqsum_t/t;
	  
	  //variance of t numbers
	  
	  xvar_t = xsqbar_t - square(xbar_t);
	  
	  //standard deviation of t numbers
	  xstddev_t = sqrt(xvar_t);
	  t++;
  }
  if(print_var)
	  printf("%f %f %f\n", xbar_t, xvar_t, xstddev_t);
  else 
	  printf("%f\n", xbar_t);

}
