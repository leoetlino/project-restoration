export SHELL:=/bin/bash
export SHELLOPTS:=$(if $(SHELLOPTS),$(SHELLOPTS):)pipefail:errexit

.PHONY: build

.ONESHELL:
build:
		cd source
		mkdir build || true
		cd build
		cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -GNinja -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_TOOLCHAIN_FILE=../../DevkitArm3DS.cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCODEADDR=$(CODEADDR)
		ninja
		cp *.sym *.bin ../../

clean:
		cd source
		rm -rf build
