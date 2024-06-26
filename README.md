# Minesweeper

This is a multiplayer minesweeper game made by C++ and SFML.

## Installation

### Dependencies

- CMake
- SFML

#### Ubuntu

```bash
sudo apt install cmake libsfml-dev
```

#### Fedora

```bash
sudo dnf install cmake SFML-devel
```

#### macOS

```bash
brew install cmake sfml
```

### Build

We use `cmake` to build this project.

```bash
cmake -B build
cmake --build build
```

and run it.

```bash
./build/minesweeper
```

## Block Status

Every block have 3 type with 11 status:

- covered
- uncovered(0-8)
- flagged

and 2 type:

- mine
- number

## Game Setting

You can change the game setting in `config.h`.

- `WIDTH`: the width of the game board
- `HEIGHT`: the height of the game board
- `MINES`: the number of mines
- `PROTECT`: protect you from the first click

## Host or Join

You can host a game by choosing `Host` in the main menu.

If you want to join a game, you need to enter the IP address of the host and press `Join`.
