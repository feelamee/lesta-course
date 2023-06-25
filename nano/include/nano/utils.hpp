#ifndef UTILS_HPP
#define UTILS_HPP

#include "nano/transform2D.hpp"
#include <nano/error.hpp>

#include <imgui.h>

#include <filesystem>
#include <string>

namespace nano
{

std::string path2str(const std::filesystem::path&);

int extract_file(std::string&, const std::filesystem::path&);

ImFont* load_font_from_file_ttf(const std::filesystem::path&,
                                float pixels_size);

#ifdef __cpp_lib_ranges
void
print(const std::ranges::range auto& r)
{
    printf("(range: %UL )[ ", std::size(r));
    for (const auto& i : r)
        printf("%d ", i);
    printf("]\n");
}
#endif

void print(const transform2D&);

} // namespace nano

#endif // UTILS_HPP
