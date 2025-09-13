@echo off
REM LuduScript Windows构建脚本
REM 使用CMake或Makefile构建项目

setlocal enabledelayedexpansion

set "PROJECT_DIR=%~dp0"
set "BUILD_DIR=%PROJECT_DIR%build"
set "BIN_DIR=%PROJECT_DIR%bin"

echo LuduScript 构建脚本
echo ==================

REM 检查参数
if "%1"=="clean" (
    echo 清理构建文件...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
    if exist "%BIN_DIR%" rmdir /s /q "%BIN_DIR%"
    echo 清理完成！
    goto :end
)

if "%1"=="help" (
    echo 用法: build.bat [选项]
    echo.
    echo 选项:
    echo   clean    - 清理构建文件
    echo   cmake    - 使用CMake构建
    echo   make     - 使用Makefile构建
    echo   test     - 运行测试
    echo   help     - 显示此帮助信息
    echo.
    echo 默认使用CMake构建（如果可用），否则使用Makefile
    goto :end
)

if "%1"=="test" (
    echo 运行测试...
    if exist "%BIN_DIR%\luduscript.exe" (
        "%BIN_DIR%\luduscript.exe" scripts\generate_poker_deck.gen --output output\test_poker.json
        "%BIN_DIR%\luduscript.exe" scripts\generate_werewolf_cards.gen --output output\test_werewolf.json
        echo 测试完成！
    ) else (
        echo 错误: 找不到可执行文件，请先构建项目
        exit /b 1
    )
    goto :end
)

REM 检查CMake是否可用
cmake --version >nul 2>&1
if %errorlevel%==0 (
    set "USE_CMAKE=1"
) else (
    set "USE_CMAKE=0"
)

REM 检查Make是否可用
make --version >nul 2>&1
if %errorlevel%==0 (
    set "USE_MAKE=1"
) else (
    set "USE_MAKE=0"
)

REM 根据参数或可用性选择构建系统
if "%1"=="cmake" (
    if !USE_CMAKE!==0 (
        echo 错误: CMake不可用
        exit /b 1
    )
    goto :cmake_build
)

if "%1"=="make" (
    if !USE_MAKE!==0 (
        echo 错误: Make不可用
        exit /b 1
    )
    goto :make_build
)

REM 默认选择构建系统
if !USE_CMAKE!==1 (
    echo 使用CMake构建...
    goto :cmake_build
) else if !USE_MAKE!==1 (
    echo 使用Makefile构建...
    goto :make_build
) else (
    echo 错误: 没有找到可用的构建系统（CMake或Make）
    echo 请安装CMake或Make，或者手动编译源文件
    exit /b 1
)

:cmake_build
echo 创建构建目录...
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

echo 配置项目...
cmake .. -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo CMake配置失败！
    exit /b 1
)

echo 构建项目...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo CMake构建失败！
    exit /b 1
)

echo CMake构建完成！
echo 可执行文件位于: %BUILD_DIR%\bin\luduscript.exe
goto :end

:make_build
echo 使用Makefile构建...
make release
if %errorlevel% neq 0 (
    echo Make构建失败！
    exit /b 1
)

echo Make构建完成！
echo 可执行文件位于: %BIN_DIR%\luduscript.exe
goto :end

:end
echo.
echo 构建脚本执行完成！
pause