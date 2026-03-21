@echo off
:: --- AUTO-ELEVATE TO ADMIN ---
setlocal DisplayName=VoxelEngineBuilder
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    powershell Start-Process -FilePath "%0" -Verb RunAs
    exit /b
)
pushd "%CD%"
:: -----------------------------

SET MSYS2_PATH=C:\msys64
SET MINGW_BIN=%MSYS2_PATH%\mingw64\bin

:: 1. Ask for the path (Supports drag-and-drop)
echo --------------------------------------------------
set /p DEMO_PATH="Drag your demo folder here and press ENTER: "
set DEMO_PATH=%DEMO_PATH:"=%
echo --------------------------------------------------

:: 2. Install MSYS2 if missing
if not exist "%MSYS2_PATH%" (
    echo [INFO] MSYS2 not found. Downloading...
    powershell -Command "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri https://github.com/msys2/msys2-installer/releases/download/2024-05-07/msys2-x86_64-20240507.exe -OutFile msys2_installer.exe"
    echo [INFO] Installing MSYS2 silently... Please wait.
    start /wait msys2_installer.exe --confirm-command --accept-messages --root %MSYS2_PATH%
    del msys2_installer.exe
)

:: 3. Install GCC & Graphics Libraries
echo [INFO] Updating packages and installing libraries...
"%MSYS2_PATH%\usr\bin\bash.exe" -lc "pacman -S --noconfirm --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-vulkan-loader mingw-w64-x86_64-glfw mingw-w64-x86_64-glm"

:: 4. Build the Project
set PATH=%MINGW_BIN%;%MSYS2_PATH%\usr\bin;%PATH%
cd /d "%DEMO_PATH%"

if not exist build mkdir build
cd build

echo [INFO] Running CMake...
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
mingw32-make

echo ---------------------------------------
if exist "voxel_Engine.exe" (
    echo [SUCCESS] Your engine is ready!
) else (
    echo [ERROR] Build failed. Review the errors above.
)
echo ---------------------------------------
pause