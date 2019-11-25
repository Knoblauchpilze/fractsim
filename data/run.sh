#!/bin/sh

CURR_DIR=$(dirname $0)
echo $(cat data/config/local)
./bin/fractsim $(cat data/config/local)
