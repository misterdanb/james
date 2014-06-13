#!/bin/sh

	astyle -T4 -A1 -k1 -H -U -Q -N -K -N -S -r -f -J -L -w -Y -xd -xw -xG --mode=c "*pp"
	astyle -T4 -A1 -k1 -H -U -Q -N -K -N -S -r -f -J -L -w -Y -xd -xw -xG --mode=c "*.template"
