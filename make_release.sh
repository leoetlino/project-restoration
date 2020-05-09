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

  # Copy the version-specific build files
  cp $RST_ROOT/$TARGET_VERSION/*.bin $RST_ROOT/

  # Touch main.cpp to get an up-to-date build time
  touch $RST_ROOT/source/rst/main.cpp

  # Run the patcher
  Magikoopa --build --workdir $RST_ROOT/

  # Copy build output
  mkdir $RELEASE_DIR/$TARGET_VERSION
  flips -i $RST_ROOT/bak/code.bin $RST_ROOT/code.bin $RELEASE_DIR/$TARGET_VERSION/code.ips
  if [ -z ${RST_DEV+x} ]; then
    flips -b $RST_ROOT/bak/code.bin $RST_ROOT/code.bin $RELEASE_DIR/$TARGET_VERSION/code.bps
  fi
  cp $RST_ROOT/code.bin $RST_ROOT/source/build/patched_code.bin
  cp $RST_ROOT/source/build/patched_code.bin $RST_ROOT/source/build/patched_code_faster_aim.bin
  $RST_ROOT/make_aiming_speed_patch.py $RST_ROOT/source/build/patched_code_faster_aim.bin 1.50
  if [ -z ${RST_DEV+x} ]; then
    flips -i $RST_ROOT/bak/code.bin $RST_ROOT/source/build/patched_code_faster_aim.bin $RELEASE_DIR/$TARGET_VERSION/code_faster_aim.ips
    flips -b $RST_ROOT/bak/code.bin $RST_ROOT/source/build/patched_code_faster_aim.bin $RELEASE_DIR/$TARGET_VERSION/code_faster_aim.bps
  fi
  cp $RST_ROOT/exheader*.bin $RELEASE_DIR/$TARGET_VERSION/

  # Clean up
  rm -r $RST_ROOT/loader/*.bin $RST_ROOT/loader/*.sym || true
  rm -r $RST_ROOT/*.bin $RST_ROOT/*.sym || true
  rm -r $RST_ROOT/bak || true
}

build v100

make_patch_for_secondary_version () {
  TARGET_VERSION=$1
  mkdir $RELEASE_DIR/$TARGET_VERSION
  cp $RELEASE_DIR/v100/exheader*.bin $RELEASE_DIR/$TARGET_VERSION/
  flips -b $RST_ROOT/$TARGET_VERSION/code.bin $RST_ROOT/source/build/patched_code.bin $RELEASE_DIR/$TARGET_VERSION/code.bps &
  flips -b $RST_ROOT/$TARGET_VERSION/code.bin $RST_ROOT/source/build/patched_code_faster_aim.bin $RELEASE_DIR/$TARGET_VERSION/code_faster_aim.bps &
  wait
}

if [ -z ${RST_DEV+x} ]; then
  make_patch_for_secondary_version v101 &
  make_patch_for_secondary_version v110 &
  wait

  print_status "packing"

  pushd $RELEASE_DIR
  7z a mm3d_project_restoration_${VERSION}.7z .
  popd
fi
