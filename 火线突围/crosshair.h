#pragma once
#include "common.h"

class Crosshair
{
public:
	Crosshair();
	void Update();
	void Draw();

private:
	IMAGE img_crosshair;
	POINT position;
};
