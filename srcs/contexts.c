#include "genome.h"

void	Extremity_free(Extremity_ctx *ctx, int n)
{
	if (ctx->exts)
		free_array_exts(ctx->exts, n);
	ctx->exts = NULL;
	if (ctx->ext_l)
		free_ext(ctx->ext_l);
	ctx->ext_l = NULL;
	if (ctx->ext_r)
		free_ext(ctx->ext_r);
	ctx->ext_r = NULL;
	if (ctx->goals)
		free_array_exts(ctx->goals, n);
	ctx->goals = NULL;
	if (ctx->goal_l)
		free_ext(ctx->goal_l);
	ctx->goal_l = NULL;
	if (ctx->goal_r)
		free_ext(ctx->goal_r);
	ctx->goal_r = NULL;
	if (ctx->perm)
		free_array_int(ctx->perm, ctx->n);
	ctx->perm = NULL;
	if (ctx->perm_sent)
		free_array_int(ctx->perm_sent, ctx->n_sent);
	ctx->perm_sent = NULL;
	if (ctx->sigma_sent)
		free_array_int(ctx->sigma_sent, ctx->n_sent);
	ctx->sigma_sent = NULL;
	if (ctx->pb_sent)
		free_array_int(ctx->pb_sent, ctx->nb_sent);
	ctx->pb_sent = NULL;
	if (ctx->sb_sent)
		free_array_int(ctx->sb_sent, ctx->nb_sent);
	ctx->sb_sent = NULL;
}

void	print_Extremity_ctx(Extremity_ctx *ctx)
{
	printf("==========\n\n");
	print_array_exts(ctx->exts, ctx->n);
	print_array_exts(ctx->goals, ctx->n);

	printf("%p, %p, %p, %p\n", ctx->ext_l, ctx->ext_r, ctx->goal_l, ctx->goal_r);
	printf("n: %d\n", ctx->n);
	printf("n_sent: %d\n", ctx->n_sent);
	print_array_int(ctx->perm, ctx->n, 3);
	print_array_int(ctx->perm_sent, ctx->n_sent, 3);
	print_array_int(ctx->sigma_sent, ctx->n_sent, 3);
	print_array_int(ctx->pb_sent, ctx->nb_sent, 3);
	print_array_int(ctx->sb_sent, ctx->nb_sent, 3);

	printf("\n==========\n");

}

void	put_desires(Extremity **exts, Extremity *ext_l, Extremity *ext_r, int n)
{
	int i, j;
	Extremity *cur, *tar;
	int cur_id, cur_sign;
	int tar_id, tar_sign;

	for (i = 0; i < n * 2 ; i++)
	{
		cur = exts[i];
		cur_id = cur->id;
		cur_sign = cur->sign;
		tar_id = abs(cur_id) + cur_sign;
		tar_sign = cur_sign * (-1);
		if (tar_id == 0)
		{
			ext_l->desire = cur;
			cur->desire = ext_l;
		}
		else if (tar_id == n + 1)
		{
			ext_r->desire = cur;
			cur->desire = ext_r;
		}
		/** improved order */
		if (tar_id == 0)
			tar = ext_l;
		else if (tar_id == n + 1)
			tar = ext_r;
		else
		{
			tar = (tar_sign == 1) ?
				exts[2 * (tar_id - 1) + 1] :
				exts[2 * (tar_id - 1)];
		}
		tar->desire = cur;
	}
}

void	put_realities(Extremity_ctx *ctx, Extremity **exts, Extremity *ext_l, Extremity *ext_r, int *perm, int n)
{
	int i;
	int id1, id2, sign1, sign2;
	Extremity *ext1_right, *ext2_left;

	for (i = 0; i < n - 1; i++)
	{
		id1 = abs(perm[i]);
		sign1 = id1 / perm[i]; // no zeros
		id2 = abs(perm[i + 1]);
		sign2 = id2 / perm[i + 1];
		ext1_right = (sign1 == 1) ?
			exts[2 * (id1 - 1) + 1] :
			exts[2 * (id1 - 1)];
		ext2_left = (sign2 == 1) ?
			exts[2 * (id2 - 1)] :
			exts[2 * (id2 - 1) + 1];
		ext1_right->reality = ext2_left;
		ext2_left->reality = ext1_right;
	}

	Extremity *ext_first, *ext_last;
	id1 = abs(perm[0]);
	sign1 = id1 / perm[0];
	id2 = abs(perm[n - 1]);
	sign2 = id2 / perm[n - 1];
	ext_first = get_ext_by_value(ctx, exts, (-1) * perm[0]);
	ext_last = get_ext_by_value(ctx, exts, perm[n - 1]);
	ext_first->reality = ext_l;
	ext_l->reality = ext_first;
	ext_last->reality = ext_r;
	ext_r->reality = ext_last;
}

Extremity **perm_to_exts(int *perm, int n)
{
	Extremity **exts;
	int i, g_id;

	exts = (Extremity **)xmalloc(sizeof(Extremity *) * (n * 2));
	if (!exts)
		return (NULL);
	i = 0;
	while (i < n)
	{
		g_id = abs(perm[i]);
		exts[2 * (g_id - 1)] = create_ext(g_id, -1);
		exts[2 * (g_id - 1) + 1] = create_ext(g_id, 1);
		if (!exts[2 * (g_id - 1)] || !exts[2 * (g_id - 1) + 1])
			return (free_array_exts(exts, n), NULL);
		i++;
	}
	return (exts);
}

int	Extremity_init(char const *filename, Extremity_ctx *ctx, int *n)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 3)
		return (printf("Wrong file.\n"), -1);
	if (read_first_line(fd, ctx, n) == -1)
		return (-1);
	printf("read first line\n");
	if (read_second_line(fd, ctx) == -1)
		return (-1);
	printf("read second line.\n");
	if (read_third_line(fd, ctx) == -1)
		return (-1);
	printf("read third line.\n");
	close(fd);
	return (1);
}
