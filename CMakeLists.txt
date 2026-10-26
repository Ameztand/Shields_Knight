# Shields_Knight —— CMake 构建描述
# 放到项目根目录（和 .gitignore / README.md 同级）
# 用法：configure → build → 运行（VS Code 用 mingw 预设，VS 2022 用 msvc 预设）
# 本文件与编译器无关：MinGW g++ 与 MSVC 共用同一份源码，差异全部走下面的 if(MSVC) 分支。

cmake_minimum_required(VERSION 3.21)

project(Shields_Knight
        VERSION 0.1.0.1
        DESCRIPTION "SFML 3.1 / C++17 desktop game shell"
        LANGUAGES CXX)

# ---------------- 语言标准 ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 单配置生成器（MinGW Makefiles / Ninja）默认没有 build type，这里兜底。
# 用 Release 而不是 Debug：SFML 官方包的 debug 库（*d-3.dll）通常与 MinGW 不兼容。
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---------------- 按编译器分支（MinGW g++ / MSVC 共用同一份源码） ----------------
if(MSVC)
    # 开发期：Release 但不优化、带调试信息（等价于 g++ 的 -O0 -g）。/Z7 免共享 PDB。
    set(SK_DEV_COMPILE "$<$<CONFIG:Release>:/Od>" "$<$<CONFIG:Release>:/Z7>")
    set(SK_DEV_LINK    "$<$<CONFIG:Release>:/DEBUG>")
    # 源码是无 BOM 的 UTF-8 且含中文字面量：
    #   不加 /source-charset:utf-8，MSVC 会按 936 读 UTF-8，报 C4819 + C2001，直接编不过；
    #   执行字符集用 936(GBK)，复刻 MinGW 的 -fexec-charset=GBK，保证 GBK 控制台中文不乱码。
    # 注意：不要换成 /utf-8 —— 那会把执行字符集变成 UTF-8，CP936 控制台里全是乱码。
    set(SK_CHARSET /source-charset:utf-8 /execution-charset:.936)
else()
    set(SK_DEV_COMPILE "$<$<CONFIG:Release>:-O0>" "$<$<CONFIG:Release>:-g>")
    set(SK_DEV_LINK "")
    set(SK_CHARSET -fexec-charset=GBK)
endif()

# ---------------- 依赖 ----------------
# 路径来自 CMakePresets.json 的 CMAKE_PREFIX_PATH：
#   MinGW 预设 -> D:/vsCodeTools/SFML-3.1.0          （MinGW 包，lib 下是 *.a）
#   MSVC  预设 -> D:/vsCodeTools/SFML-3.1.0-MSVC/SFML-3.1.0  （vc17 包，lib 下是 *.lib）
find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics)

# ---------------- 源文件 ----------------
# 自动收集 src 下所有 .cpp（等价于原来的 Get-ChildItem -Recurse *.cpp）
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

# ---------------- 编译选项（两个工具链各自的“开发期”开关） ----------------
target_compile_options(Shields_Knight PRIVATE ${SK_DEV_COMPILE} ${SK_CHARSET})
if(SK_DEV_LINK)
    target_link_options(Shields_Knight PRIVATE ${SK_DEV_LINK})
endif()

# ---------------- 产物位置 ----------------
set_target_properties(Shields_Knight PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ---------------- 运行期资源 ----------------
# renderTEST() 用相对路径 ./pic/test.png，且需要 SFML 的 dll 在 exe 旁边
get_filename_component(SFML_PREFIX "${SFML_DIR}/../../.." ABSOLUTE)
# 只拷真正链接的 3 个模块；audio/network 用不到，省约 4MB
# 两个包的 dll 同名（sfml-graphics-3.dll 等），所以这段两家通用
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

message(STATUS "Shields_Knight: 编译器 = ${CMAKE_CXX_COMPILER_ID} | ${CMAKE_CXX_COMPILER}")
message(STATUS "Shields_Knight: ${CMAKE_BUILD_TYPE} -> ${CMAKE_BINARY_DIR}/bin/Shields_Knight.exe")
message(STATUS "Shields_Knight: SFML_DIR = ${SFML_DIR}")

# =====================================================================
# 发布版 —— 要打包发给别人时，按这三步切过来，其余不用动
#
#   1) 把上面 if(MSVC)/else() 分支里的 /Od /Z7（MSVC）和 -O0 -g（g++）去掉
#      → 恢复两家各自的默认 Release 优化
#
#   2) 别动字符集那两项（/source-charset:utf-8 /execution-charset:.936 与
#      -fexec-charset=GBK），中文控制台还要用
#
#   3) 取消注释下面两行中的对应分支 → 静态链接运行时，
#      MinGW：exe 不再依赖 libstdc++-6.dll / libgcc_s_seh-1.dll
#      MSVC ：切 /MT 需要 SFML 也是 /MT 编译的，官方包是 /MD，用前先确认
# =====================================================================
# target_compile_options(Shields_Knight PRIVATE -O2)          # g++
# target_link_options(Shields_Knight PRIVATE -static)         # g++
