#include<stdio.h>
int main() {
  float val;
  float sum = 0.0;
  int i=0;
  while(scanf("%f", &val) != EOF) {
    sum += val;
    i++;
  }
  printf("%f\n", sum/i);
}
