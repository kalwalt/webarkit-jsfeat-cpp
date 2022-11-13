/*
 * Simple script for running emcc on ARToolKit
 * @author zz85 github.com/zz85
 * @author ThorstenBux github.com/ThorstenBux
 * @author Walter Perdan  @kalwalt github.com/kalwalt
 */

var exec = require("child_process").exec,
  path = require("path"),
  fs = require("fs"),
  os = require("os"),
  child;

const platform = os.platform();

var NO_LIBAR = false;

var arguments = process.argv;

for (var j = 2; j < arguments.length; j++) {
  if (arguments[j] == "--no-libar") {
    NO_LIBAR = true;
    console.log(
      "Building lib with --no-libar option, libar will be preserved."
    );
  }
}

var HAVE_NFT = 1;

var EMSCRIPTEN_ROOT = process.env.EMSCRIPTEN;
var WEBARKITLIB_ROOT =
  process.env.WEBARKITLIB_ROOT ||
  path.resolve(__dirname, "../emscripten/WebARKitLib");

if (!EMSCRIPTEN_ROOT) {
  console.log("\nWarning: EMSCRIPTEN environment variable not found.");
  console.log(
    'If you get a "command not found" error,\ndo `source <path to emsdk>/emsdk_env.sh` and try again.'
  );
}

var EMCC = EMSCRIPTEN_ROOT ? path.resolve(EMSCRIPTEN_ROOT, "emcc ") : "emcc ";
var EMPP = EMSCRIPTEN_ROOT ? path.resolve(EMSCRIPTEN_ROOT, "em++ ") : "em++ ";
var OPTIMIZE_FLAGS = " -Oz "; // -Oz for smallest size
var MEM = 256 * 1024 * 1024; // 64MB

var SOURCE_PATH = path.resolve(__dirname, "../emscripten/") + "/";
var OUTPUT_PATH = path.resolve(__dirname, "../build/") + "/";

if (!fs.existsSync(path.resolve(WEBARKITLIB_ROOT, "include/AR/config.h"))) {
  console.log("Renaming and moving config.h.in to config.h");
  fs.copyFileSync(
    path.resolve(WEBARKITLIB_ROOT, "include/AR/config.h.in"),
    path.resolve(WEBARKITLIB_ROOT, "include/AR/config.h")
  );
  console.log("Done!");
}

let ar_sources;

if (platform === "win32") {
  var glob = require("glob");
  function match(pattern) {
    var r = glob.sync("emscripten/WebARKitLib/lib/SRC/" + pattern);
    return r;
  }
  function matchAll(patterns, prefix = "") {
    let r = [];
    for (let pattern of patterns) {
      r.push(...match(prefix + pattern));
    }
    return r;
  }

  ar_sources = matchAll([
    "AR/arLabelingSub/*.c",
    "AR/*.c",
    "ARICP/*.c",
    "ARUtil/log.c",
    "ARUtil/file_utils.c",
  ]);
} else {
  ar_sources = [
    "AR/arLabelingSub/*.c",
    "AR/*.c",
    "ARICP/*.c",
    "ARUtil/log.c",
    "ARUtil/file_utils.c",
  ].map(function (src) {
    return path.resolve(__dirname, WEBARKITLIB_ROOT + "/lib/SRC/", src);
  });
}

var ar2_sources = [
  "handle.c",
  "imageSet.c",
  "jpeg.c",
  "marker.c",
  "featureMap.c",
  "featureSet.c",
  "selectTemplate.c",
  "surface.c",
  "tracking.c",
  "tracking2d.c",
  "matching.c",
  "matching2.c",
  "template.c",
  "searchPoint.c",
  "coord.c",
  "util.c",
].map(function (src) {
  return path.resolve(__dirname, WEBARKITLIB_ROOT + "/lib/SRC/AR2/", src);
});

var kpm_sources = [
  "kpmHandle.cpp",
  "kpmRefDataSet.cpp",
  "kpmMatching.cpp",
  "kpmResult.cpp",
  "kpmUtil.cpp",
  "kpmFopen.c",
  "FreakMatcher/detectors/DoG_scale_invariant_detector.cpp",
  "FreakMatcher/detectors/gaussian_scale_space_pyramid.cpp",
  "FreakMatcher/detectors/gradients.cpp",
  "FreakMatcher/detectors/harris.cpp",
  "FreakMatcher/detectors/orientation_assignment.cpp",
  "FreakMatcher/detectors/pyramid.cpp",
  "FreakMatcher/facade/visual_database_facade.cpp",
  "FreakMatcher/matchers/hough_similarity_voting.cpp",
  "FreakMatcher/matchers/freak.cpp",
  "FreakMatcher/framework/date_time.cpp",
  "FreakMatcher/framework/image.cpp",
  "FreakMatcher/framework/logger.cpp",
  "FreakMatcher/framework/timers.cpp",
].map(function (src) {
  return path.resolve(__dirname, WEBARKITLIB_ROOT + "/lib/SRC/KPM/", src);
});

