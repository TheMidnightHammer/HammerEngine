@echo off
SET MSYS2_PATH=C:\msys64
SET MINGW_BIN=%MSYS2_PATH%\mingw64\bin

:: 1. Ask for Demo Path
echo --------------------------------------------------
set /p DEMO_PATH="Enter (or drag and drop) your demo folder: "
set DEMO_PATH=%DEMO_PATH:"=%
echo --------------------------------------------------

if not exist "%DEMO_PATH%" (
    echo [ERROR] Path not found.
    pause
    exit /b
)

:: 2. Download and Install MSYS2 if missing
if not exist "%MSYS2_PATH%" (
    echo [INFO] MSYS2 not found. Downloading...
    :: Added -UseBasicParsing for better compatibility
    powershell -Command "Invoke-WebRequest -Uri https://github.com/msys2/msys2-installer/releases/download/2024-01-13/msys2-x86_64-20240113.exe -OutFile msys2_installer.exe -UseBasicParsing"
    
    echo [INFO] Unblocking installer from Windows Defender...
    powershell -Command "Unblock-File -Path .\msys2_installer.exe"

    echo [INFO] Running Installer. If Defender pops up, click 'More Info' -> 'Run Anyway'.
    start /wait msys2_installer.exe --confirm-command --accept-messages --root %MSYS2_PATH%
    del msys2_installer.exe
)

:: 3. Fix the "Lock" Error & Install Libraries
echo [INFO] Clearing any old package locks...
if exist "%MSYS2_PATH%\var\lib\pacman\db.lck" del "%MSYS2_PATH%\var\lib\pacman\db.lck"

echo [INFO] Installing GCC, Vulkan, GLFW, and GLM...
%MSYS2_PATH%\usr\bin\bash.exe -lc "pacman -S --noconfirm --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-vulkan-loader mingw-w64-x86_64-glfw mingw-w64-x86_64-glm"

:: 4. Build
set PATH=%MINGW_BIN%;%PATH%
cd /d "%DEMO_PATH%"
if not exist build mkdir build
cd build

cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
mingw32-make

echo ---------------------------------------
echo [SUCCESS] Build complete!
pause