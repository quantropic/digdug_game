#!/bin/bash

# Create a directory for sound effects if it doesn't exist
mkdir -p /home/nika/digdug_game/audio/effects

# Create Pac-Man munch sound effect
# This creates a short "waka" sound similar to the classic Pac-Man eating sound
sox -n -r 44100 -b 16 -c 1 /home/nika/digdug_game/audio/effects/munch.wav \
    synth 0.07 sine 440 vol 0.7 fade 0.01 0.07 0.01 \
    synth 0.07 sine 880 vol 0.7 fade 0.01 0.07 0.01

# Create Pac-Man death sound effect
# This creates a descending sound similar to the classic Pac-Man death sound
sox -n -r 44100 -b 16 -c 1 /home/nika/digdug_game/audio/effects/death.wav \
    synth 0.1 sine 880 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 830 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 780 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 730 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 680 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 630 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 580 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 530 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 480 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 430 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 380 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.2 sine 330 vol 0.7 fade 0.01 0.2 0.05

# Create a power pellet sound effect
sox -n -r 44100 -b 16 -c 1 /home/nika/digdug_game/audio/effects/power.wav \
    synth 0.1 sine 880 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 440 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 880 vol 0.7 fade 0.01 0.1 0.01 \
    synth 0.1 sine 440 vol 0.7 fade 0.01 0.1 0.01

echo "Pac-Man sound effects created successfully!"
