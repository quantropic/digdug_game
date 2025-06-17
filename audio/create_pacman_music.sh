#!/bin/bash

# Create a simple Pac-Man-like tune using SoX
# This will create a sequence of tones that sound similar to the Pac-Man theme

# Create the intro part
sox -n -r 44100 -b 16 -c 1 /home/nika/digdug_game/audio/pacman_intro.wav synth 0.15 sine 880 vol 0.5 \
    synth 0.15 sine 988 vol 0.5 \
    synth 0.15 sine 1046 vol 0.5 \
    synth 0.15 sine 880 vol 0.5 \
    synth 0.15 sine 1046 vol 0.5 \
    synth 0.15 sine 988 vol 0.5

# Create the main theme part
sox -n -r 44100 -b 16 -c 1 /home/nika/digdug_game/audio/pacman_main.wav synth 0.15 sine 659 vol 0.5 \
    synth 0.15 sine 587 vol 0.5 \
    synth 0.15 sine 523 vol 0.5 \
    synth 0.3 sine 494 vol 0.5 \
    synth 0.15 sine 523 vol 0.5 \
    synth 0.15 sine 587 vol 0.5 \
    synth 0.15 sine 659 vol 0.5 \
    synth 0.15 sine 587 vol 0.5 \
    synth 0.15 sine 523 vol 0.5 \
    synth 0.3 sine 494 vol 0.5 \
    synth 0.15 sine 523 vol 0.5 \
    synth 0.15 sine 587 vol 0.5 \
    synth 0.15 sine 659 vol 0.5 \
    synth 0.15 sine 587 vol 0.5 \
    synth 0.15 sine 523 vol 0.5 \
    synth 0.15 sine 494 vol 0.5 \
    synth 0.15 sine 440 vol 0.5 \
    synth 0.15 sine 494 vol 0.5

# Combine intro and main theme
sox /home/nika/digdug_game/audio/pacman_intro.wav /home/nika/digdug_game/audio/pacman_main.wav /home/nika/digdug_game/audio/pacman_music.wav

# Clean up temporary files
rm /home/nika/digdug_game/audio/pacman_intro.wav
rm /home/nika/digdug_game/audio/pacman_main.wav

echo "Pac-Man music created successfully!"
