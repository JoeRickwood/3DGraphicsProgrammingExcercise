#include "TestRepeat.h"

TestRepeat::TestRepeat(float _tiling)
{
	tiling = _tiling;
}

TestRepeat::~TestRepeat()
{
}

void TestRepeat::ShaderUpdate()
{
	Renderer* renderer = parent->GetComponent<Renderer>();

	Frame frame = renderer->uvFrame;

	frame.topRight.x = tiling;
	frame.topRight.y = tiling;

	renderer->SetUVFrame(frame);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
