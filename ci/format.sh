#!/usr/bin/env bash

set -euo pipefail

# Find all C++ source and header files, excluding managed_components.
files=()
while IFS= read -r -d '' file; do
    files+=("$file")
done < <(
    find . \
        -path './managed_components' -prune -o \
        -type f \( -name '*.cpp' -o -name '*.h' \) -print0
)

if [[ ${#files[@]} -eq 0 ]]; then
    echo "No .cpp or .h files found."
    exit 0
fi

case "${1:-check}" in
    check|dry-run)
        echo "Checking clang-format..."

        if clang-format --dry-run --Werror "${files[@]}"; then
            echo "✓ Formatting is correct."
        else
            echo "✗ Formatting issues found."
            echo "Run '$0 format' to fix them."
            exit 1
        fi
        ;;

    format|fix)
        echo "Formatting ${#files[@]} files..."

        clang-format -i "${files[@]}"

        echo "✓ Formatting complete."
        ;;

    *)
        echo "Usage: $0 [check|format]"
        exit 1
        ;;
esac
