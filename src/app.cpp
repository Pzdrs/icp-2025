#include <iostream>
#include <opencv2/opencv.hpp>

#include "app.hpp"

App::App()
{
    // default constructor
    // nothing to do here (so far...)
    std::cout << "Constructed...\n";
}

bool App::init()
{
    try {
        // initialization code
        //...

        // some init
        // if (not_success)
        //  throw std::runtime_error("something went bad");
    }
    catch (std::exception const& e) {
        std::cerr << "Init failed : " << e.what() << std::endl;
        throw;
    }
    std::cout << "Initialized...\n";
    return true;
}

int App::run(void)
{
    try {
        // app code
        //...
    }
    catch (std::exception const& e) {
        std::cerr << "App failed : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "Finished OK...\n";
    return EXIT_SUCCESS;
}

App::~App()
{
    cv::destroyAllWindows();
    std::cout << "Bye...\n";
}