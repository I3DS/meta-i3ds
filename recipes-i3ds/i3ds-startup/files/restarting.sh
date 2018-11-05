#!/bin/sh
# Delay in microseconds
RESTART_DELAY=200000

while true; do
    ${@}
    /bin/usleep ${RESTART_DELAY}
    echo "Restarting ${1}"
done
