#include "genome.h"

size_t		mem = 0;
size_t		mem_peak = 0;
clock_t		proc_start;
clock_t		proc_end;

int main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	proc_start = clock();
	Extremity_ctx ctx;
	int n;

	//                      edit ↓ here
	if (Extremity_init("tests/ex_1000.txt", &ctx, &n) == -1)
		return (printf("Error while reading file.\n"), 1);
	print_Extremity_ctx(&ctx);

	// proc_start = clock();
	// solve_dcj_mine(&ctx, 0);
	solve_dcj_mine(&ctx, 1);

	Extremity_free(&ctx, n);
	free_array_int(ctx.perm, n);
	print_info();
	return (0);
}

// int main(int argc, char **argv)
// {
// 	if (argc != 2)
// 		return (printf("Too few (or much) arguments.\n"), 1);
// 	setvbuf(stdout, NULL, _IONBF, 0);
// 	srand((unsigned int)time(NULL));
// 	proc_start = clock();
// 	Extremity_ctx ctx;
// 	int n;

// 	if (Extremity_init(argv[1], &ctx, &n) == -1)
// 		return (printf("Error while reading file.\n"), 1);

// 	// print_Extremity_ctx(&ctx);

// 	proc_start = clock();
// 	// solve_dcj_mine(&ctx, 0);
// 	solve_dcj_mine(&ctx, 1);

// 	// TODO
// 	Extremity_free(&ctx, n);
// 	free_array_int(ctx.perm, n);
// 	print_info();
// 	return (0);
// }
