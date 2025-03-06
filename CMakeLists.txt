cmake_minimum_required(VERSION 3.13)

project(
    test
    VERSION      "1.0.0"
    DESCRIPTION  "pspfunki"
    LANGUAGES    C CXX
)
file(
    COPY        "${PROJECT_SOURCE_DIR}/assets/"
    DESTINATION "${PROJECT_BINARY_DIR}/assets" NO_SOURCE_PERMISSIONS
    PATTERN     "SOURCE_FILES" EXCLUDE
    PATTERN     "*.t3s" EXCLUDE
    PATTERN     "*.png" EXCLUDE
)
file(
    GLOB_RECURSE sources
    RELATIVE     "${PROJECT_SOURCE_DIR}"
    "src/*.c" "src/*.cpp"
)

add_executable(${PROJECT_NAME} ${sources})
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11 cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
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
    ARGS -i "${PROJECT_SOURCE_DIR}/${input}" -o "${output}"
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