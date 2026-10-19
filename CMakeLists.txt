# Shields_Knight —— CMake 构建描述
# 放到项目根目录（和 .gitignore / DEVLOG.md 同级）
# 用法见《CMake接入说明.md》：三条命令 configure → build → 运行

cmake_minimum_required(VERSION 3.21)

project(Shields_Knight
        VERSION 0.1.0.1
        DESCRIPTION "SFML 3.1 / C++17 desktop game shell"
        LANGUAGES CXX)

# ---------------- 语言标准 ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# 开发期：仍用 Release 配置（为了链接 SFML 的 release 库），但关闭优化。
# -O0 断点准确、编译也更快；发布时见文件末尾的"发布版"注释块。
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -g")

# 单配置生成器（MinGW Makefiles）默认没有 build type，这里兜底。
# 用 Release 而不是 Debug：SFML 官方包的 debug 库（*d-3.dll）通常与 MinGW 不兼容。
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---------------- 依赖 ----------------
# 路径来自 CMakePresets.json 的 CMAKE_PREFIX_PATH
find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics)

# ---------------- 源文件 ----------------
# 自动收集 src 下所有 .cpp（等价于你原来 tasks.json 里的 Get-ChildItem -Recurse *.cpp）
# CONFIGURE_DEPENDS：新增 .cpp 后会自动重新 configure，不用手改这份文件
file(GLOB_RECURSE SHIELDS_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(Shields_Knight ${SHIELDS_SOURCES})

target_include_directories(Shields_Knight PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/input
    ${CMAKE_SOURCE_DIR}/src/render
    ${CMAKE_SOURCE_DIR}/src/logic)

target_link_libraries(Shields_Knight PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System)

# ---------------- 编译选项 ----------------
if(MINGW)
    # -GBK      复刻 tasks.json 的 -fexec-charset=GBK，控制台中文不乱码
    target_compile_options(Shields_Knight PRIVATE  -fexec-charset=GBK)
endif()

# ---------------- 产物位置 ----------------
set_target_properties(Shields_Knight PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ---------------- 运行期资源 ----------------
# renderTEST() 用相对路径 ./pic/test.png，且需要 SFML 的 dll 在 exe 旁边
get_filename_component(SFML_PREFIX "${SFML_DIR}/../../.." ABSOLUTE)
# 只拷真正链接的 3 个模块；audio/network 用不到，省约 4MB
set(SFML_DLLS
    "${SFML_PREFIX}/bin/sfml-graphics-3.dll"
    "${SFML_PREFIX}/bin/sfml-window-3.dll"
    "${SFML_PREFIX}/bin/sfml-system-3.dll")

add_custom_command(TARGET Shields_Knight POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_SOURCE_DIR}/pic" "$<TARGET_FILE_DIR:Shields_Knight>/pic"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            ${SFML_DLLS} "$<TARGET_FILE_DIR:Shields_Knight>"
    COMMENT "Copy pic/ and SFML runtime DLLs next to the exe")

message(STATUS "Shields_Knight: ${CMAKE_BUILD_TYPE} -> ${CMAKE_BINARY_DIR}/bin/Shields_Knight.exe")

# =====================================================================
# 发布版 —— 要打包发给别人时，按这三步切过来，其余不用动
#
#   1) 把上面那行  set(CMAKE_CXX_FLAGS_RELEASE "-O0 -g")
#      注释掉 → 恢复 CMake 默认 Release（-O3 -DNDEBUG）
#
#   2) 注释掉 MINGW 段里的 -fexec-charset=GBK？**不要**，中文控制台还要用
#
#   3) 取消注释下面两行 → 静态链接 MinGW 运行时，
#      exe 不再依赖 libstdc++-6.dll / libgcc_s_seh-1.dll
#
#   改完 Ctrl+Shift+P → CMake: 配置，然后 F7，产物仍在 build\bin
# =====================================================================
# target_compile_options(Shields_Knight PRIVATE -O2)
# target_link_options(Shields_Knight PRIVATE -static)
#
# # 一键打包：把 build\bin 整个压成 zip，放在 build\ 下
# add_custom_target(dist
#     COMMAND "${CMAKE_COMMAND}" -E tar cf
#             "${CMAKE_BINARY_DIR}/Shields_Knight_${PROJECT_VERSION}.zip"
#             --format=zip .
#     WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#     DEPENDS Shields_Knight
#     COMMENT "打包 build/bin -> Shields_Knight_${PROJECT_VERSION}.zip")
