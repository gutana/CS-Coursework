/*

QUESTIONS:
       3. Smaller 
       4. No. Variables on stack are local and only exist
              in the current stack frame. 
              When a new variable is assigned on the stack, the 
              stack pointer is incremented by sizeof the new variable. 
              Consequently, when a variable is declared on the stack, another
              cannot be at that same address. 
              VIRTUALLY: Multiple processes may have dat at the same virtual address on the stack,
              but that is not physically the same memory. 
       5. On the stack. 
              I believe I was correct. The addresses of the arguments
              gets lower, indicating they are on the stack. 
*/

#include <stdio.h>

int factorial(int n)
{
       printf("factorial(%d) address: %p\n", n, &factorial);
       printf("factorial(%d) argument address: %p\n", n, &n);

       if (n == 1) return 1; 

       return n * factorial(n - 1);
}

void main()
{
       int num; 

       printf("Please enter number to factorial: ");
       scanf("%x", &num);

       int result = factorial(num);

       printf("%d factorial is %d\n", num, result);
}

/*
       +------------------+
       |  TEXT            | 0x558348bf73d2
       |                  |
       |  instructions in | 0x558348bf73d2 = proc1
       |  machine code    | 0x558348bf74d7 = proc2
       |                  |
       +------------------+ 0x558348bf7695 = etext
       |  DATA            | 0x558348bfa010 = g1   global initialized
       |  - initialized   | 0x558348bfa014 = g3
       |    variables     | 0x558348bfa018 = ls1  static initialized
       |                  | 0x558348bfa01c = ls3
       |                  |
       | ---------------  | 0x558348bfa020 = edata
       |  - uninitialized | 0x558348bfa024 = ls0  static uninitialized
       |    variables     | 0x558348bfa028 = ls2
       |                  | 0x558348bfa044 = g0   global uninitialized
       |                  | 0x558348bfa040 = g4
       |                  | 0x558348bfa060 = g2   big array
       |                  |
       |                  |
       | ---------------  | 0x558348bfb7d0 = end
       |  - heap for      | 0x55834a8d02a0 pointed to by heap1
       |  dynamic         | 0x55834a8d02c0 pointed to by heap2
       |  allocation      |
       |                  |
       |                  |
       +------------------+
                |
                |
                V

                .
                .
                .

                ^
                |
                |
       +------------------+
       |  STACK           | 0x7ffe04536f34 = lc4, lc6   | Activation record for
       |  - function      | 0x7ffe04536f30 = lc3, lc5   | proc1 or proc2
       |    activation    |
       |    records       |
       |  - local         | 0x7ffe04536f64 = lc2       | Activation record for
       |    automatic     | 0x7ffe04536f60 = lc1       | main
       |    variables     | 0x7ffe04536f5c = lc0       |
       +------------------+
*/