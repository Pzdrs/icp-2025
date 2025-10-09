#include <iostream>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>
#include <lightbulb.hpp>

class App
{
public:
    App();
    bool init(void);
    int run(void);

    ~App();
};

App::App() {}

bool App::init()
{
    try
    {
        // případná inicializace...
    }
    catch (std::exception const &e)
    {
        std::cerr << "Init failed : " << e.what() << std::endl;
        throw;
    }

    return true;
}

int App::run(void)
{
    try
    {
        doLightbulb();
        // počkej na ESC
        while (true)
        {
            int key = cv::pollKey();
            if (key == 27)
                break;
        }
    }
    catch (std::exception const &e)
    {
        std::cerr << "App failed : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

App::~App()
{
    cv::destroyAllWindows();
    std::cout << "Bye...\n";
}

App app;

int main()
{
    if (app.init())
        return app.run();
}