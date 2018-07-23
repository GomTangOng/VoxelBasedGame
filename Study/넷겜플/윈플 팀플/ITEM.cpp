#include <Windows.h>
#include <atlImage.h>
#include "resource.h"
#include "ITEM.h"
#include "SpaceShip.h"

void ITEM::draw(HDC hdc)
{
	ItemImg->Draw(hdc, Pos.x + XDir, Pos.y +yDir, Size, Size);
}

bool ITEM::IsGet(Player& p)
{
	RECT rt1, rt2, rt3;
	rt1.top = Pos.y;
	rt1.bottom = Pos.y + Size;
	rt1.left = Pos.x;
	rt1.right = Pos.x + Size;

	rt2.top = p.getyPos();
	rt2.bottom = p.getyPos() + p.getSize();
	rt2.left = p.getxPos();
	rt2.right = p.getxPos() + p.getSize();

	if (IntersectRect(&rt3, &rt1, &rt2))
		return true;
	else
		return false;
}

void ITEM::SetDir(char c, bool b) 
{
	switch (c) {
	case 'x':
		XDir = b;
		break;
	case 'y':
		yDir = b;
		break;
	}
}

bool ITEM::getDir(char c) const
{
	switch (c)
	{
	case 'x':
		return XDir;
		break;
	case 'y':
		return yDir;
		break;
	}
}

I_BULLET::I_BULLET(CImage* img)
{
	int i = rand() % 2;
	XDir = i;
	ItemImg = img;
	Pos.x = rand() % 500;
	Pos.y = rand() % 11 - 70;
}

I_SUB::I_SUB(CImage* img)
{
	int i = rand() % 2;
	XDir = i;
	ItemImg = img;
	Pos.x = rand() % 500;
	Pos.y = rand() % 11 - 70;

}

I_POWER::I_POWER(CImage* img)
{
	int i = rand() % 2;
	XDir = i;
	ItemImg = img;
	Pos.x = rand() % 500;
	Pos.y = rand() % 11 - 70;
}

I_SKILL::I_SKILL(CImage* img)
{
	int i = rand() % 2;
	XDir = i;
	ItemImg = img; 
	Pos.x = rand() % 500;
	Pos.y = rand() % 11 - 70;
}

I_SHEILD::I_SHEILD(CImage* img)
{
	int i = rand() % 2;
	XDir = i;
	ItemImg = img;
	Pos.x = rand() % 500;
	Pos.y = rand() % 11 - 70;
}