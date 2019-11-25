#!/bin/sh

CURR_DIR=$(dirname $0)

gdb --args ./bin/fractsim $(cat data/config/local)
