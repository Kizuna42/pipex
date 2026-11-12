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

	rm -f hd_out2
	printf 'only\n' | timeout 10 "$BONUS" here_doc EOF cat cat hd_out2; rc=$?
	expect_eq "here_doc: EOF without limiter runs pipeline once" "only" "$(cat hd_out2)"

	"$BONUS" infile cat 2>/dev/null >/dev/null; rc=$?
	expect_eq "bonus: bad argument count -> non-zero" 1 "$rc"
fi

echo
echo "passed: $PASS, failed: $FAIL"
[ "$FAIL" -eq 0 ]
