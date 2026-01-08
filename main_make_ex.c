#include "genome.h"

size_t		mem = 0;
size_t		mem_peak = 0;
clock_t		proc_start;
clock_t		proc_end;

int main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	srand((unsigned int)time(NULL));
	proc_start = clock();

	// mkdir("tests", 0755);

	// for generating some permutation.
	make_example(10, "ex_10.txt", 0.1);
	printf("made ex_20.txt.\n");
	make_example(100, "ex_100.txt", 0.1);
	printf("made ex_100.txt.\n");
	make_example(1000, "ex_1000.txt", 0.1);
	printf("made ex_1000.txt.\n");
	make_example(10000, "ex_10000.txt", 0.1);
	printf("made ex_10000.txt.\n");
	make_example(100000, "ex_100000.txt", 0.1);
	printf("made ex_100000.txt.\n");
	make_example(1000000, "ex_1000000.txt", 0.1);
	printf("made ex_1000000.txt.\n");

	print_info();
	return (0);
}
