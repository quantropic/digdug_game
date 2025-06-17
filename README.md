# Dig Dug Clone

A clone of the classic Dig Dug arcade game with multiple versions.

## Versions

### Console Version

A simple console-based version of the game using ASCII characters.

#### How to Build

```bash
make digdug
```

#### How to Run

```bash
./digdug
```

### Basic Graphical Version (X11)

A graphical version using SFML that runs on X11 with colored shapes.

#### How to Build

```bash
make digdug_sfml
```

#### How to Run

```bash
./digdug_sfml
```

### Sprite-Based Graphical Version (X11)

A graphical version using SFML with Pac-Man style sprites.

#### How to Build

```bash
make digdug_sfml_sprites
```

#### How to Run

```bash
./digdug_sfml_sprites
```

### Sprite-Based Graphical Version with Sound Effects (X11)

A graphical version using SFML with Pac-Man style sprites and authentic Pac-Man sound effects (munch and death sounds).

#### How to Build

```bash
make digdug_sfml_sprites_music
```

#### How to Run

```bash
./digdug_sfml_sprites_music
```

## Game Controls

### All Versions
- W or Up Arrow: Move up
- A or Left Arrow: Move left
- S or Down Arrow: Move down
- D or Right Arrow: Move right
- SPACE: Use pump
- Q (console) or ESC (graphical): Quit game

## Game Rules

- Dig through dirt by moving around (score: 10 points per dirt block)
- Defeat enemies by pumping them (score: 100 points per enemy)
- Avoid getting caught by enemies
- Win by defeating all enemies

## Game Elements

### Console Version
- Player: 'P'
- Enemy: 'E'
- Dirt: '#'
- Rock: 'O'
- Pump: '*'
- Empty space: space

### Graphical Versions
- Player: Pac-Man style yellow character
- Enemy: Ghost-like blue character with eyes
- Dirt: Dark green textured blocks
- Rock: Detailed gray rocks
- Pump: Yellow cross
- Empty space: Black

## Requirements

- Console version: Standard C++ libraries
- Basic graphical version: SFML library (libsfml-dev package)
- Sprite-based version: SFML library (libsfml-dev package)
- Music version: SFML library with audio support (libsfml-dev package)
