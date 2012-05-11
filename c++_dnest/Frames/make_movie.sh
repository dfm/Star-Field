#!/bin/bash
ffmpeg -r 10 -f image2 -i '%03d.png' -b:v 512k movie.ogg



