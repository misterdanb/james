#!/bin/sh

	astyle -T4 -A1 -k1 -H -U -Q -K -N -S -r -f -J -L -w -Y -xd -xG --mode=c "*pp"
	astyle -T4 -A1 -k1 -H -U -Q -K -N -S -r -f -J -L -w -Y -xd -xG --mode=c "*.template"
