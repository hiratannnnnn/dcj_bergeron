#include "genome.h"

// static Adjacency *choose_l_r(Extremity_ctx *ctx, Adjacency *next, Adjacency *prev)
// {
// 	int v_1, v_2, v_3, v_4;

// 	v_1 = get_value_ext(ctx, prev->left);
// 	v_2 = get_value_ext(ctx, prev->right);
// 	v_3 = get_value_ext(ctx, next->left);
// 	v_4 = get_value_ext(ctx, next->right);
// 	if (v_3 == 0 || v_4 == 0)
// 		return NULL;
// 	else if (v_1 == v_3)
// 		return next->right_to;
// 	else if (v_1 == v_4)
// 		return next->left_to;
// 	else if (v_2 == v_3)
// 		return next->right_to;
// 	else if (v_2 == v_4)
// 		return next->left_to;
// 	return (printf("ERROR.\n"), NULL);
// }

static Adjacency *choose_l_r(Extremity_ctx *ctx, Adjacency *next, Adjacency *prev)
{
    // next（今いるノード）の left_to か right_to のうち、
    // prev（直前にいたノード）と一致する方を探し、その「反対側」の行き先を返します。

    if (next->left_to == prev) {
        return next->right_to; // 左から来たので右の行き先を返す
    } else if (next->right_to == prev) {
        return next->left_to;  // 右から来たので左の行き先を返す
    }

    // 基本的にここには来ないはず（マップが正しければ）
    return NULL;
}

// static int set_maps_adjs(Extremity_ctx *ctx, int *map_A, int *map_B,
// 	Adjacency **adj_A, Adjacency **adj_B, int log_output)
// {
// 	int i, n, n_sent, nb_sent;
// 	int *perm_sent, *sigma_sent, *pb_sent, *sb_sent;
// 	Extremity *left, *right;

// 	n = ctx->n;
// 	n_sent = ctx->n_sent;
// 	nb_sent = ctx->nb_sent;
// 	perm_sent = ctx->perm_sent;
// 	sigma_sent = ctx->sigma_sent;
// 	pb_sent = ctx->pb_sent;
// 	sb_sent = ctx->sb_sent;
// 	for (i = 0; i < n_sent - 1; i++)
// 	{
// 		printf("i: %d.\n", i);
// 		left = get_ext_by_value(ctx, ctx->exts, perm_sent[i] * sigma_sent[i]);
// 		right = get_ext_by_value(ctx, ctx->exts, perm_sent[i + 1] * sigma_sent[i + 1] * (-1));
// 		adj_A[i] = create_adj_node(left, right);
// 		if (!is_nullish(ctx, left))
// 		{
// 			map_A[left->id - 1] = i;
// 			map_A[left->id - 1 + n] = left->sign;
// 		}
// 	}
// 	for (i = 0; i < nb_sent - 1; i++)
// 	{
// 		left = get_ext_by_value(ctx, ctx->goals, pb_sent[i] * sb_sent[i]);
// 		right = get_ext_by_value(ctx, ctx->goals, pb_sent[i + 1] * sb_sent[i + 1] * (-1));
// 		adj_B[i] = create_adj_node(left, right);
// 		if (!is_nullish(ctx, left))
// 		{
// 			map_B[left->id - 1] = i;
// 			map_B[left->id - 1 + n] = left->sign;
// 		}
// 	}
// 	if (log_output)
// 	{
// 		printf("map_A: ");
// 		print_array_int(map_A, 2 * n, 2);
// 		printf("map_B: ");
// 		print_array_int(map_B, 2 * n, 2);
// 	}
// 	return (1);
// }

