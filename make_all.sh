#!/bin/sh

yq -r '(keys)[]' device_db.yaml  | while read ITER; do
    BOARD=$ITER make clean && BOARD=$ITER make -j16
done
make update_converters