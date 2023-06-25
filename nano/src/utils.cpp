#include <nano/utils.hpp>

#include <nano/error.hpp>

#include <imgui.h>

#include <SDL3/SDL_rwops.h>

#include <filesystem>
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
            "%s: %s", err_code.message().c_str(), path2str(filename).c_str());
        return EXIT_FAILURE;
    }

    std::ifstream src(filename);
    if (not src)
    {
        LOG_DEBUG("Error opening file: %s", path2str(filename).c_str());
        return EXIT_FAILURE;
    }
    dst.resize(size);
    src.read(dst.data(), size);

    return src.good() ? EXIT_SUCCESS : EXIT_FAILURE;
}

ImFont*
load_font_from_file_ttf(const std::filesystem::path& fn, float pixels_size)
{
    std::size_t fsize{ 0 };
    void* data = SDL_LoadFile(fn.string().c_str(), &fsize);

    return ImGui::GetIO().Fonts->AddFontFromMemoryTTF(data, fsize, pixels_size);
}

void
print(const transform2D& tr)
{
    const float* d = tr.data();
    // clang-format off
    printf("[%f][%f][%f]\n"
           "[%f][%f][%f]\n"
           "[%f][%f][%f]\n",
           d[0], d[1], d[2],
           d[3], d[4], d[5],
           d[6], d[7], d[8]);
    // clang-format on
}

} // namespace nano
