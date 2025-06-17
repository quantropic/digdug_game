#!/bin/bash

# Create player sprite (more visible Pac-Man)
convert -size 40x40 xc:transparent \
  -fill "rgb(255,255,0)" -draw "circle 20,20 20,5" \
  -fill "black" -draw "polygon 20,20 35,5 35,35" \
  /home/nika/digdug_game/sprites/player.png

# Create enemy sprite (more detailed ghost-like)
convert -size 40x40 xc:transparent \
  -fill "rgb(50,50,255)" -draw "roundrectangle 5,5 35,30 10,10" \
  -fill "rgb(50,50,255)" -draw "polygon 5,30 10,40 15,30 20,40 25,30 30,40 35,30" \
  -fill "white" -draw "circle 15,15 18,18" \
  -fill "white" -draw "circle 25,15 28,18" \
  -fill "black" -draw "circle 15,15 16,16" \
  -fill "black" -draw "circle 25,15 26,16" \
  /home/nika/digdug_game/sprites/enemy.png

# Create dirt sprite (dark green instead of brown)
convert -size 40x40 xc:transparent -fill "rgb(0,100,0)" -draw "rectangle 0,0 40,40" -fill "rgb(0,80,0)" -draw "path 'M 0,0 L 10,0 L 10,10 L 0,10 Z M 20,0 L 30,0 L 30,10 L 20,10 Z M 10,10 L 20,10 L 20,20 L 10,20 Z M 30,10 L 40,10 L 40,20 L 30,20 Z M 0,20 L 10,20 L 10,30 L 0,30 Z M 20,20 L 30,20 L 30,30 L 20,30 Z M 10,30 L 20,30 L 20,40 L 10,40 Z M 30,30 L 40,30 L 40,40 L 30,40 Z'" /home/nika/digdug_game/sprites/dirt.png

# Create rock sprite with more detailed texture
convert -size 40x40 xc:transparent \
  -fill "rgb(120,120,120)" -draw "circle 20,20 35,20" \
  -fill "rgb(150,150,150)" -draw "circle 15,15 20,10" \
  -fill "rgb(100,100,100)" -draw "circle 25,25 30,20" \
  -fill "rgb(80,80,80)" -draw "circle 10,30 15,25" \
  -fill "rgb(170,170,170)" -draw "circle 30,10 35,5" \
  -fill "rgb(90,90,90)" -draw "circle 25,30 30,25" \
  -fill "rgb(130,130,130)" -draw "circle 10,10 15,5" \
  /home/nika/digdug_game/sprites/rock.png

# Create pump sprite
convert -size 40x40 xc:transparent -fill yellow -draw "rectangle 15,15 25,25" -draw "line 20,5 20,15" -draw "line 20,25 20,35" -draw "line 5,20 15,20" -draw "line 25,20 35,20" /home/nika/digdug_game/sprites/pump.png

echo "Sprites created successfully!"
