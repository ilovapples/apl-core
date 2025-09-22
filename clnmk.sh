#!/usr/bin/env sh

scriptdir="$(dirname "$0")"
if [ "$1" = "yes" ]; then
	make -C $scriptdir clean && make -C $scriptdir NO_LOGGING=-DAPLCORE__DISABLE_LOGGING
else
	make -C $scriptdir clean && make -C $scriptdir NO_LOGGING=
fi
