#!/bin/sh
#if [ -z "MMGT_CLEAR" ];
#if [ -n "${VAR:+x}" ]; then
if [ -z "$MMGT_CLEAR" ]; then
echo Applying env vars...
. /opt/occ63/env.sh
fi
progdir=`dirname $0`
LD_LIBRARY_PATH=$progdir/bin:$LD_LIBRARY_PATH
$progdir/bin/camocc2& echo $!

