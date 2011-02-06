#!/bin/bash

VERSION=$(dpkg-parsechangelog | grep ^Version: | cut -d" " -f 2)

debuild -S -sn -I.bzr

cd ..; dput -f ppa:awuerl/blitzortung blitzortung-tracker_${VERSION}_source.changes
