#!/bin/bash
ffmpeg -r 10 -f image2 -i '%04d.png' -vcodec huffyuv movie.mkv



