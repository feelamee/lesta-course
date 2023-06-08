#ifndef UTILS_HPP
#define UTILS_HPP

#include <filesystem>
#include <string>

namespace nano
{

std::string path2str(const std::filesystem::path&);

int extract_file(std::string&, const std::filesystem::path&);

void print(const std::ranges::range auto& r);

} // namespace nano

#endif // UTILS_HPP
