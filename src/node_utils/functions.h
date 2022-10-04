#ifndef UTILS_H
#define UTILS_H

int get_channel(int type) { return (type & 0xFF); };

int get_data_type(int type) { return (type & 0xFF00); }

#endif