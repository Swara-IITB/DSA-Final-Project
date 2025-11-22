#!/bin/bash

# Paths
EXEC="./a.out"
TEST_DIR="testcases"
OUTPUT_DIR="outputs"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Loop over each testcase directory
for i in {1..10}; do
    GRAPH="$TEST_DIR/graph_$i.json"
    QUERIES="$TEST_DIR/query_graph$i.json"
    OUTPUT="$OUTPUT_DIR/output$i.json"

    echo "Running testcase $i..."
    $EXEC "$GRAPH" "$QUERIES" "$OUTPUT"

    if [ $? -eq 0 ]; then
        echo "Testcase $i finished. Output saved to $OUTPUT"
    else
        echo "Testcase $i failed."
    fi
done
for i in {1..10}; do
    GRAPH="$TEST_DIR/graph_$i.json"
    QUERIES="$TEST_DIR/penalty_query_graph_$i.json"
    OUTPUT="$OUTPUT_DIR/penalty_output$i.json"

    echo "Running testcase $i..."
    $EXEC "$GRAPH" "$QUERIES" "$OUTPUT"

    if [ $? -eq 0 ]; then
        echo "Testcase $i finished. Output saved to $OUTPUT"
    else
        echo "Testcase $i failed."
    fi
done
echo "All testcases completed."
