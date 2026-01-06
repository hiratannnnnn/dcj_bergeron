#include "genome.h"

void	double_cut_join(Extremity_ctx *ctx, Extremity *src, Extremity *tar)
{
	Extremity *e1, *e2, *tmp1, *tmp2;

	if (is_nullish(ctx, tar))
	{
		if (src->sign == -1)
		{
			e1 = src;
			e2 = src->reality;
			e1->reality = ctx->ext_l;
			e2->reality = ctx->ext_r;
		}
		else
		{
			e1 = src;
			e2 = src->reality;
			e1->reality = ctx->ext_r;
			e2->reality = ctx->ext_l;
		}
		return ;
	}
	else
	{
		tmp1 = src->reality;
		src->reality = tar;
		tmp1->reality = tar->reality;
		tmp2 = tar->reality;
		tar->reality = src;
		tmp2->reality = tmp1;
	}
}
