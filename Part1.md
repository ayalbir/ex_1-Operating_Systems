# Part 1: PGN File Splitter Bash Script

## Overview
This part of the exercise involved creating a Bash script named `split_pgn.sh` to process chess games recorded in Portable Game Notation (PGN) format. The script splits multiple chess games contained in a single PGN file into separate files within a specified destination directory.

## Objectives
- Validate the presence of required command-line arguments.
- Verify the existence of the source PGN file.
- Ensure the destination directory exists or create it if necessary.
- Split multiple chess games from the source PGN file into individual files.
- Use functions to manage file operations effectively.

## Script Details
### Command-Line Arguments
The script accepts exactly two arguments:
- **Source PGN file**: The path to the PGN file containing multiple chess games.
- **Destination directory**: The directory where split game files will be saved.

### Usage
```bash
./split_pgn.sh <source_pgn_file> <destination_directory>
```

### Example Usages
#### Incorrect Usage
```bash
./split_pgn.sh /path/to/pgn/file
Usage: ./split_pgn.sh <source_pgn_file> <destination_directory>
```

#### Correct Usage (Directory already exists)
```bash
./split_pgn.sh /path/to/pgn/file /path/to/existing/destination
Saved game to /path/to/existing/destination/game1.pgn
Saved game to /path/to/existing/destination/game2.pgn
...
```

#### Correct Usage (Creating Directory)
```bash
./split_pgn.sh /path/to/pgn/file /path/to/new/destination
Created directory '/path/to/new/destination'.
Saved game to /path/to/new/destination/game1.pgn
Saved game to /path/to/new/destination/game2.pgn
...
```

#### Error Handling
```bash
./split_pgn.sh /path/to/nonexistent/file /path/to/destination
Error: File '/path/to/nonexistent/file' does not exist.
```

## Example Files and Folders
- **`pgns/`**: Contains example PGN files with multiple games.
- **`splited_pgn/`**: Contains the split individual game files created by the script.

## Conclusion
This part demonstrates how to handle file operations, validate inputs, and process structured data in a Bash script. The `split_pgn.sh` script effectively splits chess games from a single PGN file into multiple files, each representing an individual game.
