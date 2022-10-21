#ifndef CACHE_H
#define CACHE_H

#include <cache/LRUCache.h>
#include <node_utils/data_t.h>
#include <node_utils/functions.h>
#include <optional>
#include <string>

namespace jsfeat {

template <size_t Capacity>
class Cache : public LRUCache<std::string, data_t, Capacity> {
public:
  Cache(){};
  std::optional<data_t> put_buffer(size_t data_size, int data_type) {
    type = get_data_type(data_type) | 0;
    data_t data(data_size, type);
    std::string buffer_count = buffer + std::to_string(counter);
    this->put(buffer_count, data);
    counter++;
    std::optional<data_t> dt = this->get(buffer_count);
    return dt;
  }
  ~Cache() {
#ifdef DEBUG_EM
    JSLOGd("deleting Cache");
#endif
  }

private:
  int counter{0};
  std::string buffer{"buffer_"};
  int type{0};
};

} // namespace jsfeat

#endif // end of