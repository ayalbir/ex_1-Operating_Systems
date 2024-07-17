# Part 2: Chess Simulator Bash Script

## Overview
This part involved developing a Bash script `chess_sim.sh` that simulates a chess game using Portable Game Notation (PGN) files. The script utilizes `parse_moves.py`, a provided Python script, to convert PGN moves to Universal Chess Interface (UCI) format for easier manipulation.

## Objectives
- Parse chess games from PGN files.
- Display and manipulate chess games in the terminal using Bash.
- Convert chess notation from PGN to UCI using Python.

## Script Details

### Command-Line Arguments
The script accepts one argument:
- **PGN file**: The path to the PGN file containing a single chess game.

### Usage
```bash
./chess_sim.sh <pgn_file>
```

### Example Usage
```bash
./chess_sim.sh /path/to/single_game.pgn
```

### Key Bindings and Their Effects
- **'d' (Next Move)**: Advance the game by one move.
- **'a' (Previous Move)**: Revert to the previous move.
- **'w' (Go to Start)**: Reset the board to the initial state.
- **'s' (Go to End)**: Apply all moves until the end of the game.
- **'q' (Quit)**: Exit the script.

### Example Output
```plaintext
  a b c d e f g h
8 r n b q k b n r
7 p p p p . p p p
6 . . . . . . . .
5 . . . . p . . .
4 . . . . P . . .
3 . . . . . N . .
2 P P P P . P P P
1 R N B Q K B . R
  a b c d e f g h

Move 5/20
Press 'd' to move forward, 'a' to move back, 'w' to go to the start, 's' to go to the end, 'q' to quit:
```

## Conclusion
This part integrates Python and Bash scripting to create an interactive chess game simulator. The `chess_sim.sh` script effectively parses and displays chess games, allowing users to navigate through the moves using simple key presses.
