#!/usr/bin/env bash
set -euo pipefail

source /u/sw/etc/bash.bashrc
read -r -a modules <<< "${CLANGD_MODULES:-gcc-glibc eigen}"
module load "${modules[@]}"

compiler=$(command -v g++)
if [[ -z "$compiler" ]]; then
  printf 'Unable to find g++ after loading Lmod modules.\n' >&2
  exit 1
fi

mapfile -t include_dirs < <(
  env | awk -F= '$1 ~ /^mk[A-Za-z0-9_]*Inc$/ && $2 != "" { print $2 }' | sort -u
)

{
  printf 'CompileFlags:\n'
  printf '  Compiler: %s\n' "$compiler"
  if ((${#include_dirs[@]} > 0)); then
    printf '  Add:\n'
    for include_dir in "${include_dirs[@]}"; do
      printf '    - -I\n    - %s\n' "$include_dir"
    done
  fi
} > .clangd

printf 'Configured clangd with %d Lmod include directories.\n' "${#include_dirs[@]}"
