IS_ALL_CORRECT=1
for file in $(find . -name "*.h" -o -name "*.c"); do
    clang-format -style=file -output-replacements-xml $file | grep "<replacement " >/dev/null
    if [ $? -eq 0 ]; then
        echo "File $file is not formatted correctly, please run 'clang-format -i $file'" >&2
        IS_ALL_CORRECT=0
    fi
done
if [ "$IS_ALL_CORRECT" = "0" ]; then
    echo "Some files are not formatted correctly." >&2
    exit 1
else
    echo "All files are formatted correctly."
    exit 0
fi
