#!/bin/sh
#
# Build script for Maude (common)

# Use the date of the last commit for build reproducibility
export SOURCE_DATE_EPOCH=$(git log -1 --format=%at)

# Version string to include in the ZIP filename
zip_version() {
	case $TAG_NAME in
		Maude*)
			echo "${TAG_NAME#Maude}" ;; # only the version number
		Alpha*)
			echo "a${TAG_NAME#A}" ;; # lowercase alpha
		*)
			echo "$TAG_NAME" ;; # the tag name as is
	esac
}

# Whether this is a release or a development version
is_release() {
	case $TAG_NAME in
		Maude*|Alpha*)
			return 0 ;;
		*)
			return 1 ;;
	esac
}
