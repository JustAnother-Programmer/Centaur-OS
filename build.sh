#!/bin/bash
set -euo pipefail

# Specify location of cross-compiler
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# Get directory script is in
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# Array of directories needed for build
dirs=(
    "$script_dir/bin"
    "$script_dir/build"
    "$script_dir/build/disk"
    "$script_dir/build/gdt"
    "$script_dir/build/idt"
    "$script_dir/build/io"
    "$script_dir/build/isr80h"
    "$script_dir/build/string"
    "$script_dir/build/task"
    "$script_dir/build/fs/fat"
    "$script_dir/build/memory/heap"
    "$script_dir/build/memory/paging"
    "$script_dir/programs/blank/build"
)

# Create the directories, if they already exist do nothing
for d in "${dirs[@]}"; do
    mkdir -pv -- "$d"
done

make all