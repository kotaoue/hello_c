# CMock モックテスト サンプル

CMock を使った現在時刻のモックテスト入門サンプルです。

## ディレクトリ構成

```
unit_test_with_mock/
├── src/
│   ├── clock.h / clock.c   # 依存関係: 現在時刻を取得する（モック対象）
│   └── greet.h / greet.c   # テスト対象: 時刻に応じた挨拶を返す
├── test/
│   └── test_clock_mock.c   # CMock によるモックテスト
├── mocks/
│   ├── Mockclock.h         # CMock が生成したモックヘッダ
│   └── Mockclock.c         # CMock が生成したモック実装
├── vendor/
│   ├── unity/              # Unity テストフレームワーク（CMock の依存）
│   └── cmock/              # CMock モック生成ライブラリ
├── scripts/
│   └── generate_mocks.rb   # モック生成スクリプト
└── Makefile
```

## CMock とは

[CMock](https://github.com/ThrowTheSwitch/CMock) はヘッダファイルを解析してモックを自動生成する C 向けのライブラリです。

`clock.h` を渡すと `Mockclock.h` / `Mockclock.c` を自動生成し、
テスト内で `get_current_hour()` の戻り値を任意の値に差し替えることができます。

## モックを使う理由

`get_greeting()` は内部で `get_current_hour()` を呼び出します。

```
get_greeting()
    └── get_current_hour()   ← time() で実時刻を取得
```

実時刻に依存したままでは「午前9時のテスト」を任意のタイミングで実行できません。
CMock で `get_current_hour()` をモックすると、戻り値を固定できます。

```c
get_current_hour_ExpectAndReturn(9);  // 実際の time() は呼ばれない
TEST_ASSERT_EQUAL_STRING("Good morning!", get_greeting());
```

## ビルドと実行

```sh
cd unit_test_with_mock

# テストをビルドして実行
make test

# clock.h を変更した場合にモックを再生成
make mocks

# ビルド成果物を削除
make clean
```

## 実行結果の例

```
==========================================
 CMock モックテスト
==========================================
test/test_clock_mock.c:43:test_get_greeting_returns_morning_when_hour_is_9:PASS
test/test_clock_mock.c:44:test_get_greeting_returns_afternoon_when_hour_is_15:PASS
test/test_clock_mock.c:45:test_get_greeting_returns_evening_when_hour_is_22:PASS

-----------------------
3 Tests 0 Failures 0 Ignored
OK
```
