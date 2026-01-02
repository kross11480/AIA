#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
INPUT_FILE="$SCRIPT_DIR/Foundation.drawio"
OUTPUT_DIR="$SCRIPT_DIR/diagrams"

# Create img directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

MAPFILE=($(grep -o 'name="[^"]*"' "$INPUT_FILE"| cut -d'"' -f2))

for i in "${!MAPFILE[@]}"; do
  CLEAN_NAME=$(echo "${MAPFILE[$i]}" | tr '[:upper:]' '[:lower:]' | tr ' ' '-' | sed 's/[^a-z0-9-]//g')

  # Export directly into the img folder
  drawio -x -f svg -p $i --crop --transparent -o "$OUTPUT_DIR/$CLEAN_NAME.svg" "$INPUT_FILE"
done