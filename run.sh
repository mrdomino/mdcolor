#!/bin/sh
make mdcolor &&
  head -c40 /dev/urandom | ./mdcolor > color.html &&
  xdg-open color.html
