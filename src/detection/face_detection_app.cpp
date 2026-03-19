#include "detection/face_detection_app.hpp"

#include <iostream>

#include "detection/face_processor.hpp"

int run_face_detection(const FaceDetectionOptions &options)
{
    if (options.cascadePath.empty())
    {
        std::cerr << "Face detection requires options.cascadePath." << std::endl;
        return EXIT_FAILURE;
    }

    FaceRecognizer recognizer(options.cascadePath, options.cameraIndex, options.showFps);
    if (!recognizer.init())
    {
        std::cerr << "Face detection failed: could not load face cascade." << std::endl;
        return EXIT_FAILURE;
    }

    return recognizer.run();
}
