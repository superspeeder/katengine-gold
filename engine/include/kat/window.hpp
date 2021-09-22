#pragma once

#include "kat/core.hpp"
#include <unordered_map>
#include <functional>
#include <vector>
#include <concepts>

namespace kat {

    namespace window_evt_mng {
        //// EVENT ORGANIZATION CONSTANTS

        const unsigned short kFirstWin = 0;
        const unsigned short kCountWin = 8;
        const unsigned short kLastWin = kFirstWin + kCountWin;
        const unsigned short kFirstSys = kLastWin;
        const unsigned short kCountSys = 1;
        const unsigned short kLastSys = kFirstSys + kCountSys;

        const kat::evtmng::evt_inc<kFirstWin, kCountWin> winEvt;
        const kat::evtmng::evt_inc<kFirstSys, kCountSys> sysEvt;
    }


    enum class WindowEventType : unsigned short {
        eMouseButton = window_evt_mng::winEvt(0),
        eCursorPos = window_evt_mng::winEvt(1), 
        eCursorEnter = window_evt_mng::winEvt(2), 
        eScroll = window_evt_mng::winEvt(3), 
        eKey = window_evt_mng::winEvt(4),
        eChar = window_evt_mng::winEvt(5),
        eCharMods = window_evt_mng::winEvt(6),
        eDropPath = window_evt_mng::winEvt(7)
    };

    enum class SystemEventType : unsigned short {
        eJoystick = window_evt_mng::sysEvt(0)
    };


    struct WindowSettings {

    };

    struct WindowEventData {
        
    };

    template<typename T>
    concept WinEvtDcls = std::derived_from<WindowEventData, T> && requires { T::type; } && std::same_as<decltype(T::type), const WindowEventType>; 

    template<std::derived_from<WindowEventData> T>
    inline std::function<void(WindowEventData*)> adaptWindowEventListener(std::function<void(T*)> fListener) {
        return [fListener](WindowEventData* pData) {
            fListener(dynamic_cast<T*>(pData)); // call the listener properly
        };
    };

    class Window {
    public:
        Window();



        template<WinEvtDcls T>
        inline void addListener(std::function<void(T*)> fListener) {
            addListener(T::type, adaptWindowEventListener(fListener));
        };

        void addListener(const WindowEventType type, std::function<void(WindowEventData*)> fListener);

    private:
        GraphicsAPI m_CompatibleGraphicsAPI;
        GLFWwindow* m_Window;
        glm::ivec2 m_FramebufferSize;

        std::unordered_map<WindowEventType, std::vector<std::function<void(WindowEventData*)> > > m_WindowEventListeners;
    };

    struct WindowUserData {
        void* pData;
        Window* pWindow;
    };

}