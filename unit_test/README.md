# Unit Test サンプル

Unity と CMock を使ったユニットテストの入門サンプルです。

## ディレクトリ構成

```
unit_test/
├── src/
│   ├── greet.h / greet.c   # テスト対象: 時刻に応じた挨拶を返す
│   ├── clock.h / clock.c   # 依存関係: 現在時刻を取得する
├── test/
│   ├── test_greet.c        # テスト 1: Unity による単体テスト
│   └── test_clock_mock.c   # テスト 2: CMock によるモックテスト
├── mocks/
│   ├── Mockclock.h         # CMock が生成したモックヘッダ
│   └── Mockclock.c         # CMock が生成したモック実装
├── vendor/
│   ├── unity/              # Unity テストフレームワーク
│   └── cmock/              # CMock モック生成ライブラリ
├── scripts/
│   └── generate_mocks.rb   # CMock によるモック生成スクリプト
└── Makefile
```

## テストの概要

### テスト 1: Unity を使った単体テスト（`test/test_greet.c`）

[Unity](https://github.com/ThrowTheSwitch/Unity) は C 向けの軽量なユニットテストフレームワークです。

`get_greeting_by_hour(int hour)` は引数として時刻を受け取るため、外部依存がありません。
モックなしでシンプルにテストできます。

```c
void test_get_greeting_by_hour_morning(void) {
    TEST_ASSERT_EQUAL_STRING("Good morning!", get_greeting_by_hour(6));
}
```

### テスト 2: CMock を使ったモックテスト（`test/test_clock_mock.c`）

[CMock](https://github.com/ThrowTheSwitch/CMock) はヘッダファイルからモックを自動生成するツールです。

`get_greeting()` は内部で `get_current_hour()` を呼び出して現在時刻を取得します。
実際の時刻に依存しないよう、CMock が生成した `Mockclock` を使って
`get_current_hour()` の戻り値を固定します。

```c
void test_get_greeting_returns_morning_when_hour_is_9(void) {
    get_current_hour_ExpectAndReturn(9);  // get_current_hour() が 9 を返すよう指定
    TEST_ASSERT_EQUAL_STRING("Good morning!", get_greeting());
}
```

## 前提条件

- `gcc`
- `make`
- `ruby`（モック再生成時のみ）

## ビルドと実行

```sh
cd unit_test

# テストをビルドして実行
make test

# モックファイルを再生成する場合（clock.h を変更したとき）
make mocks

# ビルド成果物を削除
make clean
```

## 実行結果の例

```
==========================================
 テスト 1: Unity 単体テスト
==========================================
test/test_greet.c:32:test_get_greeting_by_hour_morning:PASS
test/test_greet.c:33:test_get_greeting_by_hour_afternoon:PASS
test/test_greet.c:34:test_get_greeting_by_hour_evening:PASS

-----------------------
3 Tests 0 Failures 0 Ignored 
OK

==========================================
 テスト 2: CMock モックテスト
==========================================
test/test_clock_mock.c:43:test_get_greeting_returns_morning_when_hour_is_9:PASS
test/test_clock_mock.c:44:test_get_greeting_returns_afternoon_when_hour_is_15:PASS
test/test_clock_mock.c:45:test_get_greeting_returns_evening_when_hour_is_22:PASS

-----------------------
3 Tests 0 Failures 0 Ignored 
OK
```
