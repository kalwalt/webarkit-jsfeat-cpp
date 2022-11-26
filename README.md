# webarkit-jsfeat-cpp
c++ [jsfeat](https://github.com/inspirit/jsfeat) version to build a WASM version thanks to Emscripten.
The project is a Work in Progress, not all the features are released to make a comparison with the original jsfeat code. 
At the moment only some classes are implemented and a bunch of functions are under testing. Anyway the main idea is to provide c++ code to build a WASM version, it's not intended to be used as a c++ library.
Maybe this will become the future of [webarkit](https://github.com/webarkit)
## Classes
### **Matrix_t**
This class is responsible to hold different types of data, to be used by all classes.

### **Matrix_smart**
Experimental class very similar to Matrix_t but use shared_ptr in the core

### **Imgproc**
It is one of important classes to manipulate image data for image processing. For now we have these functions:
- gaussain_blur
- grayscale
- grayscale_m
- pyrdown
- equalize_histogram
- resample
- warp_affine

### **Orb**
Oriented FAST and rotated BRIEF (ORB) is one most important class for feature tracking. Ths feature is under testing.

### **Keypoint_t**
A class to store key points for image tracking algorithms.

### **Keypoints**
A class to store a vector of keypoint_t for image tracking algorithms.

### **Yape06**
Feature detector from Computer Vision Lab, Ecole Polytechnique Federale de Lausanne (EPFL), Switzerland.

### **Pyramid_t**
A class to handle multiple pyramidal images.

This project is quite experimental so do not expect great results. More classes will be implemented in the future.

### Useful functions

At the moment you can try `load_jpeg_data` function to load a jpeg file and get important jpeg data (width, height, dpi and raw data).
See grayscale_example or grayscale_m_example to see the function in action.
Try `yape06_detect` function as a compact version for the yape06 detect routine. See it in action in the  yape06_detect_video_example.html

## Libs
Libs are stored in `build` folder: 
- **jsfeatcpp.js** the Release lib.
- **jsfeatES6cpp.js** the ES6 Release lib.
- **jsfeatcpp_debug.js** the Debug lib.
- **jsfeatES6cpp_debug.js** the ES6 Debug lib.
## Usage
You can import the libs as a ES6 module in a script tag:

```html
<script type="module">
    import jsfeatCpp from "./../build/jsfeatcpp.js"
</script>
```

There isn't a npm package yet, so this is the only way.

or the non ES6 version :

```html
 <script src="../build/jsfeatcpp.js"></script>
```

but the Module object will be available only when it is full loaded, so wrap your code into the listener:

```js
window.addEventListener('jsfeatCpp-loaded', function (e) {
    //your code here...
    })
```

## Examples
Take a look at our examples in the examples folder:

- equalize_histogram_video_example.html
- gaussian_blur_video_example.html
- grayscale_example.html
- grayscale_m_example.html
- grayscale_rgba_example.html
- grayscale_video_example.html
- keypoint_t_test.html
- keypoints_test.html
- matrix_t_test.html
- orb_test.html
- pyrdown_video_example.html
- resample_video_example.html
- sample_orb_mixed.html
- yape06_video_example.html
- yape06_detect_video_example.html

Both examples use the debug version of the lib but, of course you can use the non-debug version as well.

## NPM
npm is used to build the `libar.bc` and `libar_debug.bc` libs inside the build folder. You don't need to run `npm install` but it is required to have node and npm installed globally.

## Building the library
If you make changes and/or if you make some enhancement to the C++ code ypu need to build the library. Run the shell script in the main folder:
For the first time run:
`./build.sh emscripten-all`
this will build all the libs. After that if you need to compile and build only jsfeat libs:
`./build.sh emscripten`
and the two libs will be compiled in the build directory.

If you run under Windows use the `./build.Unix.sh` script instead.

We used emsdk 3.1.20.