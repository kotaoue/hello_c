/**
 * allocator.c — カスタムメモリアロケータ の実装
 *
 * 固定サイズの静的配列をメモリプールとして使い、
 * ファーストフィット・ベストフィット・コアレッシングを実装する。
 */

#include "allocator.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* 内部データ                                                            */
/* ------------------------------------------------------------------ */

/** メモリプール本体（静的配列） */
static uint8_t pool[POOL_SIZE];

/**
 * ブロックリストの先頭ポインタ。
 * allocator_init() 呼び出し後は pool の先頭を指す。
 */
static BlockHeader *head = NULL;

/* ------------------------------------------------------------------ */
/* 内部ヘルパー                                                          */
/* ------------------------------------------------------------------ */

/**
 * split_block — ブロックが要求サイズより大きい場合に二分割する。
 *
 * 分割後:
 *   [block: size=size] → [remainder: size=残り-sizeof(BlockHeader)]
 *
 * ブロックが小さすぎて分割できない場合は何もしない。
 *
 * @param block  分割対象のブロック
 * @param size   必要なユーザーデータサイズ
 */
static void split_block(BlockHeader *block, size_t size)
{
    /* 分割後の残りが BlockHeader + 最低 1 バイト以上必要 */
    if (block->size <= size + sizeof(BlockHeader)) {
        return;
    }

    /* 新しい空きブロックをブロックの後半に配置 */
    BlockHeader *remainder =
        (BlockHeader *)((uint8_t *)(block + 1) + size);
    remainder->size   = block->size - size - sizeof(BlockHeader);
    remainder->is_free = true;
    remainder->next   = block->next;

    block->size = size;
    block->next = remainder;
}

/**
 * coalesce — 空きブロックを連結する（フラグメンテーション解消）。
 *
 * block が空きで、次のブロックも空きであれば二つを一つに結合する。
 * リスト全体を走査するので、my_free() の後に呼ぶ。
 */
static void coalesce(void)
{
    BlockHeader *cur = head;
    while (cur != NULL && cur->next != NULL) {
        if (cur->is_free && cur->next->is_free) {
            /* 二つのブロックを一つに結合 */
            cur->size += sizeof(BlockHeader) + cur->next->size;
            cur->next  = cur->next->next;
            /* 同じ cur で再チェック（三連続空きの場合に対応） */
        } else {
            cur = cur->next;
        }
    }
}

/* ------------------------------------------------------------------ */
/* 公開 API                                                              */
/* ------------------------------------------------------------------ */

void allocator_init(void)
{
    memset(pool, 0, sizeof(pool));

    head           = (BlockHeader *)pool;
    head->size     = POOL_SIZE - sizeof(BlockHeader);
    head->is_free  = true;
    head->next     = NULL;
}

/* ---------- ファーストフィット ---------------------------------------- */

void *my_malloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    BlockHeader *cur = head;
    while (cur != NULL) {
        if (cur->is_free && cur->size >= size) {
            split_block(cur, size);
            cur->is_free = false;
            return (void *)(cur + 1); /* ヘッダの直後がユーザー領域 */
        }
        cur = cur->next;
    }

    /* 十分な空きブロックが見つからなかった */
    return NULL;
}

void my_free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }

    /* ポインタの直前にあるヘッダを取得 */
    BlockHeader *block = (BlockHeader *)ptr - 1;

    /* 二重解放を検出（リリースビルドでも動作する明示的なチェック） */
    if (block->is_free) {
        fprintf(stderr,
                "[ERROR] double-free detected: addr=%p\n",
                ptr);
        return;
    }

    block->is_free = true;

    /* 隣接する空きブロックを結合してフラグメンテーションを抑える */
    coalesce();
}

/* ---------- ベストフィット -------------------------------------------- */

void *my_malloc_best_fit(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    BlockHeader *best = NULL;
    BlockHeader *cur  = head;

    /* リスト全体を走査して最小の適合ブロックを探す */
    while (cur != NULL) {
        if (cur->is_free && cur->size >= size) {
            if (best == NULL || cur->size < best->size) {
                best = cur;
            }
        }
        cur = cur->next;
    }

    if (best == NULL) {
        return NULL;
    }

    split_block(best, size);
    best->is_free = false;
    return (void *)(best + 1);
}

/* ---------- デバッグユーティリティ --------------------------------------- */

void allocator_dump(void)
{
    printf("=== allocator dump ===\n");
    BlockHeader *cur = head;
    size_t index = 0;
    while (cur != NULL) {
        printf("  [%zu] addr=%p  size=%-6zu  %s\n",
               index,
               (void *)(cur + 1),
               cur->size,
               cur->is_free ? "FREE" : "USED");
        cur = cur->next;
        index++;
    }
    printf("======================\n");
}

size_t allocator_check_leaks(void)
{
    size_t leaks = 0;
    BlockHeader *cur = head;
    while (cur != NULL) {
        if (!cur->is_free) {
            fprintf(stderr,
                    "[LEAK] addr=%p  size=%zu\n",
                    (void *)(cur + 1),
                    cur->size);
            leaks++;
        }
        cur = cur->next;
    }
    if (leaks == 0) {
        printf("[OK] No memory leaks detected.\n");
    } else {
        fprintf(stderr, "[LEAK] %zu block(s) were not freed.\n", leaks);
    }
    return leaks;
}
