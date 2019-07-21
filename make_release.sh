#!/bin/bash

set -eu

RST_ROOT=$(dirname "$0")
RELEASE_DIR=$RST_ROOT/release
VERSION=$(git describe --tags --dirty --always --long --match '*')

print_status () {
  MSG=$1
  BLUE='\033[0;34m'
  NC='\033[0m'
  echo -e "${BLUE}${MSG}${NC}"
}

# Clean up the release directory
rm -r $RELEASE_DIR || true
mkdir $RELEASE_DIR

build () {
  TARGET_VERSION=$1

  print_status "building for $TARGET_VERSION"

  # Copy the version-specific hooks
  rm -r $RST_ROOT/hooks/ || true
  mkdir $RST_ROOT/hooks
  cp $RST_ROOT/$TARGET_VERSION/hooks.hks $RST_ROOT/hooks/

  # Copy the version-specific build files
  cp $RST_ROOT/$TARGET_VERSION/*.bin $RST_ROOT/
  cp $RST_ROOT/$TARGET_VERSION/Version.cmake $RST_ROOT/source/

  # Touch main.cpp to get an up-to-date build time
  touch $RST_ROOT/source/rst/main.cpp

  # Run the patcher
  Magikoopa --build --workdir $RST_ROOT/

  # Copy build output
  mkdir $RELEASE_DIR/$TARGET_VERSION
  flips -i $RST_ROOT/bak/code.bin $RST_ROOT/code.bin $RELEASE_DIR/$TARGET_VERSION/code.ips
  cp $RST_ROOT/code.bin $RST_ROOT/source/build/patched_code.bin
  cp $RST_ROOT/exheader*.bin $RELEASE_DIR/$TARGET_VERSION/

  # Clean up
  rm -r $RST_ROOT/loader/*.bin $RST_ROOT/loader/*.sym || true
  rm -r $RST_ROOT/*.bin $RST_ROOT/*.sym || true
  rm -r $RST_ROOT/bak $RST_ROOT/hooks || true
}

build v100

make_patch_for_secondary_version () {
  TARGET_VERSION=$1
  mkdir $RELEASE_DIR/$TARGET_VERSION
  cp $RELEASE_DIR/v100/exheader*.bin $RELEASE_DIR/$TARGET_VERSION/
  flips -b $RST_ROOT/$TARGET_VERSION/code.bin $RST_ROOT/source/build/patched_code.bin $RELEASE_DIR/$TARGET_VERSION/code.bps
}

if [ -z ${RST_SKIP_110+x} ]; then
  make_patch_for_secondary_version v101 &
  make_patch_for_secondary_version v110 &
  wait
fi

print_status "packing"

pushd $RELEASE_DIR
7z a mm3d_project_restoration_${VERSION}.7z .
popd
