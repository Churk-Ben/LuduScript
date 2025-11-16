cmake_minimum_required(VERSION 3.16)
project(LuduScript VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# 收集源文件
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"
)

# 收集头文件
file(GLOB_RECURSE HEADERS 
    "include/*.h"
    "include/*.hpp"
)

# 创建可执行文件
add_executable(luduscript ${SOURCES} ${HEADERS})

# 包含目录
target_include_directories(luduscript PRIVATE
    ${CMAKE_SOURCE_DIR}/include  # 你的 parser.h 所在目录
)

# 设置目标属性
set_target_properties(luduscript PROPERTIES
    OUTPUT_NAME "luduscript"
    DEBUG_POSTFIX "_d"
)

# 编译定义
target_compile_definitions(luduscript PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# 安装规则
install(TARGETS luduscript
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 安装头文件
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# 安装脚本示例
install(DIRECTORY scripts/
    DESTINATION share/luduscript/scripts
    FILES_MATCHING PATTERN "*.gen" PATTERN "*.lds"
)

# 安装示例文件
install(DIRECTORY examples/
    DESTINATION share/luduscript/examples
)

# CPack配置
set(CPACK_PACKAGE_NAME "LuduScript")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "LuduScript - A domain-specific language for game card generation")
set(CPACK_PACKAGE_VENDOR "LuduScript Project")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

include(CPack)

# 添加自定义目标用于清理输出
add_custom_target(clean-output
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/output
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/output
    COMMENT "Cleaning output directory"
)
