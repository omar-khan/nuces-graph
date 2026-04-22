#include <stdio.h>

void test_sequence() {
  int x = 10;
  x = x + 5;
  printf("Sequence: %d\n", x);
}

void test_if_else(int a) {
  if (a > 0) {
    printf("Positive\n");
  } else {
    printf("Non-positive\n");
  }

  printf("back\n");
}

void test_switch(int a) {
  switch (a) {
  case 1:
    printf("One\n");
    break;
  case 2:
    printf("Two\n");
    break;
  default:
    printf("Other\n");
    break;
  }
}

void test_while() {
  int i = 0;
  while (i < 5) {
    printf("While: %d\n", i);
    i++;
  }
  printf("done\n");
}

void test_do_while() {
  int i = 0;
  do {
    printf("Do-While: %d\n", i);
    i++;
  } while (i < 5);
  printf("done\n");
}

void test_for() {
  for (int i = 0; i < 5; i++) {
    printf("For: %d\n", i);
  }
  printf("done\n");
}

int main() {
  test_sequence();
  test_if_else(10);
  test_switch(1);
  test_while();
  test_do_while();
  test_for();
  return 0;
}
