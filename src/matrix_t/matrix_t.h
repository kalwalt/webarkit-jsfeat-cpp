#ifndef MATRIX_T_H
#define MATRIX_T_H

#include <cstddef>
#include <cstdio>
#include <iostream>
#include <node_utils/data_t.h>
#include <types/types.h>

namespace jsfeat {
class matrix_t {

public:
  int cols;
  int rows;
  int type;
  int channel;
  int size;
  u_char *data;
  Array<u_char> u8;
  data_t *dt;

  matrix_t(int c, int r, int data_type, int data_buffer) {
    cols = c;
    rows = r;
    type = get_data_type(data_type) | 0;
    printf("type is : %i\n", type);
    channel = get_channel(data_type) | 0;
    printf("channel is : %i\n", channel);
    _data_type_size.assign(
        {-1, 1, 4, -1, 4, -1, -1, -1, 8, -1, -1, -1, -1, -1, -1, -1, 8});
    size = (cols * get_data_type_size(data_type) * channel) *
           rows; //(cols * rows);
    printf("size is: %i\n", size);
    data = new u_char[size * channel];
    dt = new data_t((cols * get_data_type_size(type) * channel) * rows);
  };

  int getCols() const { return cols; };

  void setCols(int c) { cols = c; };

  int getRows() const { return rows; };

  void setRows(int r) { rows = r; };

  int getType() const { return type; };

  void setType(int _type) { type = _type; };

  int getChannel() const { return channel; };

  void setChannel(int _channel) { channel = _channel; };

  void allocate() {
    if (type == Types::U8_t) {
      std::cout << "size parameter: " << size << std::endl;
      for (int i = 0; i < size; i++) {
        dt->u8.push_back(0);
      }
      std::cout << "size is: " << dt->u8.size() << std::endl;
      std::cout << "allocated" << std::endl;
      std::cout << (int)dt->u8.at(0) << std::endl;
    } else if (type == Types::S32_t) {
      std::cout << "size parameter: " << size << std::endl;
      for (int i = 0; i < size; i++) {
        dt->i32.push_back(0);
      }
      std::cout << "size is: " << dt->i32.size() << std::endl;
      std::cout << "allocated" << std::endl;
      std::cout << (int)dt->i32.at(0) << std::endl;
    } else if (type == Types::F32_t) {
      std::cout << "size parameter: " << size << std::endl;
      for (int i = 0; i < size; i++) {
        dt->f32.push_back(0.0);
      }
      std::cout << "size is: " << dt->f32.size() << std::endl;
      std::cout << "allocated" << std::endl;
      std::cout << (float)dt->f32.at(0) << std::endl;
    } else if (type == Types::F64_t) {
      std::cout << "size parameter: " << size << std::endl;
      for (int i = 0; i < size; i++) {
        dt->f64.push_back(0.0);
      }
      std::cout << "size is: " << dt->f64.size() << std::endl;
      std::cout << "allocated" << std::endl;
      std::cout << (float)dt->f64.at(0) << std::endl;
    }
  }

  template <typename T> void fill(T *data) {
    if (type == Types::U8_t) {
      assign_data();
      for (int i = 0; i < size - 1; i++) {
        this->data[i] = data[i];
      };
    }
  }

  void resize(int c, int r, int ch) {
    if (!ch) {
      ch = channel;
    }
    std::cout << "channel: " << ch << std::endl;
    // relocate buffer only if new size doesnt fit
    std::cout << "Type inside resize: " << type << std::endl;
    std::cout << "Get data type size: " << get_data_type_size(type)
              << std::endl;
    std::cout << "size: " << size << std::endl;
    int new_size = ((c * get_data_type_size(type) * ch) * r);
    std::cout << "New size is: " << new_size << std::endl;
    if (new_size > size) {
      cols = c;
      rows = r;
      channel = ch;
      allocate();
    } else {
      cols = c;
      rows = r;
      channel = ch;
    }
  }

  auto getPointer() { return (int)this; }

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
  Array<int> _data_type_size;
  int get_data_type(int type) { return (type & 0xFF00); }
  int get_channel(int type) { return (type & 0xFF); };
  int get_data_type_size(int type) {
    return _data_type_size[(type & 0xFF00) >> 8];
  }
  void assign_data() { data = dt->u8.data(); }
};
} // namespace jsfeat

#endif