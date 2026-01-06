#include "genome.h"

size_t      mem = 0;
size_t      mem_peak = 0;
clock_t     proc_start;
clock_t     proc_end;

/**
 * Usage: ./solve [input_file]
 */
int main(int argc, char **argv)
{
    Extremity_ctx ctx;
    int n;

    // 引数チェック: プログラム名 + ファイルパスの計2つが必要
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file_path>\n", argv[0]);
        fprintf(stderr, "Example: %s tests/ex_100.txt\n", argv[0]);
        return (1);
    }

    // バッファリング無効化（デバッグログのリアルタイム表示用）
    setvbuf(stdout, NULL, _IONBF, 0);

    // 処理開始時間の記録
    proc_start = clock();

    // ファイル読み込みと初期化
    if (Extremity_init(argv[1], &ctx, &n) == -1)
    {
        fprintf(stderr, "Error: Could not read or parse file '%s'.\n", argv[1]);
        return (1);
    }

    // 読み込んだゲノム情報の表示（必要に応じて）
    print_Extremity_ctx(&ctx);

    // DCJ距離計算とパス探索の実行
    // 第2引数を1にすると詳細なデバッグログを出力
    if (solve_dcj_mine(&ctx, 0) == -1)
    {
        fprintf(stderr, "Error: DCJ solver encountered a critical error.\n");
        // エラー時もメモリ解放が必要な場合はここで行う
    }

    // メモリ解放
    Extremity_free(&ctx, n);
    if (ctx.perm)
        free_array_int(ctx.perm, n);

    // 実行時間やメモリ使用量の統計表示
    print_info();

    return (0);
}
