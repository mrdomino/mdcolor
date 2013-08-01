#!/bin/sh
BROWSER=surf
make mdcolor && ./mdcolor $(sha256 < $1) | ./pagefmt > page.html &&
	${BROWSER} page.html
