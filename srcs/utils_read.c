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

static void connect_genes(Extremity *a, Extremity *b)
{
    if (a && a->reality) {
        if (a->reality->reality == a) a->reality->reality = NULL;
    }
    if (b && b->reality) {
        if (b->reality->reality == b) b->reality->reality = NULL;
    }
    if (a) a->reality = b;
    if (b) b->reality = a;
}

static void connect_to_cap(Extremity *gene, Extremity *cap)
{
    if (gene && gene->reality) {
         if (gene->reality->reality == gene) gene->reality->reality = NULL;
    }
    if (gene) gene->reality = cap;
    if (cap) cap->reality = gene; 
}

void    cut_line(Extremity_ctx *ctx, Extremity **exts, char const *line, int n, int *n_adj)
{
    char *cur, *endptr;
    int val;
    Extremity *first = NULL; 
    Extremity *last = NULL;

    cur = (char *)line;

    while (*cur)
    {
        while (*cur == ' ' || *cur == '\n' || *cur == '\t')
            cur++;
        if (!*cur) break;

        if (*cur == '<') // 線形染色体 開始
        {
            cur++;
            while (*cur == ' ' || *cur == '\t') cur++;
            val = (int)strtol(cur, &endptr, 10);
            cur = endptr;

            first = get_ext_by_value(ctx, exts, (-1) * val);
            
            // 左端を ext_l に接続
            connect_to_cap(first, ctx->ext_l);
            
            last = get_ext_by_value(ctx, exts, val);
            (*n_adj)++;
        }
        else if (*cur == '(') // 円形染色体 開始
        {
            cur++;
            while (*cur == ' ' || *cur == '\t') cur++;
            val = (int)strtol(cur, &endptr, 10);
            cur = endptr;

            first = get_ext_by_value(ctx, exts, (-1) * val);
            last = get_ext_by_value(ctx, exts, val);
        }
        else if (*cur == '>') // 線形染色体 終了
        {
            // 右端を ext_r に接続
            connect_to_cap(last, ctx->ext_r);
            
            first = NULL; last = NULL; cur++;
        }
        else if (*cur == ')') // 円形染色体 終了
        {
            // 最初と最後を繋ぐ
            connect_genes(last, first);
            first = NULL; last = NULL; cur++;
        }
        else // 遺伝子
        {
            val = (int)strtol(cur, &endptr, 10);
            if (cur == endptr) { cur++; continue; }
            cur = endptr;

            Extremity *current_left = get_ext_by_value(ctx, exts, (-1) * val);
            
            if (last) {
                // 通常の遺伝子間接続
                connect_genes(last, current_left);
            }

            last = get_ext_by_value(ctx, exts, val);
        }
    }
}
// ...existing code...
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
