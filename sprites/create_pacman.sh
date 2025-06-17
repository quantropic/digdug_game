#!/bin/bash

# Create a simple but very visible Pac-Man sprite
convert -size 40x40 xc:black \
  -fill yellow -draw "circle 20,20 35,20" \
  -fill black -draw "polygon 20,20 40,5 40,35" \
  /home/nika/digdug_game/sprites/player.png

echo "New Pac-Man sprite created!"
