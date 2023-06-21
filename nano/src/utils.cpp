#include <nano/utils.hpp>

#include <nano/error.hpp>

#include <fstream>

namespace nano
{

std::string
path2str(const std::filesystem::path& p)
{
    return std::filesystem::weakly_canonical(p).string();
}

int
extract_file(std::string& dst, const std::filesystem::path& filename)
{
    std::error_code err_code;
    const std::size_t size = std::filesystem::file_size(filename, err_code);
    if (err_code.default_error_condition())
    {
        LOG_DEBUG(
                  "%s: %s",
                  err_code.message().c_str(),
                  path2str(filename).c_str());
        return EXIT_FAILURE;
    }

    std::ifstream src(filename);
    if (not src)
    {
        LOG_DEBUG(
                  "Error opening file: %s",
                  path2str(filename).c_str());
        return EXIT_FAILURE;
    }
    dst.resize(size);
    src.read(dst.data(), size);

    return src.good() ? EXIT_SUCCESS : EXIT_FAILURE;
}

} // namespace nano
