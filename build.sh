function usage {
    echo "Usage: $(basename $0) [--debug] ( linux | emscripten )"
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
        linux) BUILD_LINUX=1
            ;;
        emscripten) BUILD_EM=1
            ;;
        --debug_em) DEBUG_EM=1
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

if [ $BUILD_LINUX ] ; then
  g++ -Isrc test.cpp -o test
  g++ -Isrc test.cpp -g -o test_d
fi

MEMORY_OPTION=' -s TOTAL_MEMORY=268435456 -s ALLOW_MEMORY_GROWTH=1 '

if [ $BUILD_EM ]; then
  echo "Building jsfeat with emscripten..."
  emcc -Isrc src/jsfeat.cpp -r -o build/libjsfeat.bc
  echo "Linking libs and final emscripten output."
  emcc -Isrc build/libjsfeat.bc emscripten/webarkitJsfeat.cpp -sEXPORTED_FUNCTIONS=_Grayscale,_Grayscale_m -sEXPORTED_RUNTIME_METHODS=cwrap $MEMORY_OPTION --post-js js/post_bindings.api.js --bind -o build/grayscale.js
fi

if [ $DEBUG_EM ]; then
  echo "Building jsfeat with emscripten..."
  emcc -Isrc src/jsfeat.cpp -DDEBUG_EM -r -o build/libjsfeat_debug.bc
  echo "Linking libs and final emscripten output."
  emcc -Isrc -DDEBUG_EM build/libjsfeat_debug.bc emscripten/webarkitJsfeat.cpp -sEXPORTED_FUNCTIONS=_Grayscale,_Grayscale_m -sEXPORTED_RUNTIME_METHODS=cwrap -g -O0 $MEMORY_OPTION -sASSERTIONS=1 --profiling -s DEMANGLE_SUPPORT=1 --post-js js/post_bindings.api.js --bind -o build/grayscale_debug.js
fi