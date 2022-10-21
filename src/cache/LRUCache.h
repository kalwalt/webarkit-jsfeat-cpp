#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <list>
#include <optional>
#include <unordered_map>

template <typename K, typename V, std::size_t Capacity> class LRUCache {

public:
  // Assert that Max size is > 0
  static_assert(Capacity > 0, "");
  /*Adds a key=>value item
  Returns false if key already exists*/
  bool put(const K &k, const V &v) {
    // Return false if the key already exists
    if (index.count(k)) {
      return false;
    }
    // Check if cache is full
    if (items.size() == Capacity) {
      // Delete the LRU item
      index.erase(items.back().first); // Erase the last item key from the map
      items.pop_back();                // Evict last item from the list
    }

    // Insert the new item at front of the list
    items.emplace_front(k, v);

    // Insert {key->item_iterator} in the map
    index.emplace(k, items.begin());

    return true;
  }

  /*Gets the value for a key.
   Returns empty std::optional if not found.
   The returned item becomes most-recently-used*/
  std::optional<V> get(const K &k) {
    auto itr = index.find(k);
    if (itr == index.end()) {
      return {}; // empty std::optional
    }

    /*Use list splice to transfer this item to
     the first position, which makes the item
     most-recently-used. Iterators still stay valid.*/
    items.splice(items.begin(), items, itr->second);

    // Return the value in a std::optional
    return itr->second->second;
  }

  // Erases an item
  void erase(const K &k) {
    auto itr = index.find(k);
    if (itr == index.end()) {
      return;
    }

    // Erase from the list
    items.erase(itr->second);

    // Erase from the  map
    index.erase(itr);
  }

private:
  /*std::list stores items (pair<K,V>) in
  most-recently-used to least-recently-used order.*/
  std::list<std::pair<K, V>> items;

  // unordered_map acts as an index to the items store above.
  std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> index;
};

#endif