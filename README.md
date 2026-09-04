# 🔧 pipex — shell のパイプラインを pipe / fork / dup2 / execve で再実装する

![language](https://img.shields.io/badge/language-C-blue.svg)
![norm](https://img.shields.io/badge/42-norminette-success.svg)
![flags](https://img.shields.io/badge/build-Wall%20Wextra%20Werror-brightgreen.svg)
![platform](https://img.shields.io/badge/platform-macOS%20%7C%20POSIX-lightgrey.svg)

> **TL;DR (EN):** A [42](https://42tokyo.jp/) project that reimplements the shell
> pipeline `< file1 cmd1 | cmd2 > file2` in C, using only `pipe`, `fork`, `dup2`,
> `execve`, `waitpid` and `open`. The bonus extends it to an arbitrary number of
> commands and to `here_doc <LIMITER>`. PATH is resolved by hand: split `$PATH`
> on `:` and probe each directory with `access(F_OK)`. 308 lines across 5 source
> files (plus a vendored libft), norminette-clean, `-Wall -Wextra -Werror`.
> Known limits — including a pipe-buffer deadlock on large inputs — are
> documented below rather than hidden.

本リポジトリは 42 Tokyo の課題 **pipex** の提出物です。`< infile cmd1 | cmd2 > outfile`
というシェルの1行を、シェルを使わずに `pipe(2)` / `fork(2)` / `dup2(2)` / `execve(2)`
だけで組み立て直します。ボーナスでは任意個数のコマンドの連結と `here_doc` に対応しました。
学習成果物なので、実装できている範囲と、実装しきれていない制限の両方をこの README に書いています。

---

## ビルド

```sh
git clone https://github.com/Kizuna42/pipex.git
cd pipex
make          # -> ./pipex        （mandatory）
make bonus    # -> ./pipex_bonus  （bonus）
```

`libft/libft.a` は `all` / `bonus` の前提条件として `make -C libft` で自動ビルドされます。
他のターゲットは `clean` / `fclean` / `re`（いずれも libft に再帰）。`test` ターゲットはありません。

`CFLAGS = -Wall -Wextra -Werror -fsanitize=address`。AddressSanitizer は開発時の既定として常時
有効にしています（macOS の ASan には LeakSanitizer が含まれないため、リーク検査は行っていません）。
`norminette src/` は 5 ファイルすべて `OK!` です。

---

## 使い方

コマンドは **PATH 上の名前で** 指定します（`cat`、`grep ap` など）。絶対パス指定は動きません
（「既知の制限」2 を参照）。

```console
$ printf 'apple\nbanana\ncherry\napricot\n' > infile

# mandatory: < infile grep ap | wc -l > outfile と等価
$ ./pipex infile "grep ap" "wc -l" outfile
$ cat outfile
       2

# bonus: 3個以上のコマンド
$ ./pipex_bonus infile "grep a" "tr a-z A-Z" "sort" out_bonus
$ cat out_bonus
APPLE
APRICOT
BANANA

# bonus: here_doc（出力ファイルは O_APPEND で開かれる）
$ printf 'one\ntwo\nEOF\n' | ./pipex_bonus here_doc EOF "cat -e" "wc -l" out_hd
$ cat out_hd
       2

# 引数の数が合わない場合
$ ./pipex one two
Error: Bad arguments
Ex: ./pipex <file1> <cmd1> <cmd2> <file2>

# 存在しないコマンド（perror で報告し、後段は空入力のまま続行する）
$ ./pipex infile "nosuchcmd" "wc -l" out
Error: No such file or directory
$ cat out
       0
```

---

## 実装のポイント

### 最終コマンドを fork せず、メインプロセス自身が execve する

mandatory の `main()` は `fork` を1回しか呼びません。子プロセスが `cmd1` を、
親プロセス自身が `waitpid` のあとに `cmd2` を `execve` してプロセスイメージごと置き換わります。
bonus も同じ考え方で、`while (i < argc - 2)` の中で中間コマンドの数だけ fork し、
最後の `argv[argc - 2]` は `main` が自分で `execve` します。
この結果 `pipex` の終了ステータスは最終コマンドのものになり、bash の `cmd1 | cmd2` と一致します。

### パイプは1本ずつ作って使い捨て、状態は fd テーブルが持つ

bonus はコマンド数分の pipe を配列で先に確保しません。`child_process()` の中で pipe を1本作り、
子は `dup2(fd[1], STDOUT)`、親は `close(fd[1])` → `dup2(fd[0], STDIN)` として
「次段の入力」を自分の標準入力に固定し、ループの次の反復へ進みます。
構造体は1つも定義していません。パイプラインの状態は **プロセスの fd テーブル**（STDIN / STDOUT の
付け替え）と `argv` のインデックス `i` だけで表現されています。

### PATH 解決と open モードの集約

`find_path()` は `envp` を線形走査して `PATH` 行を見つけ、`+ 5` で値部分を取り、`ft_split(':')` で
分割します。各ディレクトリに `"/" + cmd` を連結して `access(path, F_OK)` を試し、最初に見つかった
ものを返します。ディレクトリ数 d に対して最大 d 回の `access` システムコールです。

ファイルオープンは、42 の Norm（1関数25行、1ファイル5関数まで）の下で分岐を散らかさないよう
`open_file(char *argv, int i)` の `i` に集約しました。`0` = `O_WRONLY|O_CREAT|O_APPEND`（here_doc）、
`1` = `O_WRONLY|O_CREAT|O_TRUNC`（通常出力）、`2` = `O_RDONLY`（入力）。mandatory と bonus は
`src/utils.c`（`find_path` / `error` / `execute`）を共有し、bonus 固有の `open_file` / `usage` だけを
`utils_bonus.c` に分けています。

### メモリ解放の方針

`execve` が成功すればプロセスイメージごと置き換わるので、成功パスでは `ft_split` の結果を解放しません。
逆に `find_path()` が `NULL` を返す失敗パスでは、`execute()` が `cmd` 配列を明示的に free してから
`error()` を呼びます。**「`execve` に到達しない経路だけ解放する」** という割り切りです。

---

## 既知の制限

学習成果物として、実測で確認できた制限をそのまま残しています。

1. **大きな入力でデッドロックする（最も重い制限）**
   mandatory / bonus とも「先に `waitpid` で前段の終了を待ってから、自プロセスが後段のコマンドに
   `execve` する」逐次構造です。そのため前段の出力がパイプバッファ（64KB）を超えると、
   読む側が誰もいない状態で `write` がブロックして停止します。
   実測: 740,000 バイトのファイルに対して `timeout 5 ./pipex big.txt cat "head -1" out` が
   exit 124（`./pipex_bonus` も同じ）。小さいファイルでは正常終了します。
   正しくは全プロセスを先に fork してから一括で待つべきですが、**その修正は入れていません。**
2. **コマンドは PATH 上の名前でしか解決できない**
   `find_path()` が無条件に `dir + "/" + cmd` を連結するため、`/bin/cat` は
   `/usr/bin//bin/cat` になり `access` に失敗します。
3. **`PATH` が存在しない環境でクラッシュする**
   `while (ft_strnstr(envp[i], "PATH", 4) == 0) i++;` に `envp[i]` の NULL チェックがなく、
   `env -i ./pipex ...` で `ft_strnstr` 内 SEGV になります。
4. **`here_doc` の LIMITER は前方一致**
   `ft_strncmp(line, limiter, ft_strlen(limiter))` で判定しているため、LIMITER が `EOF` のとき
   `EOFX` の行も終端として扱われます。
5. **引数エラーでも終了ステータスは 0**
   bonus の `usage()` が `exit(EXIT_SUCCESS)` を呼ぶためです。

---

## テスト

第三者製のテスター [vfurmane/pipex-tester](https://github.com/vfurmane/pipex-tester) で
確認しました。**42 の公式評価結果ではありません。**
実測結果は **30 OK / 1 TO**。唯一の TO は `/dev/urandom | cat | head -1` のケースで、
原因は上記「既知の制限」1 のデッドロックと同一です。

このリポジトリの `pipex-tester` は `.gitmodules` を伴わない gitlink なので、`git clone` しても
中身は空になります。実行するにはテスターを別途 clone し、`config.vars` の `PROJECT_DIRECTORY` を
このリポジトリに向けてください。

---

## プロジェクト構成

```
.
├── Makefile
├── src/
│   ├── pipex.h          (33 行)  共有ヘッダ
│   ├── pipex.c          (64 行)  mandatory
│   ├── utils.c          (68 行)  find_path / error / execute（共有）
│   ├── pipex_bonus.c   (105 行)  bonus（N コマンド連結・here_doc）
│   └── utils_bonus.c    (38 行)  open_file / usage
├── libft/                        自作 libft を vendoring（統計には含めない）
└── pipex-tester/                 第三者テスターの gitlink
```

`src/` 配下のみで **5 ファイル / 308 行 / 12 関数**（うち `main` が 2つ）。libft からは
`ft_split` / `ft_strnstr` / `ft_strjoin` / `ft_strncmp` / `ft_strlen` / `ft_putstr_fd` /
`get_next_line` を使っています。

---

## 学んだこと

- **`execve` はプロセスイメージを置き換える**ので、その先にメモリの持ち主はいない。
  「誰がいつ free するか」はコード上の位置ではなく、`execve` に到達するかどうかで決まる。
  この視点がないと `execute()` の非対称な free は書けなかった。
- **fd テーブルは fork をまたいで継承される状態そのもの**である。bonus で配列も構造体も持たずに
  N 段のパイプラインが組めるのは、`dup2` で書き換えた STDIN が次の反復に引き継がれるため。
- **`waitpid` を置く位置がパイプラインの並行性を決める**。前段を待ってから後段を起動する構造は、
  小さい入力では正しく動くのに、パイプバッファを超えた瞬間にデッドロックする。
  「テストが通る」ことと「構造が正しい」ことは別だと、この課題で実測して理解した。
  この経験は rank03 の [minishell](https://github.com/Kizuna42/minishell) に繋がっている。

---

*This project has been created as part of the 42 curriculum by kizuna.*
