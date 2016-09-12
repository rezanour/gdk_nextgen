#include <Windows.h>
#include <memory>
#include "../GraphicsBackend/GraphicsBackend.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    std::shared_ptr<GraphicsBackend> backend = CreateGraphicsBackend(GraphicsBackendType::Vulkan);
    return 0;
}
