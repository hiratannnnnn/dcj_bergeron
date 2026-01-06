# include "lib.h"

void	*xmalloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		fprintf(stderr, "failed to allocate memory size of %d.\n", (int)size);
		return (NULL);
	}
	mem += size;
	if (mem > mem_peak)
		mem_peak = mem;
	return ptr;
}

void	*xcalloc(int n, size_t size_each)
{
	void *ptr;

	ptr = xmalloc(size_each * n);
	if (!ptr)
	{
		fprintf(stderr, "failed to allocate memory size of %d.\n", (int)size_each * n);
		return (NULL);
	}
	memset(ptr, 0, n * size_each);
	return ptr;
}

void	xfree(void *ptr, size_t size)
{
	if (ptr)
	{
		free(ptr);
		mem -= size;
	}
}
