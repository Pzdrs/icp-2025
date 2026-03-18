#include "scuffcraft.hpp"
#include "detection/face_detection_app.hpp"

#include <iostream>
#include <limits>

bool AskDetection()
{
    while (true)
    {
        std::cout << "Select mode:\n";
        std::cout << "  1) Scuffcraft\n";
        std::cout << "  2) Face Detection\n";
        std::cout << "> ";

        int choice;

        if (!(std::cin >> choice))
        {
            // špatný input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input, try again.\n\n";
            continue;
        }

        if (choice == 1)
            return false;
        if (choice == 2)
            return true;

        std::cout << "Invalid choice, try again.\n\n";
    }
}

int main()
{
    if (AskDetection())
    {
        return detection::RunFaceDetection({});
    }

    Scuffcraft app;
    app.Run();

    return 0;
}