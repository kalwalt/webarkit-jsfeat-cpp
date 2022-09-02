#ifndef MATRIX_T_H
#define MATRIX_T_H

#include <cstddef>
#include <emscripten/val.h>
#include <iostream>
#include <node_utils/data_t.h>
#include <string>
#include <types/types.h>

namespace jsfeat {
class matrix_t {

public:
  int cols;
  int rows;
  int type;
  int channel;
  int size;
  data_t *dt;
  emscripten::val data = emscripten::val::null();

  matrix_t(int c, int r, int data_type, emscripten::val data_buffer) {
    cols = c;
    rows = r;
    type = get_data_type(data_type) | 0;
    channel = get_channel(data_type) | 0;
    size = (cols * channel) * rows;
    dt = new data_t(size);
    if (isType(data_buffer, "object")) {
      fillData(data_buffer);
    } else {
      allocate();
    }
  };

  int getCols() const { return cols; };

  void setCols(int c) { cols = c; };

  int getRows() const { return rows; };

  void setRows(int r) { rows = r; };

  int getType() const { return type; };

  void setType(int _type) { type = _type; };

  int getChannel() const { return channel; };

  void setChannel(int _channel) { channel = _channel; };

  emscripten::val getData() const {
    if (type == Types::U8_t) {
      emscripten::val view {emscripten::typed_memory_view(dt->u8.size(), dt->u8.data())};
      auto result = emscripten::val::global("Uint8Array").new_(dt->u8.size());
      result.call<void>("set", view);
      return view;
    } else if (type == Types::S32_t) {
      emscripten::val view {emscripten::typed_memory_view(dt->i32.size(), dt->i32.data())};
      auto result = emscripten::val::global("Int32Array").new_(dt->i32.size());
      result.call<void>("set", view);
      return view;
    } else if (type == Types::F32_t) {
      emscripten::val view {emscripten::typed_memory_view(dt->f32.size(), dt->f32.data())};
      auto result = emscripten::val::global("Float32Array").new_(dt->f32.size());
      result.call<void>("set", view);
      return view;
    } else if (type == Types::F64_t) {
      emscripten::val view {emscripten::typed_memory_view(dt->f64.size(), dt->f64.data())};
      auto result = emscripten::val::global("Float64Array").new_(dt->f64.size());
      result.call<void>("set", view);
      return view;
    }
  }

  void allocate() {
    if (type == Types::U8_t) {
      for (int i = 0; i < size; i++) {
        dt->u8.push_back(0);
      }
    } else if (type == Types::S32_t) {
      for (int i = 0; i < size; i++) {
        dt->i32.push_back(0);
      }
    } else if (type == Types::F32_t) {
      for (int i = 0; i < size; i++) {
        dt->f32.push_back(0.0);
      }
    } else if (type == Types::F64_t) {
      for (int i = 0; i < size; i++) {
        dt->f64.push_back(0.0);
      }
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
        dt->u8.resize(new_size);
      } else if (type == Types::S32_t) {
        dt->i32.resize(new_size);
      } else if (type == Types::F32_t) {
        dt->f32.resize(new_size);
      } else if (type == Types::F64_t) {
        dt->f64.resize(new_size);
      }
    } else {
      cols = c;
      rows = r;
      channel = ch;
    }
  }

  auto getPointer() { return reinterpret_cast<int>(this); }

#ifdef __EMSCRIPTEN__
  static _Mat_t get(const matrix_t &m) {
    _Mat_t output;
    output.cols = m.cols;
    output.rows = m.rows;
    output.channels = m.channel;
    output.size = m.size;
    return output;
  };
#endif
private:
  int get_data_type(int type) { return (type & 0xFF00); }
  int get_channel(int type) { return (type & 0xFF); };
  void fillData(emscripten::val data_buffer) {
    if (type == Types::U8_t) {
      dt->u8 = emscripten::convertJSArrayToNumberVector<u_char>(data_buffer);
      dt->u8.resize(size);
    } else if (type == Types::S32_t) {
      dt->i32 = emscripten::convertJSArrayToNumberVector<int>(data_buffer);
      dt->i32.resize(size);
    } else if (type == Types::F32_t) {
      dt->f32 = emscripten::convertJSArrayToNumberVector<float>(data_buffer);
      dt->f32.resize(size);
    } else if (type == Types::F64_t) {
      dt->f64 = emscripten::convertJSArrayToNumberVector<double>(data_buffer);
      dt->f64.resize(size);
    }
  }
  bool isType(emscripten::val value, const std::string &type) {
    return (value.typeOf().as<std::string>() == type);
  }
};
} // namespace jsfeat

#endif