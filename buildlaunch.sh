rm -r build
/opt/devkitpro/portlibs/3ds/bin/arm-none-eabi-cmake -S . -B build
cmake --build ./build
'/Users/bruno/Desktop/citra-emu/citra' ./build/test.3dsx