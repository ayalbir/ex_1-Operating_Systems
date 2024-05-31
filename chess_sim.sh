#!/bin/bash
# 214104226 Ayal Birenstock
# Ensure correct number of arguments
if [ -z "$1" ] || [ ! -z "$2" ]; then
  echo "Usage: $0 <source_pgn_file>"
  exit 1
fi

input_file=$1

#this is the basic board, saved in an array of strings
board=(
  "  a b c d e f g h"
  "8 r n b q k b n r 8"
  "7 p p p p p p p p 7"
  "6 . . . . . . . . 6"
  "5 . . . . . . . . 5"
  "4 . . . . . . . . 4"
  "3 . . . . . . . . 3"
  "2 P P P P P P P P 2"
  "1 R N B Q K B N R 1"
  "  a b c d e f g h"
)

# Extract the moves from the PGN file
moves=""

# Read the PGN file line by line
in_moves_section=false
while IFS= read -r line; do
  if [[ $line == "" ]]; then
    in_moves_section=false
  elif [[ $line == "1."* ]]; then
    in_moves_section=true
  fi

  if $in_moves_section; then
    moves+="$line "
  fi
done <"$input_file"

# Remove the trailing space
moves=${moves::-1}

#use the Python script
output=$(python3 parse_moves.py "$moves")

#note that the moves aren't numbered

# Print the details of the game
function print_game_details() {
  echo "Metadata from PGN file:"
  while IFS= read -r l; do
    if [[ $l == "1."* ]]; then
      break
    fi
    echo "$l"
  done <"$input_file"
  echo ""
}

# Print the instructions
function print_instructions() {
  echo -n "Press 'd' to move forward, 'a' to move back, 'w' to go to the start,'s' to go to the end, 'q' to quit: "
}

# Print the board
function print_board() {
  for row in "${board[@]}"; do
    echo "$row"
  done
}

# Apply a move to the board
function apply_move() {
  move=$1
  src_file=${move:0:1}
  src_rank=${move:1:1}
  dest_file=${move:2:1}
  dest_rank=${move:3:1}
  promotion_piece=${move:4:1}

  src_index=$((9 - src_rank))
  dest_index=$((9 - dest_rank))
  src_col=$(($(printf "%d" "'$src_file") - 97))
  dest_col=$(($(printf "%d" "'$dest_file") - 97))

  if [[ $src_index -ge 1 && $src_index -le 8 && $dest_index -ge 1 && $dest_index -le 8 ]]; then
    piece=${board[$src_index]:$((src_col * 2 + 2)):1}
    board[$src_index]=${board[$src_index]:0:$((src_col * 2 + 2))}"."${board[$src_index]:$((src_col * 2 + 3))}
    if [[ $piece == "P" && $dest_index == 1 ]]; then
      #Uppercase pawn promotion
      #turn the promotion piece to uppercase
      promotion_piece=$(echo $promotion_piece | tr '[:lower:]' '[:upper:]')
      piece=$promotion_piece
    elif [[ $piece == "p" && $dest_index == 8 ]]; then
      #Lowercase pawn promotion
      piece=$promotion_piece
    fi
    board[$dest_index]=${board[$dest_index]:0:$((dest_col * 2 + 2))}$piece${board[$dest_index]:$((dest_col * 2 + 3))}
  else
    echo "Invalid move: $move"
  fi
}

#reset the board to the initial state
function reset_board() {
  board=(
    "  a b c d e f g h"
    "8 r n b q k b n r 8"
    "7 p p p p p p p p 7"
    "6 . . . . . . . . 6"
    "5 . . . . . . . . 5"
    "4 . . . . . . . . 4"
    "3 . . . . . . . . 3"
    "2 P P P P P P P P 2"
    "1 R N B Q K B N R 1"
    "  a b c d e f g h"
  )
}

# Print the game details
print_game_details

all_moves=$(echo "$output" | wc -w)
moves_array=($output)

# The first move (nothing has been done yet) is 0
current_move=0

echo " "
echo "Move $current_move/$all_moves"
print_board
print_instructions

# Now the game starts.

# Detect the key pressed by the user, and act accordingly
while read -n 1 key; do
  # 'q' to quit
  if [ "$key" == "q" ]; then
    echo "Exiting."
    break
  fi
  # 'd' to move forward
  if [ "$key" == "d" ]; then
    if [ "$current_move" -lt "$all_moves" ]; then
      current_move=$((current_move + 1))
      echo " "
      echo "Move $current_move/$all_moves"
      reset_board
      for ((i = 0; i < current_move; i++)); do
        apply_move ${moves_array[i]}
      done
      print_board
      print_instructions
    else
      echo " "
      echo "No more moves available."
      continue
    fi
  fi
  # If the key is 'a', move back
  if [ "$key" == "a" ]; then
    if [ "$current_move" -gt 0 ]; then
      current_move=$((current_move - 1))
      echo " "
      echo "Move $current_move/$all_moves"
      reset_board
      for ((i = 0; i < current_move; i++)); do
        apply_move ${moves_array[i]}
      done
      print_board
      print_instructions

    else
      #"pressing 'a' will have no effect"
      continue
    fi
  fi
  # If the key is 'w', go to the start
  if [ "$key" == "w" ]; then
    # Print the board at the start
    current_move=0
    echo " "
    echo "Move $current_move/$all_moves"
    reset_board
    print_board
    print_instructions
  fi
  # If the key is 's', go to the end
  if [ "$key" == "s" ]; then
    # Print the board at the end
    current_move=$all_moves
    echo " "
    echo "Move $current_move/$all_moves"
    reset_board
    for ((i = 0; i < current_move; i++)); do
      apply_move ${moves_array[i]}
    done
    print_board
    print_instructions
  else
    echo "Invalid key pressed: $key"
  fi
done
