#pragma once

#include <string>

namespace detection
{

    struct FaceDetectionOptions
    {
        std::string cascadePath;
        std::string lockscreenPath;
        std::string warningPath;
        int cameraIndex = 0;
        bool showFps = true;
    };

    int RunFaceDetection(const FaceDetectionOptions &options);

}
