#!/bin/bash

PROJECT=shownetif
VERSION=$(grep "^VERSION" $PROJECT.pro | \
                awk -F \= '{print $2}' | \
                sed 's/ //')

COMPRESS=bzip2
TARFILE=${PROJECT}-${VERSION}.tar.bz2

BRANCH=master

if [ $# -ge 1 ]; then
  BRANCH = $1
fi

git archive --prefix=${PROJECT}-${VERSION}/ $BRANCH | $COMPRESS > $TARFILE
ls -l $TARFILE
