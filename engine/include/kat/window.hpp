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
        const unsigned short kCountWin = 17;
        const unsigned short kLastWin = kFirstWin + kCountWin;
        const unsigned short kFirstSys = kLastWin;
        const unsigned short kCountSys = 3;
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
        eDropPath = window_evt_mng::winEvt(7),
        ePos = window_evt_mng::winEvt(8),
        eSize = window_evt_mng::winEvt(9),
        eClose = window_evt_mng::winEvt(10),
        eRefresh = window_evt_mng::winEvt(11),
        eFocus = window_evt_mng::winEvt(12),
        eIconify = window_evt_mng::winEvt(13),
        eMaximize = window_evt_mng::winEvt(14),
        eFramebufferSize = window_evt_mng::winEvt(15),
        eContentScale = window_evt_mng::winEvt(16),
    };

    enum class SystemEventType : unsigned short {
        eJoystick = window_evt_mng::sysEvt(0),
        eError = window_evt_mng::sysEvt(1),
        eMonitor = window_evt_mng::sysEvt(2),
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

    struct GLFWError {
        int code;
        std::string msg;
    };


    class Monitor {
    public:

        static std::vector<Monitor*> getMonitors();
        static Monitor* getPrimary();

        Monitor();
        
        glm::ivec2 getPos();
        glm::ivec4 getWorkarea();
        glm::ivec2 getPhysicalSize();
        glm::vec2 getContentScale();
        std::string getName();
        void* getUserData();
        void setUserData(void* data);

        std::vector<const GLFWvidmode*> getVideoModes();
        const GLFWvidmode* getVideoMode();
        void setGamma(float gamma);
        void setGammaRamp(std::vector<std::array<unsigned short, 3> > ramp);



    private:
        GLFWmonitor* m_Monitor;

        static std::vector<Monitor*> s_Monitors;
    };

    struct MonitorUserData {

    };

    enum class WindowHint : int {

    };
// GLFW_DECORATED, GLFW_RESIZABLE, GLFW_FLOATING, GLFW_AUTO_ICONIFY and GLFW_FOCUS_ON_SHOW
    enum class WindowAttribute : int {
        eFocused = GLFW_FOCUSED,
        eIconfied = GLFW_ICONIFIED,
        eDecorated = GLFW_DECORATED,
        eResizable = GLFW_RESIZABLE,
        eFloating = GLFW_FLOATING,
        eAutoIconify = GLFW_AUTO_ICONIFY,
        eFocusOnShow = GLFW_FOCUS_ON_SHOW,
        
    };

    struct WindowIcons {
        std::vector<GLFWimage*> images;
    };


    /**
     * @brief 
     * 
     * 
     * gl::Window adds the following functions:
     * - makeContextCurrent()
     * - gl::Window* gl::Window::getCurrentContext()
     * - swapInterval(int i)
     * - bool isContextCurrent()
     * - GLFWglproc gl::Window::loadGLFunction(const char* pName)
     * - bool gl::Window isExtensionSupported(const char* ext)
     * 
     */
    class Window {
    public:
        static std::string getGLFWVersionString();
        static void init();
        static void terminate();
        
        // only works if init hasn't been run yet
        static void hideJoystickHatButtons(); // prevents joystick hats from being exposed as buttons
        static void disableCocoaMenubar(); // only effects things on macos
        static void preventCocoaChdirResources(); // only effects things on macos

        static GLFWError getGLFWError();
        static Version getGLFWVersion();

        static void defaultHints();
        static void hint(WindowHint hint, int value);
        static void hintCocoaFrameName(const std::string &name);
        static void hintX11ClassName(const std::string &name);
        static void hintX11InstanceName(const std::string &name);

        static void hintResizable(bool v);
        static void hintVisible(bool v);
        static void hintDecorated(bool v);
        static void hintFocused(bool v);
        static void hintAutoIconify(bool v);
        static void hintFloating(bool v);
        static void hintMaximized(bool v);
        static void hintCenterCursor(bool v);
        static void hintTransparentFramebuffer(bool v);
        static void hintFocusOnShow(bool v);
        static void hintScaleToMonitor(bool v);
        static void hintRedBits(int v);
        static void hintGreenBits(int v);
        static void hintBlueBits(int v);
        static void hintAlphaBits(int v);
        static void hintDepthBits(int v);
        static void hintStencilBits(int v);
        static void hintAccumRedBits(int v);
        static void hintAccumGreenBits(int v);
        static void hintAccumBlueBits(int v);
        static void hintAccumAlphaBits(int v);
        static void hintAuxBuffers(int v);
        static void hintSamples(int v);
        static void hintRefreshRate(int v);
        static void hintSrgbCapable(bool v);
        static void hintDoublebuffer(bool v);
        static void useEmbeddedOpenGL(bool v);
        static void hintContextVersion(int major, int minor);
        static void hintOpenGLForwardCompat(bool v);
        static void hintOpenGLDebugContext(bool v);
        static void hintOpenGLCoreProfile();
        static void hintOpenGLAnyProfile();
        static void hintOpenGLCompatProfile();
        static void hintCocoaRetinaFramebuffer(bool v);
        static void hintCocoaGraphicsSwitching(bool v);

        static void pollEvents();
        static void waitEvents();
        static void waitEventsTimeout(double to);
        static void postEmptyEvent();


        Window(const WindowSettings& settings, const GraphicsAPI& api);

        bool isOpen();
        void close();
        void keepOpen();
        void setOpen(bool);
        void setTitle(const std::string&);
        void setIcon(const WindowIcons&);

        glm::ivec2 getPos();
        void setPos(const glm::ivec2&);
        glm::ivec2 getSize();
        void setSizeLimits(const glm::ivec2 &min, const glm::ivec2 &max);
        void setAspectRatio(int numer, int denom);
        void setSize(const glm::ivec2&);
        glm::ivec2 getFramebufferSize();
        glm::ivec2 getFrameSize();
        glm::vec2 getContentScale();
        float getOpacity();
        void iconify();
        void restore();
        void maximize();
        void show();
        void hide();
        void focus();
        void requestAttention();
        Monitor* getMonitor();
        void setMonitor(Monitor* mon, const glm::ivec2& pos, const glm::ivec2& res, int refreshRate);
        int getAttrib(const WindowAttribute&);
        void setAttrib(const WindowAttribute&, const int& v);
        void setUserData(void* pData);
        void* getUserData();


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