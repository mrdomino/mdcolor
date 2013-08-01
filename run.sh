#!/bin/sh
make mdcolor &&
  ./mdcolor $(head -c8 /dev/urandom | sha256sum | cut -d' ' -f1) > color.html &&
  xdg-open color.html
