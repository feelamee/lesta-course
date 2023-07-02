#ifndef IMGUI_BACKEND_HPP
#define IMGUI_BACKEND_HPP

#include <nano/event.hpp>

namespace imgui
{
bool process_event(const nano::event* event);
}

#endif // IMGUI_BACKEND_HPP
