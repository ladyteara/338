#include <stdio.h>

void main(void) {
  double a, b;
  int n;

  printf("Please enter 3 numbers: ");
  scanf("%lf %lf %d", &a, &b, &n);
  printf("Got %lf, %lf, and %d", a, b, n);
}
