#pragma once

#include <cassert>
#include <string>

#define LOGD(x, ...) printf(x "\n", __VA_ARGS__)
#define LOGDD(x) printf(x "\n")

using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s32 = int32_t;

using utf8_string = std::string;
using utf8_char = std::string::value_type;

template<typename T>
struct bit_mask
{
  using utype = typename std::underlying_type<T>::type;
  utype value;

  inline void clear() { value = 0; }

  inline bool isSet(T flag) const { return value & static_cast<utype>(flag); }
  inline void set(T flag) { value |= static_cast<utype>(flag); }
  inline void reset(T flag) { value &= ~static_cast<utype>(flag); }
  inline void set(T flag, bool value) { if (value) set(flag); else reset(flag); }

  inline bit_mask<T> operator~() const
  {
    return bit_mask<T>(~value);
  }

  inline bit_mask<T> operator&(T flag) const
  {
    return bit_mask<T>(value & static_cast<utype>(flag));

  }

  inline bit_mask<T> operator|(T flag) const
  {
    return bit_mask<T>(value | static_cast<utype>(flag));
  }

  inline bit_mask<T> operator&(const bit_mask<T>& other) const
  {
    return bit_mask<T>(value & other.value);
  }

  bit_mask<T>() : value(0) { }

private:
  bit_mask<T>(utype value) : value(value) { }
};

template<typename T> struct shift_operand_size { constexpr static auto value = static_cast<T>(1); };

template <typename T>
class HeapBitSet
{
private:
  static constexpr size_t BITS = sizeof(T) * 8;
  size_t SIZE;
  size_t stride;

public:
  T* data;

  HeapBitSet() : data(nullptr), SIZE(0) { }
  ~HeapBitSet() { delete[] data; }

  HeapBitSet(const HeapBitSet&) = delete;
  HeapBitSet& operator=(const HeapBitSet&) = delete;

  void init(size_t size, size_t stride = 0)
  {    
    SIZE = size / BITS;

    if (size % BITS)
      ++SIZE;

    data = new T[SIZE]();
    this->stride = stride;
  }

  inline void resize(size_t size)
  {
    auto oldSize = SIZE;
    auto old = data;

    SIZE = size / BITS;

    data = new T[SIZE]();

    if (old)
      memcpy(data, old, oldSize);
  }

  inline void clear() { std::fill(data, data + SIZE, 0); }
  inline void fill() { std::fill(data, data + SIZE, ~0); }
  inline void flip() { std::for_each(data, data + SIZE, [](T& t) { t = ~t; }); }

  inline void set(size_t index) { data[index / BITS] |= (shift_operand_size<T>::value << (index%BITS)); }
  inline void unset(size_t index) { data[index / BITS] &= ~(shift_operand_size<T>::value << (index%BITS)); }
  inline bool isSet(size_t index) const { return (data[index / BITS] & (shift_operand_size<T>::value << (index%BITS))) != 0; }

  inline void set(size_t x, size_t y) { set(x + y * stride); }
  inline void unset(size_t x, size_t y) { unset(x + y * stride); }
  inline bool isSet(size_t x, size_t y) const { return isSet(x + y * stride); }

  inline void release() { delete[] data; data = nullptr; }

  inline size_t size() const { return SIZE; }
};

using coord_t = int32_t;

struct point_t
{
  coord_t x, y;

  bool operator==(const point_t& o) const
  {
    return x == o.x && y == o.y;
  }

  struct hash
  {
    size_t operator()(const point_t& p) const { return p.y << 16 | p.x; }
  };
};

struct pointf_t
{
  float x, y;
};

struct size2d_t
{
  coord_t w, h;
};

struct rect_t
{
  point_t origin;
  size2d_t size;
};

struct color_t
{
  u8 r, g, b, a = 255;
};

using path = std::string;

