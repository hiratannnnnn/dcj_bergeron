
#include "genome.h"

int is_all_true(int *array, int n)
{
	int i;

	for (i = 0; i < n; i++)
		if (array[i] == 0)
			return (0);
	return (1);
}

int 	is_nullish(Extremity_ctx *ctx, Extremity *ext)
{
	return (ext == NULL ||
			ext == ctx->ext_l ||
			ext == ctx->ext_r);
}

void	reverse_p(Extremity_ctx *ctx, int *perm, int *sigma)
{
	int i, n, n_sent;
	Extremity *ext;

	n = ctx->n;
	n_sent = ctx->n_sent;
	i = 0;
	while (i < n_sent)
	{
		ext = get_ext_by_value(ctx, ctx->exts, perm[i] * sigma[i]);
		if (ext->reality == ctx->ext_l)
			ext->reality = ctx->ext_r;
		else if (ext->reality == ctx->ext_r)
			ext->reality = ctx->ext_l;
		ext = get_pair_of(ctx, ext);
		if (ext->reality == ctx->ext_l)
			ext->reality = ctx->ext_r;
		else if (ext->reality == ctx->ext_r)
			ext->reality = ctx->ext_l;
		if (perm[i] == 0 || perm[i] == n + 1)
		{
			perm[i] = n + 1 - perm[i];
			sigma[i] = (-1) * sigma[i];
		}
		i++;
	}
}
