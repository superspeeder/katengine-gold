#include "kat/window.hpp"

namespace kat {


    Window::Window() {

    }

    void Window::addListener(const WindowEventType type, std::function<void(WindowEventData*)> fListener) {
        // construct a new vector if the key doesn't exist yet
        m_WindowEventListeners.try_emplace(type);
        m_WindowEventListeners[type].push_back(fListener);
    }
}