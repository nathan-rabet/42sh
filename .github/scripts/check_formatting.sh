for file in $(find . -name "*.h" -o -name "*.c"); do
    clang-format -style=file -output-replacements-xml $file | grep "<replacement " >/dev/null
    if [ $? -eq 0 ]; then
        echo "File $file is not formatted correctly, please run 'clang-format -i $file'" >&2
        IS_ALL_CORRECT=$false
    fi
done

if [ "$IS_ALL_CORRECT" = "$false" ]; then
    echo "Some files are not formatted correctly." >&2
    exit 1
fi
