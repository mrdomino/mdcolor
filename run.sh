#!/bin/sh
make mdcolor &&
  head -c10 /dev/urandom | ./mdcolor > color.html &&
  xdg-open color.html
