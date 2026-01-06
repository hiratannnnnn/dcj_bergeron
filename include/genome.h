#ifndef GENOME_H
# define GENOME_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <time.h>
// # include <sys/stat.h>
# include <errno.h>

# include "lib.h"

typedef struct Extremity
{
	int id;	// 1 ...
	int sign;		// 1 or -1
	struct Extremity *reality;	// A
	struct Extremity *desire;	// B
}	Extremity;

typedef struct Adjacency
{
	Extremity *left;
	Extremity *right;
	struct Adjacency *left_to;
	struct Adjacency *right_to;
	int visited;
}	Adjacency;

typedef struct Extremity_ctx
{
	Extremity **exts;
	Extremity *ext_l;
	Extremity *ext_r;
	Extremity **goals;
	Extremity *goal_l;
	Extremity *goal_r;
	int n;
	int *perm;
	int *sigma;
	int n_sent;
	int nb_sent;
	int n_adj_a;
	int n_adj_b;
	int *perm_sent;
	int *sigma_sent;
	int *pb_sent;
	int *sb_sent;
	FILE *fp;
}	Extremity_ctx;

// contexts.c
void		print_Extremity_ctx	(Extremity_ctx *ctx);
void		Extremity_free		(Extremity_ctx *ctx, int n);
void		put_desires			(Extremity **exts, Extremity *ext_l, Extremity *ext_r, int n);
void		put_realities		(Extremity_ctx *ctx, Extremity **exts, Extremity *ext_l, Extremity *ext_r, int *perm, int n);
Extremity 	**perm_to_exts		(int *perm, int n);
int 		Extremity_init		(char const *filename, Extremity_ctx *ctx, int *n);

// double_cut_join.c
void		double_cut_join		(Extremity_ctx *ctx, Extremity *src, Extremity *tar);


// examples.c
int 		write_perm_ext		(int *perm, int n, double cut_prob, char const *filename);
int 		make_example		(int n, char const *filename, double cut_prob);

// solve.c
int 		solve_dcj_mine		(Extremity_ctx *ctx, int log_output);

// utils.c
int 		is_all_true			(int *array, int n);
int 		is_nullish			(Extremity_ctx *ctx, Extremity *ext);
void		reverse_p			(Extremity_ctx *ctx, int *perm, int *sigma);

// utils_adj.c
Adjacency 	*create_adj_node	(Extremity *left, Extremity *right);
void		free_list_adj		(Adjacency **list, int n);

// utils_ext.c
void		free_ext			(Extremity *ext);
void 		free_array_exts		(Extremity **exts, int n);
Extremity 	*create_ext			(int id, int sign);
Extremity 	*get_pair_of		(Extremity_ctx *ctx, Extremity *ext);
Extremity 	*get_ext_by_perm_index		(Extremity_ctx *ctx, Extremity **exts, int index);
Extremity 	*get_ext_by_value			(Extremity_ctx *ctx, Extremity **exts, int value);
int 		get_value_ext		(Extremity_ctx *ctx, Extremity *ext);
void		cut_exts			(Extremity_ctx *ctx, Extremity *e1, Extremity *e2);

// utils_print.c
void 		print_array_exts	(Extremity **exts, int n);
void		print_perm_sent		(int *perm, int *sigma, int n);
void		chase_exts			(Extremity_ctx *ctx, Extremity *ext, int *used);
void		print_chromosomes	(Extremity_ctx *ctx, int n);

// utils_read.c
void		put_nums			(int *perm, char *line, int n);
void		cut_line			(Extremity_ctx *ctx, Extremity **exts, char const *line, int n, int *n_adj);
int		 	count_sentinels		(char const *line);
void		put_nums_sentinels	(int *perm, int *perm_sent, int *sigma_sent, char const *line, int n);
int 		read_first_line		(int fd, Extremity_ctx *ctx, int *n);
int 		read_second_line	(int fd, Extremity_ctx *ctx);
int 		read_third_line		(int fd, Extremity_ctx *ctx);

#endif
