#ifndef _BASIC_GL_MANAGER_H_
#define _BASIC_GL_MANAGER_H_

#include "Window.hpp"
#include "Element.hpp"
#include "LightSource.hpp"
#include "Plot.hpp"
#include <vector>
#include <string>
#include <time.h>

namespace BasicGL
{
    enum Modes { MODE_2D = 0, MODE_3D };

    class Manager
    {
        private:
            static bool initialized;
        public:
            static void Init(int argc, char *argv[]);
            static void Destroy();

            static int Create(const std::string& name, Modes mode = MODE_2D, int fps = 30, int width = 0, int height = 0, int x = -1, int y = -1);

            static void SelectWindow(int index);
            static void SetBackground(float r, float g, float b, float a = 1.0f);
            static void SetBackground(int r, int g, int b, int a = 255);
            static void SetAnimationFunction(AnimationFunction animationFunction);
            static void SetKeyboardFunction(KeyboardFunction keyboardFunction);
            static void SetMouseFunction(MouseFunction mouseFunction);
            static void SetCartesian(bool cartesian);
            static bool IsOpen();
            static float WindowWidth();
            static float WindowHeight();

            static WindowPtr CurrentWindow();
            static ElementPtr find(const std::string& name);
            static ElementPtr get(int index);
            static int index(ElementPtr element);

            static Element* CreateElement(Elements element, const std::string name = "");
            static Plot* CreatePlot(int rows = 1, int cols = 1, int index = 1, const std::string name = "");
            static LightSourcePtr CreateLightSource();
            static LightSourcePtr GetLightSource(int index);

            static void Show();
            static void Pause(float seconds = 0);
            static bool IsFullscreen();
            static void SetFullscreen(bool enabled);
            static void ToggleFullscreen();

            static bool Capture(const std::string& fileName);
            static bool StartRecording(const std::string& fileName, int fps = 60);
            static void StopRecording();
        
        private:
            static std::vector< Window > windows;
            static int currentWindow;
            static int WindowIndex();

            static void Closed();
            static void Render();
            static void KeyboardNormal(unsigned char key, int x, int y);
            static void KeyboardSpecial(int key, int x, int y);
            static void MouseButtons(int button, int state, int x, int y);
            static void MouseMotion(int x, int y);
            static void MouseEntry(int state);
            static void Resize(int w, int h);
            static void Timer(int index);
            static void now(struct timespec &t);
    };
}

#endif
