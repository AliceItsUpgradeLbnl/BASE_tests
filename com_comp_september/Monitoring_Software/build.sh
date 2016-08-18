#!/bin/bash

args=$@
function is_arg_set
{
        for i in $args ; do
            if [[ $i == $1 ]] ; then
                return 0 #this is true
            fi
        done
        return 1 #this is false
}

savedir=$PWD

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

cd $THISDIR

echo $PWD

if is_arg_set "realclean" ;
then
	rm -rf $THISDIR/include/* $THISDIR/lib/*
	for ex in "test.exe"
	do
  		rm -rf $THISDIR/bin/$ex
	done
fi

for pack in src
do
	bdir=./.build/$pack
	if is_arg_set "realclean" ;
	then
	    rm -rf $bdir
	fi
	mkdir -p $bdir
	cd $bdir
	debug="-DCMAKE_BUILD_TYPE=Release"
	#cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo
	#is_arg_set "o3" && export CXXFLAGS=-O3 # Release does it
	is_arg_set "debug" && debug=-DCMAKE_BUILD_TYPE=Debug
	#is_arg_set "debug" && extracxx=-ftrapping-math -fsignaling-nans $CXXFLAGS
	is_arg_set "debug" && extracxx=-fsignaling-nans
	cmake -DCMAKE_INSTALL_PREFIX=$THISDIR $debug -DCMAKE_CXX_FLAGS=$extracxx $THISDIR/$pack
	is_arg_set "clean" && make clean
	is_arg_set "verbose" && verbose="VERBOSE=1"
	make $verbose && make install
done

cd $savedir