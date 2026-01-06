#include "genome.h"

Adjacency *create_adj_node(Extremity *left, Extremity *right)
{
	Adjacency *tmp;

	tmp = (Adjacency *)xmalloc(sizeof(Adjacency) * 1);
	tmp->left = left;
	tmp->right = right;
	tmp->left_to = NULL;
	tmp->right_to = NULL;
	tmp->visited = 0;
	return (tmp);
}

void	free_list_adj(Adjacency **list, int n)
{
	int i;

	i = 0;
	while (i < n)
		xfree(list[i++], sizeof(Adjacency));
	xfree(list, sizeof(Adjacency *) * n);
}
