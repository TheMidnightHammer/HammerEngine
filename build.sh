#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Detect OS
if [ -f /etc/os-release ]; then
    . /etc/os-release
    DISTRO_ID=$ID
    DISTRO_LIKE=$ID_LIKE
else
    echo "Unsupported Linux distribution (cannot find /etc/os-release)."
    exit 1
fi

echo "Detected Distribution: $DISTRO_ID (Like: $DISTRO_LIKE)"
echo "----INSTALLING REQUIREMENTS----"

# Function to handle package installation based on Distro family
case "$DISTRO_ID" in
    # --- DEBIAN / UBUNTU / MINT / POP!_OS ---
    debian|ubuntu|linuxmint|pop|kali|neon)
        sudo apt update -y
        sudo apt-get install -y \
            build-essential cmake \
            libvulkan-dev vulkan-tools vulkan-validationlayers spirv-tools \
            libstb-dev libglm-dev libglfw3-dev 
        ;;

    # --- FEDORA / RHEL / CENTOS / ALMA ---
    fedora|rhel|centos|almalinux|rocky)
        # Note: 'stb-devel' and 'glm-devel' might require enabling extra repos (like RPMFusion) on some RHEL clones,
        # but are usually standard in Fedora.
        sudo dnf check-update || true # dnf check-update returns exit code 100 if updates are available
        sudo dnf install -y \
            gcc-c++ cmake make \
            vulkan-loader-devel vulkan-tools vulkan-validation-layers-devel spirv-tools \
            stb-devel glm-devel glfw-devel
        ;;

    # --- ARCH LINUX / MANJARO / ENDEAVOUROS ---
    arch|manjaro|endeavouros|garuda)
        sudo pacman -Syu --noconfirm
        sudo pacman -S --noconfirm \
            base-devel cmake \
            vulkan-icd-loader vulkan-tools vulkan-validation-layers spirv-tools \
            stb glm glfw
        ;;

    # --- OPENSUSE ---
    opensuse-leap|opensuse-tumbleweed)
        sudo zypper refresh
        # -t pattern devel_basis installs basic build tools (gcc, make, etc)
        sudo zypper install -y -t pattern devel_basis
        sudo zypper install -y \
            cmake \
            vulkan-loader-devel vulkan-tools vulkan-validationlayers spirv-tools \
            stb-devel glm-devel libglfw3-devel
        ;;

    # --- UNSUPPORTED ---
    *)
        echo "Sorry, your distribution '$DISTRO_ID' is not explicitly supported by this script."
        echo "You will need to install Vulkan, GLFW, GLM, STB, and CMake manually."
        exit 1
        ;;
esac

echo -e "\nDependencies installed successfully.\n"

# --- BUILD SECTION ---

read -p "Enter the path to the project: " path

# Validate path exists
if [ ! -d "$path" ]; then
    echo "Error: Directory '$path' does not exist."
    exit 1
fi

cd "$path"

# Clean build directory if it exists to ensure fresh cache, or make it if it doesn't
if [ -d "build" ]; then
    echo "Build directory exists. Cleaning..."
    rm -rf build/*
else
    mkdir build
fi

cd build

echo "----CONFIGURING CMAKE----"
cmake ..

echo "----BUILDING----"
make -j "$(nproc)"
mv voxel_Engine ../voxel_Engine

echo "----DONE----"
