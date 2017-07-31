#!/bin/bash

function abspath()
{
  case "${1}" in
    [./]*)
    echo "$(cd ${1%/*}; pwd)/${1##*/}"
    ;;
    *)
    echo "${PWD}/${1}"
    ;;
  esac
}

savedir=$PWD

THISFILE=`abspath $BASH_SOURCE`
XDIR=`dirname $THISFILE`
if [ -L ${THISFILE} ];
then
    target=`readlink $THISFILE`
    XDIR=`dirname $target`
fi

THISDIR=$XDIR

export LBLTBGUI=$THISDIR

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$THISDIR/lib
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$THISDIR/lib

export PATH=$PATH:$THISDIR/bin
