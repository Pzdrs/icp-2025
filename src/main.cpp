#include "scuffcraft.hpp"
#include "detection/face_detection_app.hpp"
#include "detection/image_processor.hpp"

int main()
{
    #if defined(RUN_RED_STATIC)
        RedRecognizer recognizer;
        return recognizer.run_static("assets/misc/red_cup.jpg");
    #elif defined(RUN_FACE_DETECTION)
        return run_face_detection({
            .cascadePath = "assets/misc/haarcascade_frontalface_default.xml",
            .cameraIndex = 1,
            .showFps = true,
        });
    #else
        Scuffcraft app;
        app.Run();
        return 0;
    #endif
}
