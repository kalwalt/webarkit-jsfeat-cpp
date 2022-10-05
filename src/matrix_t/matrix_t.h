#ifndef MATRIX_T_H
#define MATRIX_T_H

#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif
#include <jslog/jslog.h>
#include <node_utils/data_t.h>
#include <node_utils/functions.h>
#include <string>
#include <types/types.h>

namespace jsfeat {
class matrix_t : public data_t {

public:
  int cols;
  int rows;
  int type;
  size_t channel;
  size_t size;
#ifdef __EMSCRIPTEN__
  emscripten::val data = emscripten::val::null();

  matrix_t(int c, int r, int data_type, emscripten::val data_buffer) {
    if (c < 0 || r < 0) {
      JSLOGw("cols and rows values must be greater than zero, will be "
             "converted to absoulte values.");
      cols = std::abs(c), rows = std::abs(r);
    } else {
      cols = c;
      rows = r;
    }
    type = get_data_type(data_type) | 0;
    channel = get_channel(data_type) | 0;
    size = (cols * channel) * rows;
    if (isType(data_buffer, "object")) {
      fillData(data_buffer);
    } else {
      allocate();
    }
  };
  matrix_t(int c, int r, int data_type) {
    if (c < 0 || r < 0) {
      JSLOGw("cols and rows values must be greater than zero, will be "
             "converted to absoulte values.");
      cols = std::abs(c), rows = std::abs(r);
    } else {
      cols = c;
      rows = r;
    }
    type = get_data_type(data_type) | 0;
    channel = get_channel(data_type) | 0;
    size = (cols * channel) * rows;
    allocate();
  };
#else
  matrix_t(int c, int r, int data_type) {
    if (c < 0 || r < 0) {
      std::cout << "error!" << std::endl;
      JSLOGw("cols and rows values must be greater than zero, will be "
             "converted to absoulte values.");
      cols = std::abs(c), rows = std::abs(r);
    } else {
      cols = c;
      rows = r;
    }
    type = get_data_type(data_type) | 0;
    channel = get_channel(data_type) | 0;
    size = (cols * channel) * rows;
  };
  matrix_t(matrix_t &m) {
    cols = m.cols;
    rows = m.rows;
    type = m.type;
    channel = m.channel;
    size = m.size;
    if (type == Types::U8_t) {
      u8 = m.u8;
    } else if (type == Types::S32_t) {
      i32 = m.i32;
    } else if (type == Types::F32_t) {
      f32 = m.f32;
    } else if (type == Types::F64_t) {
      f64 = m.f64;
    }
  }
  matrix_t(const matrix_t &m) {
    cols = m.cols;
    rows = m.rows;
    type = m.type;
    channel = m.channel;
    size = m.size;
    if (type == Types::U8_t) {
      u8 = m.u8;
    } else if (type == Types::S32_t) {
      i32 = m.i32;
    } else if (type == Types::F32_t) {
      f32 = m.f32;
    } else if (type == Types::F64_t) {
      f64 = m.f64;
    }
  }
#endif

  ~matrix_t() {
#ifdef DEBUG_EM
    JSLOGd("deleting matrix_t");
#endif
  }

  int getCols() const { return cols; };

  void setCols(int c) { cols = c; };

  int getRows() const { return rows; };

  void setRows(int r) { rows = r; };

  int getType() const { return type; };

  void setType(int _type) { type = _type; };

  int getChannel() const { return channel; };

  void setChannel(int _channel) { channel = _channel; };
#ifdef __EMSCRIPTEN__
  emscripten::val getData() const {
    if (type == Types::U8_t) {
      emscripten::val view{emscripten::typed_memory_view(u8.size(), u8.data())};
      auto result = emscripten::val::global("Uint8Array").new_(u8.size());
      result.call<void>("set", view);
      return view;
    } else if (type == Types::S32_t) {
      emscripten::val view{
          emscripten::typed_memory_view(i32.size(), i32.data())};
      auto result = emscripten::val::global("Int32Array").new_(i32.size());
      result.call<void>("set", view);
      return view;
    } else if (type == Types::F32_t) {
      emscripten::val view{
          emscripten::typed_memory_view(f32.size(), f32.data())};
      auto result = emscripten::val::global("Float32Array").new_(f32.size());
      result.call<void>("set", view);
      return view;
    } else if (type == Types::F64_t) {
      emscripten::val view{
          emscripten::typed_memory_view(f64.size(), f64.data())};
      auto result = emscripten::val::global("Float64Array").new_(f64.size());
      result.call<void>("set", view);
      return view;
    }
  }
#endif

  void allocate() {
    if (type == Types::U8_t) {
      u8.assign(size, 0);
    } else if (type == Types::S32_t) {
      i32.assign(size, 0);
    } else if (type == Types::F32_t) {
      f32.assign(size, 0);
    } else if (type == Types::F64_t) {
      f64.assign(size, 0);
    }
  }

  void resize(int c, int r, int ch) {
    if (!ch) {
      ch = channel;
    }
    // relocate buffer only if new size doesnt fit
    int new_size = (c * ch) * r;
    if (new_size > size) {
      cols = c;
      rows = r;
      channel = ch;
      if (type == Types::U8_t) {
        u8.resize(new_size);
      } else if (type == Types::S32_t) {
        i32.resize(new_size);
      } else if (type == Types::F32_t) {
        f32.resize(new_size);
      } else if (type == Types::F64_t) {
        f64.resize(new_size);
      }
    } else {
      cols = c;
      rows = r;
      channel = ch;
    }
  }

#ifdef __EMSCRIPTEN__
  auto getPointer() { return reinterpret_cast<int>(this); }

  static _Mat_t get(const matrix_t &m) {
    _Mat_t output;
    output.cols = m.cols;
    output.rows = m.rows;
    output.channels = m.channel;
    output.size = m.size;
    output.data = m.getData();
    return output;
  };
#endif
private:
#ifdef __EMSCRIPTEN__
  void fillData(emscripten::val data_buffer) {
    if (type == Types::U8_t) {
      u8 = emscripten::convertJSArrayToNumberVector<u_char>(data_buffer);
      u8.resize(size);
    } else if (type == Types::S32_t) {
      i32 = emscripten::convertJSArrayToNumberVector<int>(data_buffer);
      i32.resize(size);
    } else if (type == Types::F32_t) {
      f32 = emscripten::convertJSArrayToNumberVector<float>(data_buffer);
      f32.resize(size);
    } else if (type == Types::F64_t) {
      f64 = emscripten::convertJSArrayToNumberVector<double>(data_buffer);
      f64.resize(size);
    }
  }
  bool isType(emscripten::val value, const std::string &type) {
    return (value.typeOf().as<std::string>() == type);
  }
#endif
};
} // namespace jsfeat

#endif