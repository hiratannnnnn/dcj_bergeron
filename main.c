#include "genome.h"

size_t      mem = 0;
size_t      mem_peak = 0;
clock_t     proc_start;
clock_t     proc_end;

/**
 * Usage: ./solve [input_file]
 */
int main(int argc, char **argv)
{
    Extremity_ctx ctx;
    int n;

    setvbuf(stdout, NULL, _IONBF, 0);
    proc_start = clock();
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file_path>\n", argv[0]);
        fprintf(stderr, "Example: %s tests/ex_100.txt\n", argv[0]);
        return (1);
    }
    if (Extremity_init(argv[1], &ctx, &n) == -1)
    {
        Extremity_free(&ctx, n);
        return (1);
    }
    // proc_start = clock();

    // print_Extremity_ctx(&ctx);
    solve_dcj_mine(&ctx, 0);
    Extremity_free(&ctx, n);
    print_info();

    return (0);
}
