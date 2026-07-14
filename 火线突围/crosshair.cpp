#include "crosshair.h"

Crosshair::Crosshair()
{
	loadimage(&img_crosshair, _T("img/crosshair.png"));
}

void Crosshair::Update()
{
	GetCursorPos(&position);
	ScreenToClient(GetHWnd(), &position);
}

void Crosshair::Draw()
{
	putimage_alpha(position.x - img_crosshair.getwidth() / 2, position.y - img_crosshair.getheight() / 2, &img_crosshair);
}
