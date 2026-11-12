#!/usr/bin/env bash
# pipex / pipex_bonus の挙動をシェルのパイプラインと比較する簡易テスト。
# 使い方: make && make bonus && ./tests/run_tests.sh
set -u

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
PIPEX="$ROOT/pipex"
BONUS="$ROOT/pipex_bonus"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT
cd "$WORK" || exit 1

# ASan のリーク検出は fork/exec を多用する本テストでは誤検知の原因になるため無効化する
export ASAN_OPTIONS="detect_leaks=0"

PASS=0
FAIL=0

ok() { PASS=$((PASS + 1)); printf '  \033[32mOK\033[0m   %s\n' "$1"; }
ng() { FAIL=$((FAIL + 1)); printf '  \033[31mFAIL\033[0m %s\n' "$1"; }

# expect_eq <name> <expected> <actual>
now_ms() { date +%s%3N; }

expect_eq() {
	if [ "$2" = "$3" ]; then ok "$1"; else ng "$1 (expected '$2', got '$3')"; fi
}

printf 'line1\nhello world\nfoo bar\nhello again\n' > infile
head -c 300000 /dev/zero | tr '\0' 'a' | fold -w 100 > bigfile

echo "== mandatory"

"$PIPEX" infile "grep hello" "wc -l" out; rc=$?
expect_eq "basic: grep | wc -l" "$(< infile grep hello | wc -l)" "$(cat out)"
expect_eq "basic: exit status 0" 0 "$rc"

timeout 10 "$PIPEX" bigfile cat "wc -c" out; rc=$?
expect_eq "large output (> pipe buffer) does not deadlock" 0 "$rc"
expect_eq "large output: byte count" "$(wc -c < bigfile)" "$(cat out)"

"$PIPEX" infile /bin/cat "/usr/bin/env wc -l" out; rc=$?
expect_eq "absolute path commands" "4" "$(tr -d ' ' < out)"
expect_eq "absolute path: exit status 0" 0 "$rc"

"$PIPEX" infile cat nosuchcmd_xyz out 2>/dev/null; rc=$?
expect_eq "last command not found -> 127" 127 "$rc"

"$PIPEX" infile nosuchcmd_xyz "wc -l" out 2>/dev/null; rc=$?
expect_eq "first command not found: last status kept" 0 "$rc"
expect_eq "first command not found: empty input" "0" "$(tr -d ' ' < out)"

"$PIPEX" infile cat "" out 2>/dev/null; rc=$?
expect_eq "empty command -> 127 (no crash)" 127 "$rc"

"$PIPEX" no_such_infile cat "wc -l" out 2>/dev/null; rc=$?
expect_eq "missing infile: second command still runs" "0" "$(tr -d ' ' < out)"
expect_eq "missing infile: exit status 0" 0 "$rc"

env -i "$PIPEX" infile cat "wc -l" out 2>/dev/null; rc=$?
expect_eq "no PATH in env -> 127 (no crash)" 127 "$rc"

"$PIPEX" infile cat false out; rc=$?
expect_eq "exit status of last command propagated" 1 "$rc"

"$PIPEX" infile cat 2>/dev/null >/dev/null; rc=$?
expect_eq "bad argument count -> non-zero" 1 "$rc"

rm -f newout
(umask 022 && "$PIPEX" infile cat cat newout)
expect_eq "outfile is created with 0644 (umask 022)" "644" "$(stat -c '%a' newout)"

