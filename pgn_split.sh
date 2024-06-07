#!/bin/bash
#214104226 Ayal Birenstock

#the path to the PGN file
# $1
#the dest (or create one)
# $2
input_file=$1
dest_dir=$2
#if num of args is incorrect, meaning or less then 2 or more than that.
if [ -z "$1" ] || [ -z "$2" ] || [ ! -z "$3" ]; then
    echo "Usage: $0 <source_pgn_file> <destination_directory>"
    exit 1
    fi
if [ ! -f "$input_file" ]; then
    echo "Error: File '$input_file' does not exist."
    exit 1
fi
#only if dest doesn't exist, create it
if [ ! -d "$dest_dir" ]; then
    mkdir -p "$dest_dir"
    echo "Created directory '$dest_dir'."
fi


echo "Splitting games from '$input_file' to '$dest_dir'."
#split the games by Events, and save them by "input_file_n" to dest_dir
base_name=$(basename "$input_file" .pgn)
awk '/^\[Event / { n++; close(f); f=sprintf("%s/%s_%d.pgn", "'"$dest_dir"'", "'"$base_name"'", n) } { print > f }' "$input_file"
# print for each game "saved game to dest_dir"
for file in "$dest_dir"/*; do
    echo "Saved game to $file."
done
echo "All games have been split and saved to $dest_dir."
