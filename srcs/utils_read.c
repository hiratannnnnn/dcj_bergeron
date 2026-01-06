#include "genome.h"

void	put_nums(int *perm, char *line, int n)
{
	int i;
	char *cur, *endptr;

	i = 0;
	cur = line;
	while (i < n)
	{
		while (*cur == ' ' || *cur == '\n' || *cur == '<' ||
			*cur == '>' || *cur == '(' || *cur == ')')
			cur++;
		if (!*cur)
			break;
		perm[i] = (int)strtol(cur, &endptr, 10);
		cur = endptr;
		i++;
	}
}

// void	cut_line(Extremity_ctx *ctx, Extremity **exts, char const *line, int n)
// {
// 	int i, tmp;
// 	char *cur, *endptr;
// 	Extremity *ext;

// 	i = 0;
// 	tmp = INT_MIN;
// 	cur = (char *)line;
// 	while (i < n)
// 	{
// 		while (*cur == ' ' || *cur == '\n')
// 			cur++;
// 		if (!*cur)
// 			break;
// 		if (*cur == '^')
// 		{
// 			ext = get_ext_by_value(ctx, exts, tmp);
// 			cut_exts(ctx, ext, ext->reality);
// 			cur++;
// 		}
// 		else
// 		{
// 			tmp = (int)strtol(cur, &endptr, 10);
// 			cur = endptr;
// 			i++;
// 		}
// 	}
// }

void    cut_line(Extremity_ctx *ctx, Extremity **exts, char const *line, int n, int *n_adj)
{
    int i;
    char *cur, *endptr;
    int val;
    Extremity *first, *last;

    i = 0;
    cur = (char *)line;
    first = NULL;
    while (i < n || *cur)
    {
        while (*cur == ' ' || *cur == '\n' || *cur == '\t')
            cur++;
        if (!*cur)
            break;

        if (*cur == '<')
        {
            cur++;
            while (*cur == ' ' || *cur == '\t')
                cur++;
            val = (int)strtol(cur, &endptr, 10);
            first = get_ext_by_value(ctx, exts, (-1) * val);
            first->reality = ctx->ext_l;
            ctx->ext_l->reality = first;
			(*n_adj)++;
            cur = endptr;
            i++;
        }
        else if (*cur == '(')
        {
            cur++;
            while (*cur == ' ' || *cur == '\t')
                cur++;
            val = (int)strtol(cur, &endptr, 10);
            first = get_ext_by_value(ctx, exts, (-1) * val);
            cur = endptr;
            i++;
        }
        else if (*cur == '>')
        {
            val = ctx->perm[i - 1];
            last = get_ext_by_value(ctx, exts, val);
            last->reality = ctx->ext_r;
            ctx->ext_r->reality = last;
            cur++;
        }
        else if (*cur == ')')
        {
            val = ctx->perm[i - 1];
            last = get_ext_by_value(ctx, exts, val);
            last->reality = first;
            first->reality = last;
            cur++;
        }
        else
        {
            strtol(cur, &endptr, 10);
            if (cur != endptr)
            {
                cur = endptr;
                i++;
            }
            else
                cur++;
        }
    }
}

int count_sentinels(char const *line)
{
	char *str;
	int count;

	str = (char *)line;
	count = 0;
	while (*str)
	{
		if (*str == '<' || *str == '(')
			count++;
		str++;
	}
	return (count);
}

// void	put_nums_sentinels(int *perm, int *perm_sent, int *sigma_sent, char const *line, int n)
// {
// 	int i, j;
// 	char *cur, *endptr;

// 	i = 0;
// 	j = 0;
// 	cur = (char *)line;
// 	perm_sent[j] = 0;
// 	sigma_sent[j] = 1;
// 	j++;
// 	while (i < n)
// 	{
// 		while (*cur == ' ' || *cur == '\n')
// 			cur++;
// 		if (!*cur)
// 			break;
// 		if (*cur == '^')
// 		{
// 			perm_sent[j] = n + 1;
// 			sigma_sent[j] = 1;
// 			j++;
// 			perm_sent[j] = 0;
// 			sigma_sent[j] = 1;
// 			j++;
// 			cur = cur + 1;
// 		}
// 		strtol(cur, &endptr, 10);
// 		perm_sent[j] = abs(perm[i]);
// 		sigma_sent[j] = abs(perm[i]) / perm[i];
// 		i++;
// 		j++;
// 		cur = endptr;
// 	}
// 	perm_sent[j] = n + 1;
// 	sigma_sent[j] = 1;
// }

