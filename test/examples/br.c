
// /home/ddrozdov/tools/bin/riscv32-embecosm-ubuntu2204-gcc12.2.0/bin/riscv32-unknown-elf-gcc
// -march=rv32i br.c -S -O0

int main() {

  int a = 1;
  int b = 1;

  int fib = 0;
  for (int i = 0; i < 10000000; ++i) {
    fib = a + b;
    a = b;
    b = fib;
  }

  asm("ecall");
  printf("%d", fib);

  return 0;
}