if [ -x "$BONUS" ]; then
	echo "== bonus"

	"$BONUS" infile cat "grep hello" "wc -l" out; rc=$?
	expect_eq "multiple pipes" "2" "$(tr -d ' ' < out)"
	expect_eq "multiple pipes: exit status 0" 0 "$rc"

	timeout 10 "$BONUS" bigfile cat cat cat "wc -c" out; rc=$?
	expect_eq "multiple pipes: large output does not deadlock" 0 "$rc"
	expect_eq "multiple pipes: byte count" "$(wc -c < bigfile)" "$(cat out)"

	printf 'existing\n' > hd_out
	printf 'a\nEOFX\nb\nEOF\nc\n' | "$BONUS" here_doc EOF cat cat hd_out; rc=$?
	expect_eq "here_doc: limiter must match the whole line" \
		"$(printf 'existing\na\nEOFX\nb')" "$(cat hd_out)"
	expect_eq "here_doc: exit status 0" 0 "$rc"

	# stdin を読まないコマンドで確認する（cat cat では二重実行を検出できない）
	rm -f hd_out2
	printf 'only\n' | timeout 10 "$BONUS" here_doc EOF "echo once" cat hd_out2; rc=$?
	expect_eq "here_doc: EOF without limiter runs pipeline once" "once" "$(cat hd_out2)"
	expect_eq "here_doc: EOF without limiter: exit status 0" 0 "$rc"

	# bash の `<<` と同様、stdin を使わないコマンドでも LIMITER まで読み切ってから終了する
	rm -f hd_out3
	t0=$(now_ms)
	# pipex_bonus 自身の終了時刻・出力を記録する（パイプライン全体の終了は待たない）
	(sleep 2; printf 'x\nEND\n') | {
		timeout 10 "$BONUS" here_doc END "echo hi" cat hd_out3
		echo "$? $(now_ms)" > hd_res; cat hd_out3 > hd_snap 2>/dev/null; }
	read -r rc t1 < hd_res
	el=$(( t1 - t0 ))
	expect_eq "here_doc: waits for LIMITER even if commands ignore stdin" 1 "$([ "$el" -ge 1800 ] && echo 1 || echo "0 (${el}ms)")"
	expect_eq "here_doc: output written before exit" "hi" "$(cat hd_snap)"
	expect_eq "here_doc: waits for LIMITER: exit status 0" 0 "$rc"

	rm -f out
	timeout 10 "$BONUS" here_doc END cat "wc -c" out < <(cat bigfile; printf "\nEND\n"); rc=$?
	expect_eq "here_doc: large input (> pipe buffer) does not deadlock" 0 "$rc"
	expect_eq "here_doc: large input byte count" "$(( $(wc -c < bigfile) + 1 ))" "$(cat out)"

	rm -f out
	timeout 10 "$BONUS" here_doc END "echo hi" cat out < <(cat bigfile; printf "\nEND\n"); rc=$?
	expect_eq "here_doc: large input unread by commands does not hang" 0 "$rc"
	expect_eq "here_doc: large input unread: output" "hi" "$(cat out)"

	# here_doc の蓄積は線形時間で行う（行ごとの strjoin による二乗時間の回帰検出）
	rm -f out
	t0=$(now_ms)
	{ seq 100000; echo END; } | timeout 5 "$BONUS" here_doc END cat "wc -l" out; rc=$?
	el=$(( $(now_ms) - t0 ))
	expect_eq "here_doc: 100000 lines finish within 5s (${el}ms)" 0 "$rc"
	expect_eq "here_doc: 100000 lines line count" 100000 "$(tr -d ' ' < out)"

	rm -f out
	head -c 1200000 /dev/zero | tr '\0' 'b' | fold -w 99 > hd_mb; echo >> hd_mb
	{ cat hd_mb; echo END; } | timeout 5 "$BONUS" here_doc END cat "wc -c" out; rc=$?
	expect_eq "here_doc: >1MB input finishes within 5s" 0 "$rc"
	expect_eq "here_doc: >1MB input byte count" "$(wc -c < hd_mb)" "$(tr -d ' ' < out)"

	# 途中のコマンドも全て待ってから終了する（bash: < infile sleep 2 | ls > out）
	t0=$(now_ms)
	timeout 10 "$BONUS" infile "sleep 2" ls out; rc=$?
	el=$(( $(now_ms) - t0 ))
	expect_eq "waits for intermediate commands (sleep 2 | ls)" 1 "$([ "$el" -ge 1800 ] && echo 1 || echo "0 (${el}ms)")"
	expect_eq "sleep 2 | ls: exit status 0" 0 "$rc"

	# シェル（< missing cmd1 | cmd2 > out）と同様、infile が開けなくても後続は続行する
	rm -f out
	"$BONUS" no_such_infile cat "wc -l" out 2>err; rc=$?
	expect_eq "bonus: missing infile: second command still runs" "0" "$(tr -d ' ' < out)"
	expect_eq "bonus: missing infile: exit status 0" 0 "$rc"
	expect_eq "bonus: missing infile: error message" 1 "$(grep -c no_such_infile err)"

	rm -f out
	"$BONUS" no_such_infile cat cat "wc -l" out 2>/dev/null; rc=$?
	expect_eq "bonus: missing infile (3 cmds): pipeline continues" "0" "$(tr -d ' ' < out)"
	expect_eq "bonus: missing infile (3 cmds): exit status 0" 0 "$rc"

	rm -f out side
	"$BONUS" no_such_infile "touch side" "wc -l" out 2>/dev/null
	expect_eq "bonus: missing infile: first command is not started" 0 "$([ -e side ] && echo 1 || echo 0)"

	"$BONUS" no_such_infile cat false out 2>/dev/null; rc=$?
	expect_eq "bonus: missing infile: last status propagated" 1 "$rc"

	rm -f side
	"$BONUS" infile cat "touch side" no_such_dir/out 2>/dev/null; rc=$?
	expect_eq "bonus: unwritable outfile -> 1" 1 "$rc"
	expect_eq "bonus: unwritable outfile: last command is not started" 0 "$([ -e side ] && echo 1 || echo 0)"

	"$BONUS" infile cat false out; rc=$?
	expect_eq "bonus: exit status of last command propagated" 1 "$rc"

	"$BONUS" infile cat cat nosuchcmd_xyz out 2>/dev/null; rc=$?
	expect_eq "bonus: last command not found -> 127" 127 "$rc"

	"$BONUS" infile cat 2>/dev/null >/dev/null; rc=$?
	expect_eq "bonus: bad argument count -> non-zero" 1 "$rc"
fi

echo
echo "passed: $PASS, failed: $FAIL"
[ "$FAIL" -eq 0 ]
