#!/bin/sh

# Standard bootstrap script for Autotools
# Use autoreconf to regenerate the build system

echo "Bootstrapping project..."
mkdir -p build-aux m4
autoreconf --install --verbose --force

echo "Ready to run ./configure"
