#ifndef MATRIX_T_H
#define MATRIX_T_H

#include <cstddef>
#include <cstdio>
#include <iostream>
#include <types/types.h>

namespace jsfeat {
class matrix_t {

public:
  int cols;
  int rows;
  int type;
  int channel;
  u_char *data;
  Array<u_char> u8;

  matrix_t(int c, int r, int data_type, int data_buffer) {
    cols = c;
    rows = r;
    type = get_data_type(data_type) | 0;
    printf("type is : %i\n", type);
    channel = get_channel(data_type) | 0;
    printf("channel is : %i\n", channel);
    _data_type_size.assign({-1, 1,  4,  -1, 4,  -1, -1, -1, 8,
                       -1, -1, -1, -1, -1, -1, -1, 8});
    size = (cols * get_data_type_size(data_type) * channel) *
           rows; //(cols * rows);
    printf("size is: %i\n", size);
    data = new u_char[size*channel];
  };

  int getCols() const { return cols; };

  void setCols(int c) {cols = c;};

  int getRows() const { return rows; };

  void setRows(int r) {rows = r;};

  int getType() const { return type; };

  void setType(int _type) { type = _type;};

  int getChannel() const { return channel; };

  void setChannel(int _channel) { channel = _channel; };

  void allocate() {
    if (type == Types::U8_t) {
      // printf("type is : %i\n", type);
      // u8.assign(size - 1, 0);
      std::cout << "size parameter: " << size << std::endl;
      for (int i = 0; i < size; i++) {
        u8.push_back(0);
      }
      std::cout << "size is: " << u8.size() << std::endl;
      std::cout << "allocated" << std::endl;
      std::cout << (int)u8.at(0) << std::endl;
      // data = _array.data();
      // std::cout << (int)data[0] << std::endl;
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
    std::cout << "Get data type size: " << get_data_type_size(type) << std::endl;
    std::cout << "size: " << size << std::endl;
    int new_size = ((c * get_data_type_size(type) * ch) * r) - 1;
    std::cout << "New size is: " << new_size << std::endl;
    new_size = 4;
    if (u8.empty()) {
      std::cout << "empty array!" << std::endl;
    }
    if (new_size > size - 1) {
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

private:
  int size;
  Array<int> _data_type_size;
  int get_data_type(int type) { return (type & 0xFF00); }
  int get_channel(int type) { return (type & 0xFF); };
  int get_data_type_size(int type) {
    return _data_type_size[(type & 0xFF00) >> 8];
  }
  void assign_data() { data = u8.data(); }
};
} // namespace jsfeat

#endif