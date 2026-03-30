/**
 * http_client.c — libcurl を使った HTTP GET / POST の実装
 *
 * 内部の write_callback() でレスポンスボディを動的バッファに蓄積し、
 * HttpResponse 構造体に詰めて返す。
 */

#include "http_client.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* 内部型                                                               */
/* ------------------------------------------------------------------ */

/** レスポンスボディを受け取る動的バッファ */
typedef struct {
    char  *data; /**< 蓄積データ (NULL 終端) */
    size_t len;  /**< データのバイト数 */
} Buffer;

/* ------------------------------------------------------------------ */
/* 内部関数                                                             */
/* ------------------------------------------------------------------ */

/**
 * libcurl の CURLOPT_WRITEFUNCTION に渡すコールバック。
 * チャンクを受け取るたびに Buffer を realloc して追記する。
 */
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t  total = size * nmemb;
    Buffer *buf   = (Buffer *)userdata;

    char *tmp = realloc(buf->data, buf->len + total + 1);
    if (tmp == NULL) {
        return 0; /* 0 を返すと CURLE_WRITE_ERROR になる */
    }

    buf->data = tmp;
    memcpy(buf->data + buf->len, ptr, total);
    buf->len += total;
    buf->data[buf->len] = '\0';

    return total;
}

/**
 * 設定済みの CURL ハンドルでリクエストを実行し、HttpResponse を返す。
 * 呼び出し側が curl_easy_cleanup() を行うこと。
 */
static HttpResponse *perform_request(CURL *curl)
{
    Buffer buf = {NULL, 0};

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &buf);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  /* リダイレクト追従 */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,        10L); /* タイムアウト 10 秒 */

    HttpResponse *resp = calloc(1, sizeof(HttpResponse));
    if (resp == NULL) {
        free(buf.data);
        return NULL;
    }

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl error: %s\n", curl_easy_strerror(res));
        free(buf.data);
        resp->status_code = -1;
        return resp;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp->status_code);
    resp->body     = buf.data;
    resp->body_len = buf.len;

    return resp;
}

/* ------------------------------------------------------------------ */
/* 公開 API                                                             */
/* ------------------------------------------------------------------ */

HttpResponse *http_get(const char *url)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        return NULL;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);

    HttpResponse *resp = perform_request(curl);
    curl_easy_cleanup(curl);
    return resp;
}

HttpResponse *http_post(const char *url, const char *body, const char *content_type)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        return NULL;
    }

    struct curl_slist *headers = NULL;
    if (content_type != NULL) {
        /* "Content-Type: " は 15 バイト、終端 '\0' で合計 16 バイト分を加算する */
        size_t header_len = strlen("Content-Type: ") + strlen(content_type) + 1;
        char  *header     = malloc(header_len);
        if (header == NULL) {
            curl_easy_cleanup(curl);
            return NULL;
        }
        snprintf(header, header_len, "Content-Type: %s", content_type);
        headers = curl_slist_append(headers, header);
        free(header);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    curl_easy_setopt(curl, CURLOPT_URL,        url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,  body);

    HttpResponse *resp = perform_request(curl);

    if (headers != NULL) {
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(curl);
    return resp;
}

void http_response_free(HttpResponse *resp)
{
    if (resp == NULL) {
        return;
    }
    free(resp->body);
    free(resp);
}
