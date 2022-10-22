#ifndef CACHE_H
#define CACHE_H

#include <cache/LRUCache.h>
#include <jslog/jslog.h>
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
  bool back_buffer() {
    std::string buffer_count = buffer + std::to_string(--counter);
    bool res = this->back(buffer_count);
#ifdef DEBUG_EM
    if (res == true) {
      JSLOGd("%s is back!", buffer_count.c_str());
    } else {
      JSLOGd("%s is not back...", buffer_count.c_str());
    }
#endif
    return res;
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
/* We create an instance of the Cache class inside the namespace, so let include
the header to make available when it is needed */
Cache<30> cache;
} // namespace jsfeat

#endif // end of