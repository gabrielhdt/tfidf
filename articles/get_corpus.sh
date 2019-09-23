#!/bin/bash

_usage="Usage: $0 -l N
Download N lines of random wikipedia articles."

if [[ -z "$(which wikipedia2text)" ]]
then
    echo "Wikipedia2text not found"
fi

export BROWSER=$(which w3m)

TEMP=$(getopt --options 'l:?h' --longoptions 'lines:,help' -- "$@")
eval set -- "$TEMP"
while true
do
    case "$1" in
        -l|--lines)
            _nlines="$2"
            shift 2;;
        -h|--help)
            echo "$_usage"
            exit 0;;
        --)
            shift
            break;;
        *)
            echo "$_usage"
            exit 1;;
    esac
done

missing=$_nlines
count=0
while [[ $missing -ge 0 ]]
do
    ((count++))
    fname="art${count}.plain"
    wikipedia2text -r > "$fname"
    nlines=$(wc -l "$fname" | cut -f 1-1 -d' ')
    ((missing = missing - nlines))
done