static int set_maps_adjs(Extremity_ctx *ctx, int *map_A, int *map_B,
    Adjacency **adj_A, Adjacency **adj_B, int log_output)
{
    int i, j, first_idx;
    Extremity *left, *right;

    // --- Process Genome A ---
    j = 0;
    first_idx = -1;
    if (log_output) printf("[DEBUG] set_maps_adjs: Processing Genome A\n");
    for (i = 0; i < ctx->n_sent - 1; i++)
    {
        if (ctx->perm_sent[i] == -1) first_idx = i + 1;

        left = right = NULL;
        // ... (A側の判定ロジックは既存の通り) ...
        if (ctx->perm_sent[i] == 0) {
            left = ctx->ext_l;
            right = get_ext_by_value(ctx, ctx->exts, ctx->perm_sent[i + 1] * ctx->sigma_sent[i + 1] * -1);
        } else if (ctx->perm_sent[i + 1] == ctx->n + 1) {
            left = get_ext_by_value(ctx, ctx->exts, ctx->perm_sent[i] * ctx->sigma_sent[i]);
            right = ctx->ext_r;
        } else if (ctx->perm_sent[i + 1] == ctx->n + 2) {
            left = get_ext_by_value(ctx, ctx->exts, ctx->perm_sent[i] * ctx->sigma_sent[i]);
            right = get_ext_by_value(ctx, ctx->exts, ctx->perm_sent[first_idx] * ctx->sigma_sent[first_idx] * -1);
        } else if (ctx->perm_sent[i] > 0 && ctx->perm_sent[i] <= ctx->n &&
                 ctx->perm_sent[i + 1] > 0 && ctx->perm_sent[i + 1] <= ctx->n) {
            left = get_ext_by_value(ctx, ctx->exts, ctx->perm_sent[i] * ctx->sigma_sent[i]);
            right = get_ext_by_value(ctx, ctx->exts, ctx->perm_sent[i + 1] * ctx->sigma_sent[i + 1] * -1);
        }

        if (left || right) {
			adj_A[j] = create_adj_node(left, right);
			if (!is_nullish(ctx, left)) {
				// インデックス計算: (ID-1)*2 + (sign==-1 ? 0 : 1)
				int idx = (left->id - 1) * 2 + (left->sign == -1 ? 0 : 1);
				map_A[idx] = j;
			}
			if (!is_nullish(ctx, right)) {
				int idx = (right->id - 1) * 2 + (right->sign == -1 ? 0 : 1);
				map_A[idx] = j;
			}
			j++;
		}
    }

   	// --- Process Genome B ---
    j = 0;
    first_idx = -1; // 円形染色体の開始位置を記録用
    if (log_output) printf("[DEBUG] set_maps_adjs: Processing Genome B\n");
    for (i = 0; i < ctx->nb_sent - 1; i++)
    {
        // 101, 102, 0 などの記号を考慮して Extremity を取得
        int val_i = ctx->pb_sent[i];
        int val_next = ctx->pb_sent[i + 1];

        // 記号（セパレータ）そのものはスキップの起点にする
        if (val_i == -1) { first_idx = i + 1; continue; }

        left = right = NULL;

        // 線形開始 <
        if (val_i == 0) {
            left = ctx->ext_l;
            right = get_ext_by_value(ctx, ctx->goals, val_next * ctx->sb_sent[i + 1] * -1);
        }
        // 線形終了 >
        else if (val_next == ctx->n + 1) {
            left = get_ext_by_value(ctx, ctx->goals, val_i * ctx->sb_sent[i]);
            right = ctx->ext_r;
        }
        // 円形終了 ) -> 開始点に繋ぐ
        else if (val_next == ctx->n + 2) {
            left = get_ext_by_value(ctx, ctx->goals, val_i * ctx->sb_sent[i]);
            right = get_ext_by_value(ctx, ctx->goals, ctx->pb_sent[first_idx] * ctx->sb_sent[first_idx] * -1);
        }
        // 通常の遺伝子間
        else if (val_i > 0 && val_i <= ctx->n && val_next > 0 && val_next <= ctx->n) {
            left = get_ext_by_value(ctx, ctx->goals, val_i * ctx->sb_sent[i]);
            right = get_ext_by_value(ctx, ctx->goals, val_next * ctx->sb_sent[i + 1] * -1);
        }

        // 有効なペアが作れた場合のみ登録
        if (left || right) {
			adj_B[j] = create_adj_node(left, right);
			if (!is_nullish(ctx, left)) {
				int idx = (left->id - 1) * 2 + (left->sign == -1 ? 0 : 1);
				map_B[idx] = j;
			}
			if (!is_nullish(ctx, right)) {
				int idx = (right->id - 1) * 2 + (right->sign == -1 ? 0 : 1);
				map_B[idx] = j;
			}
			j++;
		}
    }

    // 実際にセットされた個数をデバッグ出力
    if (log_output) printf("[DEBUG] Final count - adj_A: %d, adj_B: %d\n", ctx->n_adj_a, j);

    return (1);
}

