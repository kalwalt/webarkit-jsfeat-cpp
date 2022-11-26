#!/usr/bin/env bash
function usage {
    echo "Usage: $(basename $0) [--debug | --clean-em] ( libar | linux | emscripten | emscripten-all )"
    exit 1
}

if [ $# -eq 0 ]; then
    usage
fi

# -e = exit on errors
set -e

# Parse parameters
while test $# -gt 0
do
    case "$1" in
        libar) BUILD_LIBAR=1
            ;;
        linux) BUILD_LINUX=1
            ;;
        emscripten) BUILD_EM=1
            ;;
        emscripten-all) BUILD_EM_ALL=1
            ;;
        --clean-em) CLEAN_EM=1
            ;;
        --*) echo "bad option $1"
            usage
            ;;
        *) echo "bad argument $1"
            usage
            ;;
    esac
    shift
done

# Set OS-dependent variables.
OS=`uname -s`
ARCH=`uname -m`
TAR='/usr/bin/tar'
if [ "$OS" = "Linux" ]
then
    CPUS=`/usr/bin/nproc`
    TAR='/bin/tar'
    # Identify Linux OS. Sets useful variables: ID, ID_LIKE, VERSION, NAME, PRETTY_NAME.
    source /etc/os-release
    # Windows Subsystem for Linux identifies itself as 'Linux'. Additional test required.
    if grep -qE "(Microsoft|WSL)" /proc/version &> /dev/null ; then
        OS='Windows'
    fi
else
    CPUS=1
fi

if [ $BUILD_LIBAR ] ; then
  npm run build
fi

if [ $BUILD_LINUX ] ; then
  g++ -std=gnu++17 -Isrc test.cpp -o test
  g++ -std=gnu++17 -Isrc test.cpp -g -o test_d
fi

if [ $BUILD_EM ]; then 
  echo "Entering in build folder"
  cd build
  echo "Building jsfeatcpp.js  with emscripten (emcmake)..."
  emcmake cmake .. -DCMAKE_BUILD_TYPE="Release"
  echo "Running command make..."
  make
  echo "Building jsfeatcpp_debug.js  with emscripten (emcmake)..."
  emcmake cmake .. -DCMAKE_BUILD_TYPE="Debug"
  echo "Running command make..."
  make
  echo "Build completed."
fi

if [ $BUILD_EM_ALL ]; then 
  echo "Building libar libs"
  npm run build
  echo "Done!"
  echo "preparing to build jsfeat libs..."
  echo "Entering in build folder"
  cd build
  echo "Building jsfeatcpp.js  with emscripten (emcmake)..."
  emcmake cmake .. -DCMAKE_BUILD_TYPE="Release"
  echo "Running command make..."
  make
  echo "Building jsfeatcpp_debug.js  with emscripten (emcmake)..."
  emcmake cmake .. -DCMAKE_BUILD_TYPE="Debug"
  echo "Running command make..."
  make
  echo "Build completed."
fi

if [ $CLEAN_EM ]; then
  echo "Entering in build folder"
  cd build
  echo "Cleaning build folder."
  rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile
  rm -rf ./jsfeatcpp.js ./jsfeatcpp_debug.js
  echo "Removed files."
fi