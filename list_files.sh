#!/bin/bash

# Check if directory argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <directory_path>"
  exit 1
fi

DIR="$1"

# Check if directory exists
if [ ! -d "$DIR" ]; then
  echo "Error: Directory does not exist"
  exit 1
fi

# Recursive function to print tree
print_tree() {
  local dir="$1"
  local prefix="$2"

  # Get list of items
  local items=("$dir"/*)
  local count=${#items[@]}
  local i=0

  for item in "${items[@]}"; do
    [ -e "$item" ] || continue

    ((i++))
    local name=$(basename "$item")

    if [ $i -eq $count ]; then
      echo "${prefix}└── $name"
      new_prefix="${prefix}    "
    else
      echo "${prefix}├── $name"
      new_prefix="${prefix}│   "
    fi

    if [ -d "$item" ]; then
      print_tree "$item" "$new_prefix"
    fi
  done
}

# Print root directory name
echo "$(basename "$DIR")"
print_tree "$DIR" ""