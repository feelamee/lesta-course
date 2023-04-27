#ifndef COLOR_HPP
#define COLOR_HPP

#include <concepts>
#include <cstdint>

#pragma pack(push, 1)
template <typename compT = uint8_t>
    requires std::is_same_v<uint8_t, compT>
    or std::is_same_v<uint16_t, compT> struct color
{
  union
  {
    struct
    {
      compT r = 0;
      compT g = 0;
      compT b = 0;
    };
    char components[3 * sizeof (compT)];
  };

  friend bool
  operator== (const color &l, const color &r)
  {
    return l.r == r.r && l.g == r.g && l.b == r.b;
  }

  friend std::ostream &
  operator<< (std::ostream &os, color<compT> color_)
  {
    auto tmp = static_cast<color<uint16_t> > (color_);
    return os << "[" << tmp.r << " " << tmp.g << " " << tmp.b << "]";
  }

  operator color<uint16_t> () const
  {
    return { static_cast<uint16_t> (r), static_cast<uint16_t> (g),
             static_cast<uint16_t> (b) };
  }
};
#pragma pack(pop)

constexpr color<uint8_t> color_black = { 0, 0, 0 };

#endif // COLOR_HPP