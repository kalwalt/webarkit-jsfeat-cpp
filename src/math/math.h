#ifndef MATH_H
#define MATH_H

#include <algorithm>
#include <cstddef>
#include <types/types.h>
#include <utility>

namespace jsfeat {
class Math {
public:
  Math() { qsort_stack.assign(48 * 2, 0); }
  template <typename A, typename B>
  Array<A> qsort_internal(Array<A> array, size_t low, size_t high, B (*cmp)(A, A)) {
    auto isort_thresh = 7;
    int t, ta, tb, tc;
    auto sp = 0, left = 0, right = 0, i = 0, n = 0, m = 0, ptr = 0, ptr2 = 0,
         d = 0;
    auto left0 = 0, left1 = 0, right0 = 0, right1 = 0, pivot = 0, a = 0, b = 0,
         c = 0, swap_cnt = 0;

    auto stack = qsort_stack;

    if ((high - low + 1) <= 1) {
      return array;
    }

    stack[0] = low;
    stack[1] = high;

    while (sp >= 0) {

      left = stack[sp << 1];
      right = stack[(sp << 1) + 1];
      sp--;

      for (;;) {
        n = (right - left) + 1;

        if (n <= isort_thresh) {
          // insert_sort:
          for (ptr = left + 1; ptr <= right; ptr++) {
            for (ptr2 = ptr; ptr2 > left && cmp(array[ptr2], array[ptr2 - 1]);
                 ptr2--) {
              std::swap(array[ptr2], array[ptr2 - 1]);
            }
          }
          break;
        } else {
          swap_cnt = 0;

          left0 = left;
          right0 = right;
          pivot = left + (n >> 1);

          if (n > 40) {
            d = n >> 3;
            a = left, b = left + d, c = left + (d << 1);
            ta = array[a], tb = array[b], tc = array[c];
            left = cmp(ta, tb) ? (cmp(tb, tc) ? b : (cmp(ta, tc) ? c : a))
                               : (cmp(tc, tb) ? b : (cmp(ta, tc) ? a : c));

            a = pivot - d, b = pivot, c = pivot + d;
            ta = array[a], tb = array[b], tc = array[c];
            pivot = cmp(ta, tb) ? (cmp(tb, tc) ? b : (cmp(ta, tc) ? c : a))
                                : (cmp(tc, tb) ? b : (cmp(ta, tc) ? a : c));

            a = right - (d << 1), b = right - d, c = right;
            ta = array[a], tb = array[b], tc = array[c];
            right = cmp(ta, tb) ? (cmp(tb, tc) ? b : (cmp(ta, tc) ? c : a))
                                : (cmp(tc, tb) ? b : (cmp(ta, tc) ? a : c));
          }

          a = left, b = pivot, c = right;
          ta = array[a], tb = array[b], tc = array[c];
          pivot = cmp(ta, tb) ? (cmp(tb, tc) ? b : (cmp(ta, tc) ? c : a))
                              : (cmp(tc, tb) ? b : (cmp(ta, tc) ? a : c));
          if (pivot != left0) {
            std::swap(array[pivot], array[left0]);
            pivot = left0;
          }
          left = left1 = left0 + 1;
          right = right1 = right0;

          ta = array[pivot];
          for (;;) {
            while (left <= right && !cmp(ta, array[left])) {
              if (!cmp(array[left], ta)) {
                if (left > left1) {
                  std::swap(array[left1], array[left]);
                }
                swap_cnt = 1;
                left1++;
              }
              left++;
            }

            while (left <= right && !cmp(array[right], ta)) {
              if (!cmp(ta, array[right])) {
                if (right < right1) {
                  std::swap(array[right1], array[right]);
                }
                swap_cnt = 1;
                right1--;
              }
              right--;
            }

            if (left > right)
              break;

            std::swap(array[left], array[right]);
            swap_cnt = 1;
            left++;
            right--;
          }

          if (swap_cnt == 0) {
            left = left0, right = right0;
            // goto insert_sort;
            for (ptr = left + 1; ptr <= right; ptr++) {
              for (ptr2 = ptr; ptr2 > left && cmp(array[ptr2], array[ptr2 - 1]);
                   ptr2--) {
                t = array[ptr2];
                array[ptr2] = array[ptr2 - 1];
                array[ptr2 - 1] = t;
              }
            }
            break;
          }

          n = std::min((left1 - left0), (left - left1));
          m = (left - n) | 0;
          for (i = 0; i < n; ++i, ++m) {
            std::swap(array[left0 + i], array[m]);
          }

          n = std::min((right0 - right1), (right1 - right));
          m = (right0 - n + 1) | 0;
          for (i = 0; i < n; ++i, ++m) {
            std::swap(array[left + i], array[m]);
          }
          n = (left - left1);
          m = (right1 - right);
          if (n > 1) {
            if (m > 1) {
              if (n > m) {
                ++sp;
                stack[sp << 1] = left0;
                stack[(sp << 1) + 1] = left0 + n - 1;
                left = right0 - m + 1, right = right0;
              } else {
                ++sp;
                stack[sp << 1] = right0 - m + 1;
                stack[(sp << 1) + 1] = right0;
                left = left0, right = left0 + n - 1;
              }
            } else {
              left = left0, right = left0 + n - 1;
            }
          } else if (m > 1)
            left = right0 - m + 1, right = right0;
          else
            break;
        }
      }
    }
    return array;
  }
  template <typename A>
  Array<A> sort_internal(Array<A> array, size_t low, size_t high) {
    std::sort(array.begin()+low, array.begin()+high);
  return array;
  }

private:
  Array<int> qsort_stack;
};
} // namespace jsfeat
#endif