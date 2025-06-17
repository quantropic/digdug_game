#!/bin/bash

# Create a soothing, relaxing tune using SoX
# This will create a sequence of gentle tones that are more calming than the Pac-Man theme

# Create a gentle melody with soft tones
sox -n -r 44100 -b 16 -c 1 /home/nika/digdug_game/audio/soothing_part1.wav \
    synth 0.5 sine 392 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 440 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 494 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 523 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 587 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 523 vol 0.3 fade 0.1 0.4 0.1

# Create a second part with lower, more relaxing tones
sox -n -r 44100 -b 16 -c 1 /home/nika/digdug_game/audio/soothing_part2.wav \
    synth 0.5 sine 349 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 392 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 440 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 494 vol 0.3 fade 0.1 0.4 0.1 \
    synth 1.0 sine 392 vol 0.3 fade 0.1 0.9 0.1

# Create a simple soothing music file directly
sox -n -r 44100 -b 16 -c 1 /home/nika/digdug_game/audio/soothing_music.wav \
    synth 0.5 sine 392 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 440 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 494 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 523 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 587 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 523 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 349 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 392 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 440 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 494 vol 0.3 fade 0.1 0.4 0.1 \
    synth 1.0 sine 392 vol 0.3 fade 0.1 0.9 0.1 \
    synth 0.5 sine 392 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 440 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 494 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 523 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 587 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 523 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 349 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 392 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 440 vol 0.3 fade 0.1 0.4 0.1 \
    synth 0.5 sine 494 vol 0.3 fade 0.1 0.4 0.1 \
    synth 1.0 sine 392 vol 0.3 fade 0.1 0.9 0.1

echo "Soothing music created successfully!"
