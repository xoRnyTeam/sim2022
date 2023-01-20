

// /home/ddrozdov/tools/bin/riscv32-embecosm-ubuntu2204-gcc12.2.0/bin/riscv32-unknown-elf-gcc -march=rv32i 8-queens.c -e main -O1 -nostdlib -o q8.out


#include <stdio.h>

typedef int bool;

#define TRUE    1
#define FALSE   0
#define MAXCANDIDATES   100		/* max possible next extensions */
#define NMAX            100		/* maximum solution size */

typedef char* data;			/* type to pass data to backtrack */

int solution_count;			/* how many solutions are there? */

bool finished = FALSE;                  /* found all solutions yet? */

void
backtrack(int a[], int k, data input)
{
        int c[MAXCANDIDATES];           /* candidates for next position */
        int ncandidates;                /* next position candidate count */
        int i;                          /* counter */

        if (is_a_solution(a,k,input))
                process_solution(a,k,input);
        else {
                k = k+1;
                construct_candidates(a,k,input,c,&ncandidates);
                for (i=0; i<ncandidates; i++) {
                        a[k] = c[i];
                        backtrack(a,k,input);
			if (finished) return;	/* terminate early */
                }
        }
}


process_solution(int a[], int k)
{
	int i;				/* counter */

	solution_count ++;
}

is_a_solution(int a[], int k, int n)
{
	return (k == n);
}


/*	What are possible elements of the next slot in the 8-queens
 *		problem?
 *		*/

construct_candidates(int a[], int k, int n, int c[], int *ncandidates)
{
	int i,j;			/* counters */
	bool legal_move;		/* might the move be legal? */

	*ncandidates = 0;
	for (i=1; i<=n; i++) {
		legal_move = TRUE;
		for (j=1; j<k; j++) {
			if (abs((k)-j) == abs(i-a[j]))  /* diagonal threat */
				legal_move = FALSE;
			if (i == a[j])                  /* column threat */
				legal_move = FALSE;
		}
		if (legal_move == TRUE) {
			c[*ncandidates] = i;
			*ncandidates = *ncandidates + 1;
		}
	}
}



int main()
{
	int a[NMAX];			/* solution vector */
	int i;				/* counter */
	int compare[]={1, 0, 0, 2, 10, 4, 40, 92};

	for (i=9; i<=9; i++) {
		solution_count = 0;
		backtrack(a,0,i);
		// printf("n=%d  solution_count=%d\n",i,solution_count);
		if (compare[i - 1] != solution_count) {
			break;
		}

	}

	asm("ecall");
	for(int i = 0; i < NMAX; ++i) {
		//printf("%d", a[i]);
	}
	return 0;
}


