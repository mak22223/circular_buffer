// SPDX-License-Identifier: MIT
#pragma once

#include <stdint.h>

template <typename T, size_t _size>
class CircularBuffer
{
public:
  CircularBuffer() : d_full(false), d_head(0), d_tail(0) {}

  bool put(const T *buf, size_t len)
  {
    size_t available = _available();

    if (available < len || !buf) {
      return false;
    }

    if (len == available) {
      d_full = true;
    }

    if (d_head + len >= _size) {
      size_t cycle = _size - d_head;
      for (size_t i = 0; i < cycle; ++i) {
        d_data[d_head + i] = buf[i];
      }
      for (size_t i = 0; i < len - cycle; ++i) {
        d_data[i] = buf[i + cycle];
      }
      d_head = len - cycle;
    } else {
      for (size_t i = 0; i < len; ++i) {
        d_data[d_head + i] = buf[i];
      }
      d_head += len;
    }

    return true;
  }

  bool get(size_t num, T *buf)
  {
    if (_used() < num || !buf) {
      return false;
    }

    if (num > 0) {
      d_full = false;
    }

    if (d_tail + num >= _size) {
      size_t cycle = _size - d_tail;

      for (size_t i = 0; i < cycle; ++i) {
        buf[i] = d_data[d_tail + i];
      }
      for (size_t i = 0; i < num - cycle; ++i) {
        buf[i + cycle] = d_data[i];
      }
      d_tail = num - cycle;
    } else {
      for (size_t i = 0; i < num; ++i) {
        buf[i] = d_data[d_tail + i];
      }
      d_tail += num;
    }

    return true;
  }

  bool peek(size_t num, T *buf) const
  {
    if (_used() < num || !buf) {
      return false;
    }

    if (d_tail + num >= _size) {
      size_t cycle = _size - d_tail;

      for (size_t i = 0; i < cycle; ++i) {
        buf[i] = d_data[d_tail + i];
      }
      for (size_t i = 0; i < num - cycle; ++i) {
        buf[i + cycle] = d_data[i];
      }
    } else {
      for (size_t i = 0; i < num; ++i) {
        buf[i] = d_data[d_tail + i];
      }
    }

    return true;
  }

  size_t find(const T& elem) const
  {
    if (empty()) {
      return -1;
    }

    size_t used = _used();
    size_t i = 0;
    if (d_tail + used > _size) {
      size_t cycle = _size - d_tail;
      
      while (i < cycle && d_data[d_tail + i] != elem) {
        ++i;
      }

      if (d_data[d_tail + i] != elem) {
        size_t j = 0;
        while (j < used - cycle && d_data[j] != elem)
        {
          ++j;
        }
        i += j;
      }
    } else {
      while (i < used && d_data[d_tail + i] != elem) {
        ++i;
      }
    }

    if (i == used) {
      i = -1;
    }

    return i;
  }

  bool eraseFirst(size_t num)
  {
    if (_used() < num) {
      return false;
    }

    if (num > 0) {
      d_full = false;
    }

    d_tail += num;

    if (d_tail >= _size) {
      d_tail -= _size;
    }

    return true;
  }

  size_t size() const
  {
    return _size;
  }

  size_t available() const
  {
    return _available();
  }

  size_t used() const
  {
    return _used();
  }

  bool empty() const
  {
    return used() == 0;
  }

  bool full() const
  {
    return d_full;
  }

  void reset()
  {
    d_tail = d_head = 0;
    d_full = false;
  }

protected:
  inline size_t _available() const
  {
    if (d_full) {
      return 0;
    }

    if (d_tail > d_head) {
      return d_tail - d_head;
    } else {
      return _size - d_head + d_tail;
    }
  }

  inline size_t _used() const
  {
    return _size - _available();
  }

protected:
  T d_data[_size];
  bool d_full = false;
  size_t d_head;
  size_t d_tail;

};
