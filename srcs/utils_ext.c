#include "genome.h"

Extremity *create_ext(int id, int sign)
{
	Extremity *ext;

	ext = (Extremity *)xmalloc(sizeof(Extremity) * 1);
	if (!ext)
		return (NULL);
	ext->id = id;
	ext->sign = sign;
	ext->reality = NULL;
	ext->desire = NULL;
	return (ext);
}

void	free_ext(Extremity *ext)
{
	xfree(ext, sizeof(Extremity));
}

void 	free_array_exts(Extremity **exts, int n)
{
	int i;

	i = 0;
	while (i < n * 2)
	{
		if (exts[i])
			free_ext(exts[i]);
		i++;
	}
	xfree(exts, sizeof(Extremity *) * (n * 2));
}

void	cut_exts(Extremity_ctx *ctx, Extremity *left, Extremity *right)
{
	if (left->reality != right || right->reality != left)
	{
		printf("Cannot cut disconnected ones.\n");
		return ;
	}
	left->reality = ctx->ext_r;
	right->reality = ctx->ext_l;
}


Extremity *get_pair_of(Extremity_ctx *ctx, Extremity *ext)
{
	int n;

	n = ctx->n;
	if (ext->id == 0 || ext->id == n + 1)
		return ext;
	if (ext->sign == 1)
		return (ctx->exts[2 * (ext->id - 1)]);
	else
		return (ctx->exts[2 * (ext->id - 1) + 1]);
}

Extremity *get_ext_by_perm_index(Extremity_ctx *ctx, Extremity **exts, int index)
{
	int *perm;
	int sign;
	int id;

	perm = ctx->perm;
	id = abs(perm[index]);
	sign = (perm[index] > 0) ? 1 : 0;
	return (exts[2 * (id - 1) + sign]);
}

Extremity *get_ext_by_value(Extremity_ctx *ctx, Extremity **exts, int value)
{
	int id;
	int sign;
	int n;

	n = ctx->n;
	id = abs(value);
	if (id == 0)
		return (ctx->ext_l);
	if (id == n + 1)
		return (ctx->ext_r);
	
	sign = (value > 0) ? 1 : 0;
	return (exts[2 * (id - 1) + sign]);
}

int get_value_ext(Extremity_ctx *ctx, Extremity *ext)
{
	if (is_nullish(ctx, ext))
		return (0);
	return (ext->id * ext->sign);
}
