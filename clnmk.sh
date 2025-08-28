#!/usr/bin/env sh

scriptdir="$(dirname "$0")"
if [ "$1" = "yes" ]; then
	make -C $scriptdir clean && make -C $scriptdir COMPILE_WITHOUT_LOGGING_OPTION=-DAPLCORE__DISABLE_LOGGING
else
	make -C $scriptdir clean && make -C $scriptdir COMPILE_WITHOUT_LOGGING_OPTION=
fi
