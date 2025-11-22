#!/bin/bash

# Paths
EXEC="./a.out"
TEST_DIR="testcases"
OUTPUT_DIR="outputs"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Loop over each testcase directory
for i in {14..15}; do
    GRAPH="$TEST_DIR/testcase$i/graph_$i.json"
    QUERIES="$TEST_DIR/testcase$i/queries_phase2_$i.json"
    OUTPUT="$OUTPUT_DIR/output$i.json"

    echo "Running testcase $i..."
    $EXEC "$GRAPH" "$QUERIES" "$OUTPUT"

    if [ $? -eq 0 ]; then
        echo "Testcase $i finished. Output saved to $OUTPUT"
    else
        echo "Testcase $i failed."
    fi
done

echo "All testcases completed."
