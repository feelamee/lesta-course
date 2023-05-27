#ifndef UTILS_HPP
#define UTILS_HPP

#include <filesystem>
#include <string>

namespace nano
{

std::string path2str(const std::filesystem::path&);

int extract_file(std::string&, const std::filesystem::path&);

} // namespace nano

#endif // UTILS_HPP
