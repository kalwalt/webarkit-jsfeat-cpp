#ifndef MATRIX_T_H
#define MATRIX_T_H

#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif
#include <jslog/jslog.h>
#include <node_utils/data_t.h>
#include <node_utils/functions.h>
#include <types/types.h>

#include <memory>
#include <string>
#include <variant>

namespace jsfeat {

class Matrix_t : public Data_t {
 public:
  int cols;
  int rows;
  int type;
  size_t channel;
  size_t size;
  Matrix_t() {
    cols = 0;
    rows = 0;
    channel = 1;
    type = Types::U8_t;
    size = (cols * channel) * rows;
  };
#ifdef __EMSCRIPTEN__
  emscripten::val data = emscripten::val::null();

  Matrix_t(int c, int r, int data_type, emscripten::val data_buffer) {
    if (c < 0 || r < 0) {
      JSLOGw(
          "cols and rows values must be greater than zero, will be "
          "converted to absoulte values.");
      cols = std::abs(c), rows = std::abs(r);
    } else {
      cols = c;
      rows = r;
    }
    type = get_data_type(data_type) | 0;
    channel = get_channel(data_type) | 0;
    size = (cols * channel) * rows;
    if (is_type(data_buffer, "object")) {
      fill_data(data_buffer);
    } else {
      allocate();
    }
  };
  Matrix_t(int c, int r, int data_type) {
    if (c < 0 || r < 0) {
      JSLOGw(
          "cols and rows values must be greater than zero, will be "
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
  Matrix_t(int c, int r, int data_type) {
    if (c < 0 || r < 0) {
      JSLOGw(
          "cols and rows values must be greater than zero, will be "
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
  Matrix_t(int c, int r, int data_type, Data data) {
    if (c < 0 || r < 0) {
      JSLOGw(
          "cols and rows values must be greater than zero, will be "
          "converted to absoulte values.");
      cols = std::abs(c), rows = std::abs(r);
    } else {
      cols = c;
      rows = r;
    }
    type = get_data_type(data_type) | 0;
    channel = get_channel(data_type) | 0;
    size = (cols * channel) * rows;
    if (type == Types::U8_t) {
      auto m_u8 = std::get<Array<u_char> >(data);
      u8 = m_u8;
    } else if (type == Types::S32_t) {
      auto m_i32 = std::get<Array<int> >(data);
      i32 = m_i32;
    } else if (type == Types::F32_t) {
      auto m_f32 = std::get<Array<float> >(data);
      f32 = m_f32;
    } else if (type == Types::F64_t) {
      auto m_f64 = std::get<Array<double> >(data);
      f64 = m_f64;
    }
  };
  Matrix_t(Matrix_t& m) {
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
  Matrix_t(const Matrix_t& m) {
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
  // Move constructor
  Matrix_t(Matrix_t&& m) {
    // You must do this to pass to move assignment
    *this = std::move(m);
  }
  // Copy assignment operator
  Matrix_t& operator=(const Matrix_t& m) {
    // Avoid self assignment
    if (&m == this) {
      return *this;
    }
    // Get copying
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
    return *this;
  }
  // Move assignment operator
  Matrix_t& operator=(Matrix_t&& m) {
    // Avoid self assignment
    if (&m == this) {
      return *this;
    }
    // Get moving
    if (type == Types::U8_t) {
      // u8 = m.u8;
      std::swap(u8, m.u8);
    } else if (type == Types::S32_t) {
      // i32 = m.i32;
      std::swap(i32, m.i32);
    } else if (type == Types::F32_t) {
      // f32 = m.f32;
      std::swap(f32, m.f32);
    } else if (type == Types::F64_t) {
      f64 = m.f64;
      std::swap(f64, m.f64);
    }
    return *this;
  }
#endif

  ~Matrix_t() {
#ifdef DEBUG_EM
    JSLOGd("deleting matrix_t");
#endif
  }

  int get_cols() const { return cols; };

  void set_cols(int c) { cols = c; };

  int get_rows() const { return rows; };

  void set_rows(int r) { rows = r; };

  int get_type() const { return type; };

  void set_type(int _type) { type = _type; };

  int get_channel_m() const { return channel; };

  void set_channel_m(int _channel) { channel = _channel; };
#ifdef __EMSCRIPTEN__
  emscripten::val get_data() const {
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
    size = (cols * channel) * rows;
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
  auto get_pointer() { return reinterpret_cast<int>(this); }

  static _Mat_t get(const Matrix_t& m) {
    _Mat_t output;
    output.cols = m.cols;
    output.rows = m.rows;
    output.channels = m.channel;
    output.size = m.size;
    output.data = m.get_data();
    return output;
  };
#endif
 private:
#ifdef __EMSCRIPTEN__
  void fill_data(emscripten::val data_buffer) {
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
  bool is_type(emscripten::val value, const std::string& type) {
    return (value.typeOf().as<std::string>() == type);
  }
#endif
};
}  // namespace jsfeat

#endif