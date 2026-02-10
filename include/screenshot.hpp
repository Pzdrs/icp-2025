#pragma once
#include "render/renderer.hpp"
#include "window.hpp"
#include <stb_image_write.h>
#include <nfd.hpp>

struct Screenshot
{
    unsigned int width;
    unsigned int height;
    std::vector<unsigned char> data;
};

class ScreenshotManager
{
public:
    static void Capture(const Window &window)
    {
        NFD::Init();

        Screenshot sc = Renderer::CaptureScreenshot(
            window.GetFramebufferWidth(),
            window.GetFramebufferHeight());

        try
        {
            nfdnchar_t *outPath = nullptr;

            // Filter: display name + extension spec
            nfdu8filteritem_t filters[1] = {{"PNG Image", "png"}};

            auto result = NFD_SaveDialogU8(
                &outPath,
                filters,
                1,
                nullptr,
                "screenshot.png");

            if (result == NFD_OKAY)
            {
                stbi_flip_vertically_on_write(true);
                if (!stbi_write_png(outPath, sc.width, sc.height, 4,
                                    sc.data.data(), sc.width * 4))
                {
                    std::cerr << "Failed to write PNG file: " << outPath << std::endl;
                }
                NFD::FreePath(outPath); // Must free the path when done
                return;
            }

            if (result == NFD_ERROR)
            {
                std::cerr << "Native File Dialog error: " << NFD::GetError() << std::endl;
            }
        }
        catch (...)
        {
            std::cerr << "Unexpected error during save dialog.\n";
        }
    }
};