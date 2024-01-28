# Minesweeper

This is a minesweeper game made by SDL2 and C++.

## Installation

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
- flagged(right, wrong)

and 2 type:

- mine
- number

## Game Setting

You can change the game setting in `config.json`.

```json
{
    "width": 10,
    "height": 10,
    "mine": 10,
    "protect": 1,
}
```

- `width`: the width of the game board
- `height`: the height of the game board
- `mine`: the number of mines
- `protect`: protect you from the first click