// static int put_adjs(Extremity_ctx *ctx, int *map_A, int *map_B,
// 	Adjacency **adj_A, Adjacency **adj_B, int log_output)
// {
// 	int i, n, n_sent, nb_sent;
// 	int *perm_sent, *sigma_sent, *pb_sent, *sb_sent;
// 	Extremity *left, *right;
// 	int sign_diff;

// 	n = ctx->n;
// 	n_sent = ctx->n_sent;
// 	nb_sent = ctx->nb_sent;
// 	perm_sent = ctx->perm_sent;
// 	sigma_sent = ctx->sigma_sent;
// 	pb_sent = ctx->pb_sent;
// 	sb_sent = ctx->sb_sent;
// 	for (i = 0; i < n_sent - 1; i++)
// 	{
// 		left = adj_A[i]->left;
// 		right = adj_A[i]->right;
// 		if (!is_nullish(ctx, left))
// 		{
// 			sign_diff = (left->sign == map_B[left->id - 1 + n]) ? 0 : 1;
// 			adj_A[i]->left_to = (sign_diff) ?
// 				adj_B[map_B[left->id - 1] - 1] :
// 				adj_B[map_B[left->id - 1]];
// 		}
// 		if (!is_nullish(ctx, right))
// 		{
// 			sign_diff = (right->sign == map_B[right->id - 1 + n]) ? 0 : 1;
// 			adj_A[i]->right_to = (sign_diff) ?
// 				adj_B[map_B[right->id - 1] - 1] :
// 				adj_B[map_B[right->id - 1]];
// 		}
// 	}
// 	for (i = 0; i < nb_sent - 1; i++)
// 	{
// 		left = adj_B[i]->left;
// 		right = adj_B[i]->right;
// 		if (!is_nullish(ctx, left))
// 		{
// 			sign_diff = (left->sign == map_A[left->id - 1 + n]) ? 0 : 1;
// 			adj_B[i]->left_to = (sign_diff) ?
// 				adj_A[map_A[left->id - 1] - 1] :
// 				adj_A[map_A[left->id - 1]];

// 		}
// 		if (!is_nullish(ctx, right))
// 		{
// 			sign_diff = (right->sign == map_A[right->id - 1 + n]) ? 0 : 1;
// 			adj_B[i]->right_to = (sign_diff) ?
// 				adj_A[map_A[right->id - 1] - 1] :
// 				adj_A[map_A[right->id - 1]];
// 		}
// 	}
// 	return (1);
// }

static int put_adjs(Extremity_ctx *ctx, int *map_A, int *map_B,
    Adjacency **adj_A, Adjacency **adj_B, int log_output)
{
    int i, idx;
    Extremity *l, *r;

    // AからBへの接続
    for (i = 0; i < ctx->n_adj_a; i++) {
        l = adj_A[i]->left;
        r = adj_A[i]->right;
        if (!is_nullish(ctx, l)) {
            idx = (l->id - 1) * 2 + (l->sign == -1 ? 0 : 1);
            adj_A[i]->left_to = adj_B[map_B[idx]];
        }
        if (!is_nullish(ctx, r)) {
            idx = (r->id - 1) * 2 + (r->sign == -1 ? 0 : 1);
            adj_A[i]->right_to = adj_B[map_B[idx]];
        }
    }

    // BからAへの接続
    for (i = 0; i < ctx->n_adj_b; i++) {
        l = adj_B[i]->left;
        r = adj_B[i]->right;
        if (!is_nullish(ctx, l)) {
            idx = (l->id - 1) * 2 + (l->sign == -1 ? 0 : 1);
            adj_B[i]->left_to = adj_A[map_A[idx]];
        }
        if (!is_nullish(ctx, r)) {
            idx = (r->id - 1) * 2 + (r->sign == -1 ? 0 : 1);
            adj_B[i]->right_to = adj_A[map_A[idx]];
        }
    }
    return (1);
}

