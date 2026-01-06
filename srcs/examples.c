#include "genome.h"

/**
 * @param perm      : 1 ~ n, signed
 * @param n         : number of genes
 * @param cut_prob  : probability of cutting each adj.
 * @param filename  : filename
 */
int write_perm_ext(int *perm, int n, double cut_prob, char const *filename)
{
    FILE *fp;
    int i, start;
    int is_circular;

    if (!perm || n <= 0)
        return (-2);
    fp = fopen(filename, "w");
    if (!fp)
        return (printf("Failed to open file %s\n", filename), -1);

    fprintf(fp, "%d\n", n);

    // 1行目: perm 配列の内容をランダムな染色体に分割して出力
    start = 0;
    while (start < n)
    {
        is_circular = rand() % 2; // 0: 線形, 1: 円形
        fprintf(fp, "%c ", is_circular ? '(' : '<');

        for (i = start; i < n; i++)
        {
            fprintf(fp, "%d ", perm[i]);
            // 次の遺伝子との間で「切断（染色体の終わり）」を行うか判定
            // ただし最後の遺伝子の場合は必ず終了させる
            if (i == n - 1 || (double)rand() / RAND_MAX < cut_prob)
            {
                fprintf(fp, "%c", is_circular ? ')' : '>');
                start = i + 1;
                break;
            }
        }
        if (start < n) fprintf(fp, " ");
    }
    fprintf(fp, "\n");

    // 2行目: 恒等置換 (1 2 ... n) をランダムな染色体に分割して出力
    start = 0;
    while (start < n)
    {
        is_circular = rand() % 2;
        fprintf(fp, "%c ", is_circular ? '(' : '<');

        for (i = start; i < n; i++)
        {
            fprintf(fp, "%d ", i + 1);
            if (i == n - 1 || (double)rand() / RAND_MAX < cut_prob)
            {
                fprintf(fp, "%c", is_circular ? ')' : '>');
                start = i + 1;
                break;
            }
        }
        if (start < n) fprintf(fp, " ");
    }
    fprintf(fp, "\n");

    fclose(fp);
    return (0);
}

/**
 * @param n         : number of genes
 * @param filename  : filename
 * @param cut_prob  : probability of cutting each adj.
 */
int make_example(int n, char const *filename, double cut_prob)
{
    int *perm;

    perm = gen_signed_perm(n);
    write_perm_ext(perm, n, cut_prob, filename);
    // print_array_int(perm, n, 4);
	free_array_int(perm, n);
    return (1);
}
