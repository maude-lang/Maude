#!/bin/sh
#
# Build script for Maude (macOS)

BUDDY_URL="https://github.com/utwente-fmt/buddy/releases/download/v2.4/buddy-2.4.tar.gz"
YICES_URL="https://github.com/SRI-CSL/yices2/archive/refs/tags/yices-2.7.0.tar.gz"
HSLIB="$(pwd)/deps"

# These variables depend on whether the build is arm64 or x86_64
RELEASE_SUFFIX="$1"
HOMEBREW_PREFIX="$2"

set -xe

# Common definitions
. "$(dirname "$0")/common.sh"

# Install required libraries using Homebrew
brew install libsigsegv libtecla gperf bison automake autoconf libtool

# Prefer brew bison over macOS'
export PATH="$HOMEBREW_PREFIX/opt/bison/bin:$PATH"
echo 'export PATH="$HOMEBREW_PREFIX/opt/bison/bin:$PATH"' >> /Users/runner/.bash_profile

# Directory to install dependencies
mkdir "$HSLIB"

make_buddy() {

	# Download BuDDy
	curl -JLO "$BUDDY_URL"
	tar xvf buddy-2.4.tar.gz
	cd buddy-2.4

	./configure LDFLAGS=-lm \
	CFLAGS="-fno-stack-protector -O3 -mmacosx-version-min=10.13" \
	CXXFLAGS="-fno-stack-protector -O3 -mmacosx-version-min=10.13 -std=c++11" \
	--disable-shared \
	--disable-dependency-tracking \
	--prefix=/

	make
	# make check
	chmod a+x tools/install-sh
	DESTDIR="$HSLIB" make install
	cd ..
}

make_yices() {

	# Download Yices2
	curl -JLO "$YICES_URL"
	tar xvf yices2-yices-2.7.0.tar.gz
	cd yices2-yices-2.7.0

	autoconf

	gmp_path=$(find "$HOMEBREW_PREFIX" -name 'libgmp.a' -print -quit)
	gmp_include_dir=$(dirname "$(find "$HOMEBREW_PREFIX" -name 'gmp.h' -print -quit)")

	./configure \
	--with-static-gmp="$gmp_path" \
	--with-static-gmp-include-dir="$gmp_include_dir" \
	CFLAGS="-fno-stack-protector -O3 -mmacosx-version-min=10.13 -I$HOMEBREW_PREFIX/include" \
	LDFLAGS="-L$HOMEBREW_PREFIX/lib" \
	--prefix=/

	make
	#make check
	DESTDIR="$HSLIB" make install
	find "$HSLIB" -name '*.dylib' -delete
	cd ..

}

make_buddy
make_yices

# Change version for development versions
if ! is_release; then
	sed -i '' -e "s/AC_INIT(\[Maude\],\[[^]]*\]/AC_INIT([Maude],[git-$(git rev-parse --short=6 HEAD)]/" configure.ac
fi

# Build Maude
autoreconf -i
mkdir macos
cd macos

../configure --with-smt=yices2 \
	CXXFLAGS="-Wall -O3 -fno-stack-protector -fstrict-aliasing -mmacosx-version-min=10.13 -std=c++17 -I$HSLIB/include -I$HOMEBREW_PREFIX/include" \
	LDFLAGS="-Wl,-S -L$HSLIB/lib -L$HOMEBREW_PREFIX/lib" \
	GMP_LIBS="$HOMEBREW_PREFIX/opt/gmp/lib/libgmpxx.a $HOMEBREW_PREFIX/opt/gmp/lib/libgmp.a" \
	TECLA_LIBS="$HOMEBREW_PREFIX/opt/libtecla/lib/libtecla.a -lncurses" \
	LIBSIGSEGV_LIB="$HOMEBREW_PREFIX/opt/libsigsegv/lib/libsigsegv.a" \
	CC="clang" \
	CXX="clang++" \
	--disable-dependency-tracking

make -j2

# Generate the release package
zip -Xj9 "Maude-$(zip_version)-macos-$RELEASE_SUFFIX.zip" src/Main/maude ../src/Main/*.maude ../src/Main/*.sty

# Run the tests
make check