// static int track_paths(Extremity_ctx *ctx, Adjacency **adj_A, Adjacency **adj_B, int log_output)
// {
// 	int i, n, n_sent, cycles, odd_paths;
// 	int len_l, len_r;
// 	Extremity *left, *right;
// 	Adjacency *cur, *prev, *next, *seek_l, *seek_r, *last;

// 	n = ctx->n;
// 	n_sent = ctx->n_sent;
// 	cycles = odd_paths = 0;
// 	for (i = 0; i < n_sent - 1; i++)
// 	{
// 		if (adj_A[i]->visited)
// 		{
// 			if (log_output)
// 				printf("i=%d: visited.\n", i);
// 			continue;
// 		}
// 		cur = adj_A[i];
// 		if (log_output)
// 			printf("i=%d: STARTING FROM (%d, %d)\n", i,
// 				get_value_ext(ctx, cur->left),
// 				get_value_ext(ctx, cur->right));
// 		len_l = len_r = 0;
// 		left = cur->left;
// 		right = cur->right;
// 		seek_l = cur->left_to;
// 		seek_r = cur->right_to;
// 		last = NULL;
// 		cur->visited = 1;
// 		prev = cur;
// 		next = seek_l;
// 		while (next)
// 		{
// 			len_l++;
// 			if (log_output)
// 				printf("    len_l = %d: seeking left. (%d, %d)\n", len_l,
// 					get_value_ext(ctx, next->left),
// 					get_value_ext(ctx, next->right));
// 			next->visited = 1;
// 			last = next;
// 			next = choose_l_r(ctx, next, prev);
// 			prev = last;
// 			if (cur == prev)
// 			{
// 				if (seek_r)
// 				{
// 					if (log_output)
// 						printf("    found a cycle.\n");
// 					cycles++;
// 				}
// 				break;
// 			}
// 		}
// 		if (cur == last)
// 			continue;
// 		prev = cur;
// 		next = seek_r;
// 		while (next)
// 		{
// 			len_r++;
// 			if (log_output)
// 				printf("    len_r = %d: seeking right. (%d, %d)\n", len_r,
// 					get_value_ext(ctx, next->left),
// 					get_value_ext(ctx, next->right));
// 			next->visited = 1;
// 			last = next;
// 			next = choose_l_r(ctx, next, prev);
// 			prev = last;
// 			if (cur == last)
// 			{
// 				// this must not happen.
// 				printf("ERROR.\n");
// 				break;
// 			}
// 		}
// 		if ((len_l + len_r) % 2 == 1)
// 		{
// 			if (log_output)
// 				printf("    found an odd path.\n");
// 			odd_paths++;
// 		}
// 	}
// 	printf("N: %d, C: %d, I: %d\n", n, cycles, odd_paths);
// 	printf("d: %.1f\n", n - cycles - (odd_paths / 2.0));
// 	return (1);
// }

static int track_paths(Extremity_ctx *ctx, Adjacency **adj_A, Adjacency **adj_B, int log_output)
{
    int i, cycles = 0, odd_paths = 0, len_l, len_r;
    Adjacency *cur, *prev, *next, *last;

    if (log_output) printf("[DEBUG] track_paths: Traversing components\n");
    for (i = 0; i < ctx->n_adj_a; i++)
    {
        if (adj_A[i]->visited) continue;
        cur = adj_A[i];
        len_l = len_r = 0;
        cur->visited = 1;

        if (log_output) printf("  Traversing from adj_A[%d] (%d,%d)\n", i, get_value_ext(ctx, cur->left), get_value_ext(ctx, cur->right));

        // 左方向への探索
        prev = cur; next = cur->left_to; last = NULL;
        while (next) {
            len_l++;
            next->visited = 1;
            last = next;
            next = choose_l_r(ctx, next, prev);
            prev = last;
            if (cur == prev) { // サイクル検出
                if (cur->right_to) {
                    cycles++;
                    if (log_output) printf("    -> Cycle detected.\n");
                }
                break;
            }
        }
        if (cur == last) continue; // サイクルだった場合は右探索不要

        // 右方向への探索
        prev = cur; next = cur->right_to;
        while (next) {
            len_r++;
            next->visited = 1;
            last = next;
            next = choose_l_r(ctx, next, prev);
            prev = last;
        }

        // パスの長さ（エッジ数）が奇数なら odd path
        if ((len_l + len_r) % 2 == 1) {
            odd_paths++;
            if (log_output) printf("    -> Odd path detected (total len: %d).\n", len_l + len_r);
        }
    }
    printf("N: %d, C: %d, I: %d\nd: %.1f\n", ctx->n, cycles, odd_paths, ctx->n - cycles - (odd_paths / 2.0));
    return (1);
}

