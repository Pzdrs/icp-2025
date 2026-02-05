#include "render/render_command.hpp"

std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();