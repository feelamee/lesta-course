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
    err_os << "(range: " << std::size(r) << ')' << "[ ";
    for (const auto& i : r)
        err_os << i << " ";
    err_os << ']' << std::endl;
}

} // namespace nano

#endif // UTILS_HPP
