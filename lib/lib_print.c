#include "lib.h"

void print_array_int(int *arr, int n, int max_width)
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("%*d", max_width, arr[i]);
		printf((i == n - 1) ? "\n" : " ");
	}
}

void	print_info()
{
	double used_time;

	proc_end = clock();
	used_time = ((double) (proc_end - proc_start)) / CLOCKS_PER_SEC;
	printf("mem_peak: %d\n", (int)mem_peak);
	printf("remaining mem: %d\n", (int)mem);
	printf("proccess time: %f\n", used_time);
	proc_start = clock();
}
