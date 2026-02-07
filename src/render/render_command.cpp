#include "pch.hpp"
#include "render/render_command.hpp"

Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();