#!/bin/sh

if [ -z "$PREFIX" ];then PREFIX=/usr; fi
if [ -z "$MODE" ];then MODE=release; fi
if [ -z "$THREADS" ];then THREADS=4; fi

echo "PREFIX=$PREFIX"
echo "MODE=$MODE"
echo "THREADS=$THREADS"

CWD=`realpath $(dirname "$0")`
DepPath=$CWD/3rdParty

#git submodule update --init --recursive

function buildQt() {
  cd $DepPath/$1 \
  && qmake-qt5 CONFIG+=$MODE PREFIX=$PREFIX \
  && make -j $THREADS install \
  && return 0

  return 0
}

set -x
echo "Check for required libs"

echo "Building Dependencies" \
&& buildQt QFieldValidator \
&& buildQt TargomanCommon \
&& buildQt QHttp \
&& buildQt QRestServer \
&& cd $DepPath/ISO639/ && cp ISO639.h $PREFIX/include/ \
&& cd $DepPath/fasttext/ && mkdir -p build && cd build && cmake -DCMAKE_INSTALL_PREFIX=$PREFIX .. && make -j $THREADS install \
&& buildQt TextProcessor \
&& buildQt TargomanDBM \
&& cd $CWD \
&& qmake-qt5 CONFIG+=$MODE PREFIX=$PREFIX && make -j $THREADS install


