# Hill Climb Racing

A 2D physics-based racing game developed as a lab project for CSE-1211 course using SDL2 and C++.

## Description

Hill Climb Racing is a side-scrolling racing game where players navigate challenging terrain while managing fuel and collecting coins. The game features realistic physics simulation, collision detection, and an engaging scoring system.

## Features

- **Physics-based gameplay** with realistic vehicle dynamics
- **Collision detection** system for terrain and objects
- **Sound effects and background music** using SDL2_mixer
- **High score tracking** with persistent storage
- **Fuel management** system with low fuel warnings
- **Coin collection** mechanics
- **Texture management** for optimized graphics rendering

## Prerequisites

- SDL2 development libraries
- SDL2_mixer for audio
- C++ compiler (GCC recommended)
- Make utility

## Installation

1. Clone the repository:
```bash
git clone <your-repository-url>
cd Hill-Climb-Racing
```

2. Install SDL2 dependencies (Ubuntu/Debian):
```bash
sudo apt-get install libsdl2-dev libsdl2-mixer-dev
```

3. Build the project:
```bash
make
```

## Usage

Run the game:
```bash
./main
```

## Game Controls

- **Arrow Keys/WASD**: Control vehicle movement
- **Space**: Brake
- **ESC**: Exit game

## Project Structure

```
Hill-Climb-Racing/
├── Game.cpp/hpp          # Main game engine and loop
├── main.cpp              # Entry point
├── Collision.cpp/hpp     # Collision detection system
├── TextureManager.cpp/hpp # Graphics and texture handling
├── Vector2D.cpp/hpp      # 2D vector mathematics
├── highscore.cpp/hpp     # Score management
├── assets/               # Game assets (sprites, sounds)
├── ECS/                  # Entity Component System
└── Makefile             # Build configuration
```

## Game Mechanics

- **Fuel System**: Monitor fuel levels and refuel strategically
- **Physics Engine**: Realistic vehicle physics with momentum and gravity
- **Scoring**: Earn points by collecting coins and traveling distance
- **High Scores**: Persistent high score tracking

## Development

This project demonstrates:
- Object-oriented programming in C++
- Game development with SDL2
- Physics simulation
- Memory management
- Audio integration
- File I/O for high scores

## Assets

The game includes custom graphics and sound effects located in the `assets/` directory:
- Vehicle sprites and animations
- Background textures
- Sound effects (coins, fuel warnings, game over)
- Background music

## Contributing

This is a lab project for educational purposes. Feel free to fork and experiment with additional features.

## License

Educational project for CSE-1211 course.

## Acknowledgments

- SDL2 library for graphics and audio
- Course instructors and lab guidelines