static int is_diff(Extremity_ctx *ctx, Extremity *a, Extremity *b)
{
	if (!is_nullish(ctx,a))
	{
		if (a != b)
			return (1);
		else
			return (0);
	}
	else
	{
		if (a == b)
			return (0);
		if (is_nullish(ctx, b))
			return (0);
	}
	return (1);
}

static void proceed(Extremity_ctx *ctx, int log_output)
{
    int c = 0, i;
    Extremity *src, *tar;
    int tar_id, tar_sign, tar_idx;

    if (log_output) printf("[DEBUG] proceed: Starting DCJ operations\n");

    // 1回目：通常の遺伝子端点に対するDCJ
    for (i = 0; i < (ctx->n) * 2; i++)
    {
        src = ctx->exts[i];
        tar_id = ctx->goals[i]->reality->id;
        tar_sign = (ctx->goals[i]->reality->sign == 1) ? 1 : 0;

        if (tar_id == 0) tar = ctx->ext_l;
        else if (tar_id == ctx->n + 1) tar = ctx->ext_r;
        else tar = ctx->exts[(tar_id - 1) * 2 + tar_sign];

        if (!is_nullish(ctx, tar) && is_diff(ctx, src->reality, tar)) {
            if (log_output) fprintf(ctx->fp, "DCJ(G): (%d,%d) (%d,%d) -> (%d,%d) (%d,%d)\n",
                src->id*src->sign, src->reality->id*src->reality->sign,
                tar->id*tar->sign, tar->reality->id*tar->reality->sign,
                src->id*src->sign, tar->id*tar->sign,
                src->reality->id*src->reality->sign, tar->reality->id*tar->reality->sign);
            double_cut_join(ctx, src, tar);
            c++;
        }
    }

    // 2回目：端点（キャップ）が関わるDCJ
    for (i = 0; i < (ctx->n) * 2; i++)
    {
        src = ctx->exts[i];
        tar_id = ctx->goals[i]->reality->id;
        tar_sign = (ctx->goals[i]->reality->sign == 1) ? 1 : 0;

        if (tar_id == 0) tar = ctx->ext_l;
        else if (tar_id == ctx->n + 1) tar = ctx->ext_r;
        else tar = ctx->exts[(tar_id - 1) * 2 + tar_sign];

        if (is_nullish(ctx, tar) && is_diff(ctx, src->reality, tar)) {
            if (log_output) fprintf(ctx->fp, "DCJ(C): (%d,%d) (%d,%d) -> (%d,%d) (%d,%d)\n",
                src->id*src->sign, src->reality->id*src->reality->sign,
                tar->id*tar->sign, tar->reality->id*tar->reality->sign,
                src->id*src->sign, tar->id*tar->sign,
                src->reality->id*src->reality->sign, tar->reality->id*tar->reality->sign);
            double_cut_join(ctx, src, tar);
            c++;
        }
    }
    if (log_output) printf("[DEBUG] proceed: Completed. Total DCJs: %d\n", c);
}

// static void proceed(Extremity_ctx *ctx, int log_output)
// {
// 	Extremity **exts;
// 	Extremity **goals;
// 	Extremity *src;
// 	Extremity *tar;
// 	int tar_id;
// 	int tar_sign;
// 	int c;
// 	int i;

