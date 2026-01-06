# include "lib.h"

void	free_array_int(int *ptr, int const n)
{
	if (!ptr)
		return ;
	xfree(ptr, sizeof(int) * n);
}
