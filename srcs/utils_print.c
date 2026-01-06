#include "genome.h"

void 	print_array_exts(Extremity **exts, int n)
{
	int i;
	int id1, sign1;
	int id2, sign2;
	Extremity *ext1, *ext2;

	printf("==== Printing Connections from each Extremity. ====\n\n");
	i = 0;
	while (i < n)
	{
		ext1 = exts[i * 2];
		ext2 = exts[i * 2 + 1];
		id1 = ext1->id;
		id2 = ext2->id;
		sign1 = ext1->sign;
		sign2 = ext2->sign;
		printf("%d->", id1 * sign1);
		if (ext1->reality)
			printf("%d\n", ext1->reality->id * ext1->reality->sign);
		else
			printf("None\n");
		printf("%d->", id2 * sign2);
		if (ext2->reality)
			printf("%d\n", ext2->reality->id * ext2->reality->sign);
		else
			printf("None\n");
		i++;
	}
}

void	print_perm_sent(int *perm, int *sigma, int n)
{
	int i;

	printf("ind:");
	i = 0;
	while (i < n)
	{
		if (i < 10)
			printf("   %d", i);
		else
			printf("  %d", i);
		i++;
	}
	printf("\ngem:");
	i = 0;
	while (i < n)
	{
		if (sigma[i] == 1)
		{
			if (perm[i] < 10)
				printf("   ");
			else
				printf("  ");
		}
		if (sigma[i] == -1)
		{
			if (perm[i] < 10)
				printf("  -");
			else
				printf(" -");
		}
		printf("%d", perm[i]);
		i++;
	}
	printf("\n");
}

void	chase_exts(Extremity_ctx *ctx, Extremity *ext, int *used)
{
	Extremity *cur, *pair, *start;

	if (ext->reality == NULL)
		printf("$ ");
	else if (ext->reality == ctx->ext_l ||
			 ext->reality == ctx->ext_r)
		printf("=== ");
	cur = ext;
	start = ext;
	while (1)
	{
		used[cur->id - 1] = 1;
		// pair = (cur->sign == 1) ?
		// 	ctx->exts[2 * (cur->id - 1)] :
		// 	ctx->exts[2 * (cur->id - 1) + 1];
		pair = get_pair_of(ctx, cur);
		printf("%d ", pair->id * pair->sign);
		cur = pair->reality;
		if (cur == NULL ||
			cur == ctx->ext_l ||
			cur == ctx->ext_r ||
			cur == start)
			break;
	}
	if (cur == NULL)
		printf(" $\n");
	else if (cur == ctx->ext_l || cur == ctx->ext_r)
		printf(" ===\n");
}

void	print_chromosomes(Extremity_ctx *ctx, int n)
{
	int i;
	int *used;
	Extremity *ext;

	used = (int *)xmalloc(sizeof(int) * n);
	if (!used)
		return ;
	for (i = 0; i < n; i++)
		used[i] = 0;
	for (i = 0; i < n * 2; i++)
	{
		ext = ctx->exts[i];
		if (ext->reality == NULL ||
			ext->reality == ctx->ext_l ||
			ext->reality == ctx->ext_r)
			if (used[ext->id - 1] == 0)
				chase_exts(ctx, ext, used);
	}
	free_array_int(used, n);
}