void    put_nums_sentinels(int *perm, int *perm_sent, int *sigma_sent, char const *line, int n)
{
    int i, j;
    char *cur, *endptr;

    i = 0;
    j = 0;
    cur = (char *)line;
    while (i < n)
    {
        while (*cur == ' ' || *cur == '\n' || *cur == '\t')
            cur++;
        if (*cur == '<')
        {
            perm_sent[j] = 0;
            sigma_sent[j] = 1;
            j++;
            cur++;
        }
        else if (*cur == '(')
        {
            perm_sent[j] = -1;
            sigma_sent[j] = 1;
            j++;
            cur++;
        }
        while (i < n)
        {
            while (*cur == ' ' || *cur == '\t')
                cur++;
            if (*cur == '>' || *cur == ')')
                break;
            strtol(cur, &endptr, 10);
            if (cur == endptr)
                break;
            perm_sent[j] = abs(perm[i]);
            sigma_sent[j] = abs(perm[i]) / perm[i];
            i++;
            j++;
            cur = endptr;
        }
        while (*cur == ' ' || *cur == '\t')
            cur++;
        if (*cur == '>')
        {
            perm_sent[j] = n + 1;
            sigma_sent[j] = 1;
            j++;
            cur++;
        }
        else if (*cur == ')')
        {
            perm_sent[j] = n + 2;
            sigma_sent[j] = 1;
            j++;
            cur++;
        }
    }
}

int read_first_line(int fd, Extremity_ctx *ctx, int *n)
{
	char *line;

	line = get_next_line(fd);
	*n = atoi(line);
	ctx->n = *n;
	free(line);
	if (*n <= 0)
		return (printf("Wrong size.\n"), -1);
	return (1);
}

int read_second_line(int fd, Extremity_ctx *ctx)
{
	int n, c_sent;
	int *perm, *perm_sent, *sigma_sent;
	char *line;

	n = ctx->n;
	line = get_next_line(fd);
	if (!line)
		return (-1);
	perm = (int *)xmalloc(sizeof(int) * (n));
	if (!perm)
		return (-1);
	put_nums(perm, line, n);
	ctx->perm = perm;
	c_sent = count_sentinels(line);
	ctx->n_sent = n + c_sent * 2;
	perm_sent = (int *)xmalloc(sizeof(int) * (ctx->n_sent));
	if (!perm_sent)
		return (-1);
	sigma_sent = (int *)xmalloc(sizeof(int) * (ctx->n_sent));
	if (!sigma_sent)
		return (-1);
	put_nums_sentinels(perm, perm_sent, sigma_sent, line, n);
	ctx->perm_sent = perm_sent;
	ctx->sigma_sent = sigma_sent;
	ctx->exts = perm_to_exts(perm, n);
	ctx->ext_l = create_ext(0, 1);
	if (!ctx->ext_l)
		return (-1);
	ctx->ext_r = create_ext(n + 1, -1);
	if (!ctx->ext_r)
		return (-1);
	put_realities(ctx, ctx->exts, ctx->ext_l, ctx->ext_r, perm, n);
	put_desires(ctx->exts, ctx->ext_l, ctx->ext_r, n);
	ctx->n_adj_a = ctx->n;
	cut_line(ctx, ctx->exts, line, n, &(ctx->n_adj_a));
	free(line);
	return (1);
}

int read_third_line(int fd, Extremity_ctx *ctx)
{
	int n, cb_sent;
	int *id_perm;
	char *line;

	n = ctx->n;
	line = get_next_line(fd);
	id_perm = gen_id_perm(n);
	if (!id_perm)
		return (-1);
	cb_sent = count_sentinels(line);
	ctx->nb_sent = n + 2 * cb_sent;
	ctx->pb_sent = (int *)xmalloc(sizeof(int) * (ctx->nb_sent));
	if (!ctx->pb_sent)
		return (-1);
	ctx->sb_sent = (int *)xmalloc(sizeof(int) * (ctx->nb_sent));
	if (!ctx->sb_sent)
		return (-1);
	put_nums_sentinels(id_perm, ctx->pb_sent, ctx->sb_sent, line, n);
	ctx->goals = perm_to_exts(id_perm, n);
	ctx->goal_l = create_ext(0, 1);
	if (!ctx->goal_l)
		return (-1);
	ctx->goal_r = create_ext(n + 1, -1);
	if (!ctx->goal_r)
		return (-1);
	put_realities(ctx, ctx->goals, ctx->goal_l, ctx->goal_r, id_perm, n);
	put_desires(ctx->goals, ctx->goal_l, ctx->goal_r, n);
	ctx->n_adj_b = ctx->n;
	cut_line(ctx, ctx->goals, line, n, &(ctx->n_adj_b));
	free(line);
	free_array_int(id_perm, n);
	return (1);
}
