#include <cache/LRUCache.h>
#include <node_utils/data_t.h>
#include <string>
namespace jsfeat {
template <size_t Capacity>
class Cache : public LRUCache<std::string, data_t, Capacity> {
public:
  Cache(){};
  void put_buffer(size_t data_size, int data_type) {
    type = get_data_type(data_type) | 0;
    data_t data(data_size, type);
    std::string buffer_count = buffer + std::to_string(counter);
    this->put(buffer_count, data);
    counter++;
  }
  ~Cache() {
#ifdef DEBUG_EM
    JSLOGd("deleting Cache");
#endif
  }
private:
  int counter{0};
  std::string buffer{"buffer_"};
  int type {0};
};
} // namespace jsfeat