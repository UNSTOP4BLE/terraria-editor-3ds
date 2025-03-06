rm -r build
/opt/devkitpro/portlibs/3ds/bin/arm-none-eabi-cmake -S . -B build
cmake --build ./build