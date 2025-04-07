#include "Renderer.h"

class TestRepeat : public Component 
{
private:
	float tiling;

public:
	TestRepeat(float _tiling);
	~TestRepeat();

	void ShaderUpdate() override;
};