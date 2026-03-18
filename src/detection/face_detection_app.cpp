#include "detection/face_detection_app.hpp"

#include <array>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/filesystem.hpp>

#include "detection/face_processor.hpp"

namespace detection
{
    namespace
    {
        std::string ResolveCascadePath(const FaceDetectionOptions &options)
        {
            if (!options.cascadePath.empty())
            {
                return options.cascadePath;
            }

            const std::array<std::string, 2> bundledPaths = {
                "assets/haarcascade_frontalface_default.xml",
                "assets/misc/haarcascade_frontalface_default.xml"};
            for (const auto &candidate : bundledPaths)
            {
                if (cv::utils::fs::exists(candidate))
                {
                    return candidate;
                }
            }

            try
            {
                return cv::samples::findFile("haarcascades/haarcascade_frontalface_default.xml", false);
            }
            catch (const cv::Exception &)
            {
                return {};
            }
        }
    }

    int RunFaceDetection(const FaceDetectionOptions &options)
    {
        FaceDetectionOptions resolved = options;
        resolved.cascadePath = ResolveCascadePath(options);

        if (resolved.cascadePath.empty())
        {
            std::cerr << "Face detection requires a Haar cascade file.\n"
                      << "Add haarcascade_frontalface_default.xml to assets/ or specify a path via the menu." << std::endl;
            return EXIT_FAILURE;
        }

        try
        {
            FaceProcessor processor(resolved.cascadePath, resolved.cameraIndex);
            FPSMeter fps;
            FPSMeter *fpsPtr = resolved.showFps ? &fps : nullptr;
            return processor.RunFromCameraPlusFPS(fpsPtr);
        }
        catch (const std::exception &ex)
        {
            std::cerr << "Face detection failed: " << ex.what() << std::endl;
            return EXIT_FAILURE;
        }
    }

}
