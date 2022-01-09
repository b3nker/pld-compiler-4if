#!/bin/bash

python3 ./tests/fuzzing/fuzz.py >_fuzzed.c

(./compiler/ifcc _fuzzed.c >_fuzzed.s && gcc -o fuzzed-pld.out _fuzzed.s && ./fuzzed-pld.out) &
pid_pld=$!

(gcc -Wno-overflow -Wno-div-by-zero -o fuzzed-gcc.out _fuzzed.c && ./fuzzed-gcc.out) &
pid_gcc=$!

wait $pid_gcc
out_gcc=$?

wait $pid_pld
out_pld=$?

if [[ "$out_gcc" -eq "$out_pld" ]]; then
  echo "success ($out_pld)"
  rm _fuzzed.c _fuzzed.s fuzzed-pld.out fuzzed-gcc.out
  exit 0
else
  echo "[fail] computed $out_pld but expected $out_gcc"
  if [[ "$out_pld" -eq "136" ]]; then
    echo "return code is 136, exception?"
  else
    cat _fuzzed.c
  fi
  echo
  echo
  exit 1
fi
