#ifndef UTILS_HPP
#define UTILS_HPP

#include <nano/error.hpp>

#include <filesystem>
#include <string>

namespace nano
{

std::string path2str(const std::filesystem::path&);

int extract_file(std::string&, const std::filesystem::path&);

void
print(const std::ranges::range auto& r)
{
    fprintf(err_os, "(range: %UL )[ ", std::size(r));
    for (const auto& i : r)
        fprintf(err_os, "%d ", i);
    fprintf(err_os, "]\n");
}

} // namespace nano

#endif // UTILS_HPP
