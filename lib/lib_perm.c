#include "lib.h"

int		*gen_perm(int start, int end, int *n)
{
	int *perm;
	int i;

	*n = end - start + 1;
	if (*n < 1)
		return (NULL);
	perm = xmalloc(sizeof(int) * (*n));
	if (!perm)
		return (NULL);

	for (i = 0; i < *n; i++)
		perm[i] = start + i;
	shuffle_perm(perm, *n);
	return perm;
}

int 	*gen_unsigned_perm(int n)
{
	int *perm;
	int i;

	perm = xmalloc(sizeof(int) * n);
	if (!perm)
		return (NULL);

	for (i = 1; i <= n; i++)
		perm[i-1] = i;
	shuffle_perm(perm, n);
	return (perm);
}

int		*gen_signed_perm(int n)
{
	int *perm;
	int i;

	perm = gen_unsigned_perm(n);
	if (!perm)
		return (NULL);
	for (i = 0; i < n; i++)
		if (rand() % 2 == 0)
			perm[i] = (-1) * perm[i];
	return (perm);
}

int 	*gen_id_perm(int n)
{
	int *perm;
	int i;

	perm = (int *)xmalloc(sizeof(int) * n);
	if (!perm)
		return (NULL);
	for (i = 0; i < n; i++)
		perm[i] = i + 1;
	return (perm);
}

void shuffle_perm(int *perm, int n)
{
	int i, j, tmp;

	// Fisher-Yates Shuffle
	for (i = n - 1; i > 0; i--)
	{
		j = rand() % (i + 1);
		tmp = perm[i];
		perm[i] = perm[j];
		perm[j] = tmp;
	}
}
