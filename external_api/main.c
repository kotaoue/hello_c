/**
 * main.c — 外部 API 接続サンプル
 *
 * httpbin.org を使って HTTP GET / POST リクエストを実際に送信し、
 * レスポンスをターミナルに表示する。
 *
 * httpbin.org は HTTP リクエストをそのままエコーバックする
 * テスト用の公開 API サービス。
 *   GET  https://httpbin.org/get   → リクエスト情報を JSON で返す
 *   POST https://httpbin.org/post  → 送信したボディを JSON で返す
 */

#include "http_client.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* デモ関数                                                              */
/* ------------------------------------------------------------------ */

/** GET リクエストのデモ */
static void demo_get(void)
{
    const char *url = "https://httpbin.org/get";

    printf("\n--- GET リクエスト ---\n");
    printf("URL: %s\n", url);

    HttpResponse *resp = http_get(url);
    if (resp == NULL) {
        fprintf(stderr, "メモリ確保に失敗しました\n");
        return;
    }

    printf("ステータスコード: %ld\n", resp->status_code);
    printf("レスポンスボディ:\n%s\n", resp->body != NULL ? resp->body : "(empty)");

    http_response_free(resp);
}

/** POST リクエストのデモ */
static void demo_post(void)
{
    const char *url  = "https://httpbin.org/post";
    const char *body = "{\"name\": \"hello_c\", \"version\": 1}";

    printf("\n--- POST リクエスト ---\n");
    printf("URL:              %s\n", url);
    printf("リクエストボディ: %s\n", body);

    HttpResponse *resp = http_post(url, body, "application/json");
    if (resp == NULL) {
        fprintf(stderr, "メモリ確保に失敗しました\n");
        return;
    }

    printf("ステータスコード: %ld\n", resp->status_code);
    printf("レスポンスボディ:\n%s\n", resp->body != NULL ? resp->body : "(empty)");

    http_response_free(resp);
}

/* ------------------------------------------------------------------ */
/* エントリポイント                                                       */
/* ------------------------------------------------------------------ */

int main(void)
{
    /* libcurl のグローバル初期化（プログラム全体で一度だけ呼ぶ） */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    demo_get();
    demo_post();

    /* グローバルリソースの解放 */
    curl_global_cleanup();

    return 0;
}
