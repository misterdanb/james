#include "Renderer.hpp"

Renderer::Renderer(RenderContext &rc)
	: _rc(rc), _rcClassic(rc.gameboyClassicSpecific), _rcColor(rc.gameboyColorSpecific)
{
}

Renderer::~Renderer()
{
}
