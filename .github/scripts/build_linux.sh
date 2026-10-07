#!/bin/sh
#
# Build script for Maude (Linux)

set -e

YICES_VERSION="2.7.0"
YICES_URL="https://github.com/SRI-CSL/yices2/archive/refs/tags/yices-$YICES_VERSION.tar.gz"
CXXFLAGS="-Wall -O3 -flto -fno-stack-protector"

# Common definitions
. "$(dirname "$0")/common.sh"

# Install build dependencies
sudo apt install -y flex bison make gperf wget libbdd-dev libgmp-dev libsigsegv-dev libtecla-dev

# Download, build and install yices2
curl -JLO "$YICES_URL"
tar -xf "yices2-yices-$YICES_VERSION.tar.gz"
cd "yices2-yices-$YICES_VERSION"
autoconf
./configure --prefix="/usr"
make -j 2
sudo make install
cd ..

# Change version for development versions
if ! is_release; then
	sed -i "s/AC_INIT(\[Maude\],\[[^]]*\]/AC_INIT([Maude],[git-$(git rev-parse --short=6 HEAD)]/" configure.ac
fi

# Build Maude
autoreconf -i
mkdir linux64
cd linux64

../configure --with-smt=yices2 \
	CXXFLAGS="$CXXFLAGS" \
	LDFLAGS="-static-libstdc++ -Wl,-S -Wl,-Bstatic" \
	LIBS="-Wl,-Bdynamic" \
	--disable-dependency-tracking

make -j 2

# Strip maude binary
strip src/Main/maude

# Generate the release package
zip -j9X "Maude-$(zip_version)-linux-x86_64.zip" src/Main/maude ../src/Main/*.maude ../src/Main/*.sty

# Run the tests
make check
