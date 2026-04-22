#include <stdio.h>

void foo() {
  int i;
  for (i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      printf("Foo\n");
    } else {
      printf("Foo Bar\n");
    }
  }
  printf("Done\n");
}

void bar() { foo(); }

void baz() {
  foo();
  bar();

  int i = 10;
  while (i > 0) {
    i--;
  }
}

int main() {
  baz();
  foo();
  return 0;
}
