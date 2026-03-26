/**
 * main.c — カスタムメモリアロケータ のデモ
 *
 * ファーストフィット・ベストフィット・メモリリーク検出の
 * 動作を確認するサンプルプログラム。
 */

#include "allocator.h"

#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* デモ関数                                                              */
/* ------------------------------------------------------------------ */

/** ファーストフィットの基本的な確保と解放 */
static void demo_first_fit(void)
{
    printf("\n--- ファーストフィット ---\n");

    allocator_init();

    void *a = my_malloc(64);
    void *b = my_malloc(128);
    void *c = my_malloc(64);

    printf("確保後:\n");
    allocator_dump();

    /* b を解放してから新しい 64 バイトを確保すると、
     * ファーストフィットでは b の場所（128 バイトブロック）が選ばれ、
     * 残りが新たな空きブロックとして分割される。                       */
    my_free(b);
    printf("b 解放後:\n");
    allocator_dump();

    void *d = my_malloc(64);
    printf("64 バイトを再確保後 (d=%p):\n", d);
    allocator_dump();

    my_free(a);
    my_free(c);
    my_free(d);

    printf("全解放後:\n");
    allocator_dump();
    allocator_check_leaks();
}

/** ベストフィットの動作確認 */
static void demo_best_fit(void)
{
    printf("\n--- ベストフィット ---\n");

    allocator_init();

    /* 異なるサイズのブロックを確保・解放してフラグメンテーションを作る */
    void *a = my_malloc_best_fit(32);
    void *b = my_malloc_best_fit(256);
    void *c = my_malloc_best_fit(64);
    void *d = my_malloc_best_fit(128);

    my_free(a); /* 32 バイトの空き */
    my_free(c); /* 64 バイトの空き */

    printf("フラグメンテーション状態:\n");
    allocator_dump();

    /* ベストフィットでは 64 バイトを要求すると
     * 32 バイトブロックではなく 64 バイトブロックが選ばれる            */
    void *e = my_malloc_best_fit(48);
    printf("48 バイト確保後 (e=%p):\n", e);
    allocator_dump();

    my_free(b);
    my_free(d);
    my_free(e);

    printf("全解放後:\n");
    allocator_dump();
    allocator_check_leaks();
}

/** メモリリーク検出のデモ */
static void demo_leak_detection(void)
{
    printf("\n--- メモリリーク検出 ---\n");

    allocator_init();

    void *x = my_malloc(100);
    void *y = my_malloc(200);
    (void)x; /* 意図的に解放しない（リーク） */

    my_free(y); /* y だけ解放 */

    printf("x を解放しなかった場合:\n");
    allocator_check_leaks(); /* x のリークが報告される */
}

/** 文字列をユーザー領域に書き込む確認 */
static void demo_string(void)
{
    printf("\n--- 文字列書き込み ---\n");

    allocator_init();

    const char *msg = "Hello from custom allocator!";
    size_t len = strlen(msg) + 1;

    char *buf = (char *)my_malloc(len);
    if (buf == NULL) {
        fprintf(stderr, "確保失敗\n");
        return;
    }

    memcpy(buf, msg, len);
    printf("書き込んだ文字列: %s\n", buf);

    my_free(buf);
    allocator_check_leaks();
}

/* ------------------------------------------------------------------ */
/* エントリポイント                                                       */
/* ------------------------------------------------------------------ */

int main(void)
{
    demo_first_fit();
    demo_best_fit();
    demo_leak_detection();
    demo_string();
    return 0;
}