var webarkit_sources = ["WebARKitLog.cpp"].map(function (src) {
  return path.resolve(__dirname, WEBARKITLIB_ROOT + "/lib/SRC/WebARKit/", src);
});

if (HAVE_NFT) {
  ar_sources = ar_sources
    .concat(ar2_sources)
    .concat(kpm_sources)
    .concat(webarkit_sources);
}

var DEFINES = " ";
if (HAVE_NFT) DEFINES += " -D HAVE_NFT";

var FLAGS = "" + OPTIMIZE_FLAGS;
FLAGS += " -Wno-warn-absolute-paths";
FLAGS += " -s TOTAL_MEMORY=" + MEM + " ";
FLAGS += " -s USE_ZLIB=1";
FLAGS += " -s USE_LIBJPEG=1";
FLAGS += " --memory-init-file 0"; // for memless file
FLAGS += ' -s EXPORTED_RUNTIME_METHODS=["FS"]';
FLAGS += " -s ALLOW_MEMORY_GROWTH=1";

var SIMD128_FLAGS = " -msimd128";

FLAGS += " --bind ";

/* DEBUG FLAGS */
var DEBUG_FLAGS = " -g ";
DEBUG_FLAGS += " -s ASSERTIONS=1 ";
DEBUG_FLAGS += " --profiling ";
DEBUG_FLAGS += " -s ALLOW_MEMORY_GROWTH=1";
DEBUG_FLAGS += "  -s DEMANGLE_SUPPORT=1 ";

var INCLUDES = [
  path.resolve(__dirname, WEBARKITLIB_ROOT + "/include"),
  OUTPUT_PATH,
  SOURCE_PATH,
  path.resolve(__dirname, WEBARKITLIB_ROOT + "/lib/SRC/KPM/FreakMatcher"),
]
  .map(function (s) {
    return "-I" + s;
  })
  .join(" ");

function format(str) {
  for (var f = 1; f < arguments.length; f++) {
    str = str.replace(/{\w*}/, arguments[f]);
  }
  return str;
}

function clean_builds() {
  try {
    var stats = fs.statSync(OUTPUT_PATH);
  } catch (e) {
    fs.mkdirSync(OUTPUT_PATH);
  }

  try {
    var files = fs.readdirSync(OUTPUT_PATH);
    var filesLength = files.length;
    if (filesLength > 0) {
      if (NO_LIBAR == true) {
        for (var i = 0; i < filesLength - 2; i++) {
          var filePath = OUTPUT_PATH + "/" + files[i];
          if (fs.statSync(filePath).isFile()) fs.unlinkSync(filePath);
        }
      } else {
        for (var i = 0; i < filesLength; i++) {
          var filePath = OUTPUT_PATH + "/" + files[i];
          if (fs.statSync(filePath).isFile()) fs.unlinkSync(filePath);
        }
      }
    }
  } catch (e) {
    return console.log(e);
  }
}

var compile_arlib = format(
  EMCC +
    INCLUDES +
    " " +
    ar_sources.join(" ") +
    FLAGS +
    " " +
    DEFINES +
    " -r -o {OUTPUT_PATH}libar.bc ",
  OUTPUT_PATH
);

var compile_debug_arlib = format(
  EMCC +
    INCLUDES +
    " " +
    ar_sources.join(" ") +
    DEBUG_FLAGS +
    FLAGS +
    " " +
    DEFINES +
    " -r -o {OUTPUT_PATH}libar_debug.bc ",
  OUTPUT_PATH
);

var compile_simd_arlib = format(
  EMCC +
    INCLUDES +
    " " +
    ar_sources.join(" ") +
    FLAGS +
    SIMD128_FLAGS +
    " " +
    DEFINES +
    " -r -o {OUTPUT_PATH}libar_simd.bc ",
  OUTPUT_PATH
);

/*
 * Run commands
 */

function onExec(error, stdout, stderr) {
  if (stdout) console.log("stdout: " + stdout);
  if (stderr) console.log("stderr: " + stderr);
  if (error !== null) {
    console.log("exec error: " + error.code);
    process.exit(error.code);
  } else {
    runJob();
  }
}

function runJob() {
  if (!jobs.length) {
    console.log("Jobs completed");
    return;
  }
  var cmd = jobs.shift();

  if (typeof cmd === "function") {
    cmd();
    runJob();
    return;
  }

  console.log("\nRunning command: " + cmd + "\n");
  exec(cmd, onExec);
}

var jobs = [];

function addJob(job) {
  jobs.push(job);
}

addJob(clean_builds);
addJob(compile_arlib);
addJob(compile_debug_arlib);
//addJob(compile_simd_arlib);

if (NO_LIBAR == true) {
  jobs.splice(1, 2);
}

runJob();
