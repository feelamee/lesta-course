#ifndef UTILS_HPP
#define UTILS_HPP

#include <nano/error.hpp>

#include <filesystem>
#include <string>

namespace nano
{

std::string path2str(const std::filesystem::path&);

int extract_file(std::string&, const std::filesystem::path&);

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

} // namespace nano

#endif // UTILS_HPP
