cmake_minimum_required(VERSION 3.13)

project(
    terraedit3d
    VERSION      "1.0.0"
    DESCRIPTION  "terraria inverntory editor"
    LANGUAGES    C CXX
)
file(
    COPY        "${PROJECT_SOURCE_DIR}/assets/"
    DESTINATION "${PROJECT_BINARY_DIR}/assets" NO_SOURCE_PERMISSIONS
    PATTERN     "SOURCE_FILES" EXCLUDE
    PATTERN     "*.t3s" EXCLUDE
)
file(
    GLOB_RECURSE sources
    RELATIVE     "${PROJECT_SOURCE_DIR}"
    "src/*.c" "src/*.cpp"
)
find_program(MAKEROM makerom)
find_program(BANNERTOOL bannertool)

add_executable(${PROJECT_NAME} ${sources})
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11 cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-strict-aliasing)
target_link_libraries(${PROJECT_NAME} PRIVATE -lcitro2d -lcitro3d -lctru -lm)

#create t3x
file(
  GLOB_RECURSE tex
  RELATIVE     "${PROJECT_SOURCE_DIR}"
  "assets/*.t3s"
)

foreach(input IN LISTS tex)
  cmake_path(GET input PARENT_PATH parent)
  cmake_path(GET input STEM LAST_ONLY name)

  set(output "${PROJECT_BINARY_DIR}/${parent}/${name}.t3x")
  list(APPEND builttex "${output}")

  add_custom_command(
    OUTPUT  "${output}"
    COMMAND tex3ds
    ARGS --border transparent -i "${PROJECT_SOURCE_DIR}/${input}" -o "${output}"
    DEPENDS "${PROJECT_SOURCE_DIR}/${input}"
    COMMENT "Building texture ${output}"
)

endforeach()

add_custom_target(tex ALL DEPENDS ${builttex})

ctr_generate_smdh(${PROJECT_NAME}.smdh
  NAME "${PROJECT_NAME}"
  DESCRIPTION "${PROJECT_VERSION}"
  AUTHOR "UNSTOP4BLE"
)

ctr_create_3dsx(${PROJECT_NAME}
  SMDH ${PROJECT_NAME}.smdh
  ROMFS "${PROJECT_BINARY_DIR}/assets"
)

#banner
add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bnr
                   COMMAND ${BANNERTOOL} makebanner -ci ${PROJECT_SOURCE_DIR}/cia/banner.cgfx -a ${PROJECT_SOURCE_DIR}/cia/mus.wav -o ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bnr)

add_custom_target(build_bnr ALL
                  DEPENDS ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bnr)
#cia
add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.cia
                   COMMAND ${MAKEROM} -f cia -o ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.cia -rsf ${PROJECT_SOURCE_DIR}/cia/cia.rsf -target t -exefslogo -elf ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf -icon ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.smdh -banner ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bnr) #-major ${LIB_VERSION_MAJOR} -minor ${LIB_VERSION_MINOR} -micro ${LIB_VERSION_PATCH})
add_custom_target(build_cia ALL
                  DEPENDS ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.cia)