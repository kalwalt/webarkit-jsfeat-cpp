# webarkit-jsfeat-cpp
c++ [jsfeat](https://github.com/inspirit/jsfeat) version to build a WASM version thanks to Emscripten.
The project is a Work in Progress, not all the features are released to make a comparison with the original jsfeat code. 
At the moment only a raw **matrix_t** class is implemented and a bunch of imgproc.grayscale's functions are under testing. Anyway the main idea is to provide c++ code to build a WASM version, it's not intended to be used as a c++ library.
## Classes
For now only **matrix_t** and the **imgproc** classes with some grayscale methods were implemented. This project is quite experimental so do not expect great results. More classes will be implemented in the future.

## Libs
Libs are stored in `build` folder: 
- **jsfeatcpp.js** the Release lib.
- **jsfeatcpp_debug.js** the Debug lib.
## Usage
You can import the libs as a ES6 module in a script tag:

```html
<script type="module">
    import jsfeatCpp from "./../build/jsfeatcpp.js"
</script>
```

There isn't a npm package yet, so this is the only way.

## Examples
Take a look at our examples in the examples folder:

- matrix_t_test.html
- grayscale_example.html

Both examples use the debug version of the lib but, of course you can use the non-debug version as well.

## Building the library
If you make changes and/or if you make some enhancement to the C++ code ypu need to build the library. Run the shell script in the main folder:
`./build.sh emscripten`
and the two libs will be compiled in the build directory.
We used emsdk 3.1.20.