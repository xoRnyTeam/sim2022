
// /home/ddrozdov/tools/bin/riscv32-embecosm-ubuntu2204-gcc12.2.0/bin/riscv32-unknown-elf-gcc -march=rv32i 8-queens.c -e main -O1 -o 8.out

int main() {

  int a = 1;
  int b = 1;

  int fib = 0;
  for (int i = 0; i < 100000000; ++i) {
    fib = a + b;
    a = b;
    b = fib;
  }

  asm("ecall");
  //printf("%d", fib);

  return fib;
}