#pragma once

#include <string>

struct FaceDetectionOptions
{
    std::string cascadePath;
    int cameraIndex = 1;
    bool showFps = true;
};

int run_face_detection(const FaceDetectionOptions &options);
