#!/bin/bash
# Usage: ./export_drawio.sh yourfile.drawio
mkdir -p diagrams

FILE=$1
# Extract page names from the XML
# This looks for the 'name="..."' attribute in the <diagram> tags
NAMES=$(grep -oE 'name="[^"]+"' "$FILE" | cut -d'"' -f2)

INDEX=1
for NAME in $NAMES; do
  echo "Exporting page $INDEX: $NAME.svg"
  drawio -x -f svg -p $INDEX -o "diagrams/${NAME}.svg" "$FILE"
  INDEX=$((INDEX + 1))
done