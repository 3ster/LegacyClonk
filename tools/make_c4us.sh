#!/usr/bin/bash

# example usage: OLDOBJVERSION=4,9,10,7 OBJVERSION=4.9.10.7 VERSION=330 C4GROUP=~/clonk/c4group64 ./make_c4us.sh

set -e

if [ -z "$C4GROUP" ]; then
	echo "Please define the environment variable C4GROUP to point to your c4group(64) binary" >&2
	exit 1
fi

if [ -z "$VERSION" ]; then
	echo "Please define the environment variable VERSION and tell what version this will be (e.g. 330)" >&2
	exit 1
fi

if [ -z "$OLDOBJVERSION" ]; then
	echo "Please define the environment variable OLDOBJVERSION and tell what version the old definitions were (comma-separated, e.g. 4,9,10,7)" >&2
	exit 1
fi

if [ -z "$OBJVERSION" ]; then
	echo "Please define the environment variable OBJVERSION and tell what version the new definitions are (e.g. 4.9.10.7)" >&2
	exit 1
fi

SCRIPT_DIR="$(realpath $(dirname $0))"

"$SCRIPT_DIR/make_Language.c4g.sh"

if [ -z "$SRCDIR" ]; then
	SRCDIR="$SCRIPT_DIR/../planet/System.c4g"
fi

TARGET_DIR="$(realpath .)"

TAG="continuous-v330"

if [ -z "$GET_win32" ]; then
	GET_win32="curl -L -o /tmp/LegacyClonk.zip https://github.com/Fulgen301/LegacyClonk/releases/download/$TAG/LegacyClonk.zip && unzip /tmp/LegacyClonk.zip && rm /tmp/LegacyClonk.zip"
fi

if [ -z "$GET_linux" ]; then
	GET_linux="curl -L https://github.com/Fulgen301/LegacyClonk/releases/download/$TAG/LegacyClonk-x86.tar.gz | tar x"
fi

if [ -z "$GET_linux64" ]; then
	GET_linux64="curl -L https://github.com/Fulgen301/LegacyClonk/releases/download/$TAG/LegacyClonk-x64.tar.gz | tar x"
fi

if [ -z "$GET_mac" ]; then
	GET_mac="cp -r /home/maxmitti/lc_/tools/Clonk.app ."
fi

function win32() {
	mv clonk.exe "$1/Clonk.exe"
	mv clonk.pdb "$1/Clonk.pdb"
	mv *.dll *.pdb *.exe "$1"
}

function linux() {
	mv clonk c4group "$1"
}

function linux64() {
	linux "$@"
}

function mac() {
	mv Clonk.app "$1"
	mv "$1/Language.c4g" "$1/Clonk.app/Contents/MacOS"
	mv "$1/Clonk.app/Contents/MacOS/c4group" "$1"
}

AUTOUPDATEFILE=$(cat <<EOF
Update
Name=$OBJVERSION
RequireVersion=$OLDOBJVERSION
EOF
)


# MAC here

for PLATFORM in "win32" "linux" "linux64" "mac"; do
	if [ -d "$TARGET_DIR/$PLATFORM" ]; then
		rm -r "$TARGET_DIR/$PLATFORM"
	fi
	mkdir -p "$TARGET_DIR/$PLATFORM"
	pushd "$TARGET_DIR/$PLATFORM"
	UPDATE=lc_${VERSION}_$PLATFORM.c4u
	mkdir $UPDATE
	cp "$TARGET_DIR/Language.c4g" $UPDATE
	echo "$AUTOUPDATEFILE" > "$UPDATE/AutoUpdate.txt"
	CMD="GET_$PLATFORM"
	bash -c "${!CMD}"
	"$PLATFORM" "$UPDATE"
	"$C4GROUP" "$UPDATE" -p
	mv "$UPDATE" "$TARGET_DIR"
	popd
done