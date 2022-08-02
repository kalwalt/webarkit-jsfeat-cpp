#ifndef MATRIX_T_H
#define MATRIX_T_H

#include <types/types.h>
#include <cstddef>
#include <cstdio>

class matrix_t {

public:
  int cols;
  int rows;
  int type;
  int channel;
  u_char *data;

  matrix_t(int c, int r, int data_type, int data_buffer) {
    cols = c;
    rows = r;
    size = (cols * rows);
    type = get_data_type(data_type) | 0;
    printf("type is : %i\n", type);
    channel = get_channel(data_type) | 0;
    _data_type_size = {-1, 1,  4,  -1, 4,  -1, -1, -1, 8,
                       -1, -1, -1, -1, -1, -1, -1, 8};
  };

  void allocate() {
    if (type == Types::U8_t) {
      //printf("type is : %i\n", type);
      Array<u_char> array(size - 1);
      for (int i = 0; i < size - 1; i++) {
        array.at(i) = 0;
        //std::cout << (int)array[i] << std::endl;
      }
      data = array.data();
      //std::cout << (int)data[0] << std::endl;
    }
  }

  void resize(int c, int r, int ch) {
    if (!ch) {
      ch = channel;
    }
    // relocate buffer only if new size doesnt fit
    int new_size = (c * get_data_type_size(type) * ch) * r;
    if (new_size > size-1) {
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
};
#endif