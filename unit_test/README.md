# unit_test

モックを使わないシンプルなCのユニットテストのサンプルです。

## テスト対象の関数

| 関数 | 説明 |
| ---- | ---- |
| `is_odd(int n)` | `n` が奇数なら 1、偶数なら 0 を返す |
| `mod(int a, int b)` | `a % b` の余りを返す |

## ビルドとテストの実行

```sh
make test
```

## 実行結果の例

```sh
=== is_odd tests ===
[PASS] is_odd(1) == 1
[PASS] is_odd(2) == 0
[PASS] is_odd(3) == 1
[PASS] is_odd(0) == 0
[PASS] is_odd(-1) == 1
[PASS] is_odd(-2) == 0

=== mod tests ===
[PASS] mod(10, 3) == 1
[PASS] mod(9, 3) == 0
[PASS] mod(7, 2) == 1
[PASS] mod(0, 5) == 0
[PASS] mod(1, 1) == 0
[PASS] mod(5, 0) == 0

Result: 12 passed, 0 failed
```

## クリーンアップ

```sh
make clean
```
