#ifndef MATRIX_SMART_H
#define MATRIX_SMART_H

namespace jsfeat {
class Matrix_smart : public Data_t {
 public:
  int cols;
  int rows;
  int type;
  size_t channel;
  size_t size;
  Matrix_smart(int c, int r, int data_type) {
    cols = c;
    rows = r;
    type = get_data_type(data_type) | 0;
    channel = get_channel(data_type) | 0;
    size = (cols * channel) * rows;
    allocate();
  }
  ~Matrix_smart() {
#ifdef DEBUG_EM
    JSLOGd("deleting Matrix_smart");
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

  std::shared_ptr<Matrix_smart> get_smart_pointer() {
    return std::shared_ptr<Matrix_smart>(this);
  }
#ifdef __EMSCRIPTEN__
  emscripten::val get_data() const {
    if (type == Types::U8_t) {
      std::cout << u8.size() << std::endl;
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
      std::cout << "size in allocate: " << u8.size() << std::endl;
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
};

}  // namespace jsfeat

#endif