// 	exts = ctx->exts;
// 	goals = ctx->goals;
// 	c = 0;
// 	for (i = 0; i < (ctx->n) * 2; i++)
// 	{
// 		src = exts[i];
// 		tar_id = goals[i]->reality->id;
// 		tar_sign = goals[i]->reality->sign;
// 		tar_sign = (tar_sign == 1) ? 1 : 0;
// 		if (tar_id == 0)
// 			tar = ctx->ext_l;
// 		else if (tar_id == ctx->n + 1)
// 			tar = ctx->ext_r;
// 		else
// 			tar = exts[(tar_id - 1) * 2 + tar_sign];
// 		if (!is_nullish(ctx, tar) && is_diff(ctx, src->reality, tar))
// 		{
// 			if (log_output)
// 			{
// 				int src_num = src->id * src->sign;
// 				int tar_num = tar->id * tar->sign;
// 				int src_pair_num = src->reality->id * src->reality->sign;
// 				int tar_pair_num = tar->reality->id * tar->reality->sign;
// 				fprintf(ctx->fp, "(%d, %d) (%d, %d) --DCJ--> (%d, %d) (%d, %d)\n",
// 					src_num, src_pair_num, tar_num, tar_pair_num,
// 					src_num, tar_num, src_pair_num, tar_pair_num);
// 			}
// 			double_cut_join(ctx, src, tar);
// 			c++;
// 		}
// 	}
// 	for (i = 0; i < (ctx->n) * 2; i++)
// 	{
// 		src = exts[i];
// 		tar_id = goals[i]->reality->id;
// 		tar_sign = goals[i]->reality->sign;
// 		tar_sign = (tar_sign == 1) ? 1 : 0;
// 		if (tar_id == 0)
// 			tar = ctx->ext_l;
// 		else if (tar_id == ctx->n + 1)
// 			tar = ctx->ext_r;
// 		else
// 			tar = exts[(tar_id - 1) * 2 + tar_sign];
// 		if (is_nullish(ctx, tar) && is_diff(ctx, src->reality, tar))
// 		{
// 			if (log_output)
// 			{
// 				int src_num = src->id * src->sign;
// 				int tar_num = tar->id * tar->sign;
// 				int src_pair_num = src->reality->id * src->reality->sign;
// 				int tar_pair_num = tar->reality->id * tar->reality->sign;
// 				fprintf(ctx->fp, "(%d, %d) (%d, %d) --DCJ--> (%d, %d) (%d, %d)\n",
// 					src_num, src_pair_num, tar_num, tar_pair_num,
// 					src_num, tar_num, src_pair_num, tar_pair_num);
// 			}
// 			double_cut_join(ctx, src, tar);
// 			c++;
// 		}
// 	}
// 	if (log_output)
// 		fprintf(ctx->fp, "number of dcjs: %d\n", c);
// }

int solve_dcj_mine(Extremity_ctx *ctx, int log_output)
{
    int *map_A, *map_B;
    Adjacency **adj_A, **adj_B;

    if (log_output) {
        ctx->fp = fopen("log", "w");
        if (!ctx->fp) return (-1);
    }

    map_A = (int *)xcalloc((2 * ctx->n), sizeof(int));
    map_B = (int *)xcalloc((2 * ctx->n), sizeof(int));

    printf("[DEBUG] Allocated maps. n_adj_a: %d, n_adj_b: %d\n", ctx->n_adj_a, ctx->n_adj_b);

    adj_A = (Adjacency **)xmalloc(sizeof(Adjacency *) * (ctx->n_adj_a));
    adj_B = (Adjacency **)xmalloc(sizeof(Adjacency *) * (ctx->n_adj_b));

    set_maps_adjs(ctx, map_A, map_B, adj_A, adj_B, log_output);
    put_adjs(ctx, map_A, map_B, adj_A, adj_B, log_output);
    track_paths(ctx, adj_A, adj_B, log_output);

    printf("[DEBUG] Proceeding to DCJ operations...\n");
    proceed(ctx, log_output);

    free_array_int(map_A, 2 * ctx->n);
    free_array_int(map_B, 2 * ctx->n);
    free_list_adj(adj_A, ctx->n_adj_a);
    free_list_adj(adj_B, ctx->n_adj_b);

    if (log_output) fclose(ctx->fp);
    return (1);
}
