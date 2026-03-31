/**
 * http_client.h — libcurl を使った HTTP クライアントの公開 API
 */

#pragma once

#include <stddef.h>

/**
 * HTTP レスポンスを表す構造体。
 * http_response_free() で解放すること。
 */
typedef struct {
    char  *body;        /**< NULL 終端のレスポンスボディ。失敗時は NULL。 */
    size_t body_len;    /**< ボディのバイト数。 */
    long   status_code; /**< HTTP ステータスコード (200, 404 など)。失敗時は -1。 */
} HttpResponse;

/**
 * HTTP GET リクエストを送信する。
 *
 * @param url  リクエスト先の URL
 * @return     HttpResponse へのポインタ。呼び出し元が http_response_free() で解放する。
 *             メモリ確保失敗時は NULL。
 */
HttpResponse *http_get(const char *url);

/**
 * HTTP POST リクエストを送信する。
 *
 * @param url          リクエスト先の URL
 * @param body         送信するボディ文字列 (例: JSON 文字列)
 * @param content_type Content-Type ヘッダの値 (例: "application/json")
 * @return             HttpResponse へのポインタ。呼び出し元が http_response_free() で解放する。
 *                     メモリ確保失敗時は NULL。
 */
HttpResponse *http_post(const char *url, const char *body, const char *content_type);

/**
 * HttpResponse を解放する。
 *
 * @param resp  http_get() / http_post() が返したポインタ。NULL を渡しても安全。
 */
void http_response_free(HttpResponse *resp);
