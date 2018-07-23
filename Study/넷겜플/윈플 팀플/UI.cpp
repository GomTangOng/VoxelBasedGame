#include <Windows.h>
#include <atlImage.h>
#include "SpaceShip.h"
#include "UI.h"

void UI::draw(HDC hdc)
{
	if (num != 0)
	{
		for (int i = 0; i < num; ++i)
			Icon.Draw(hdc, Pos.x + 100 - ( 50 * i ), Pos.y, size, size);
	}
}

UI_BOMB::UI_BOMB(int i)
{
	Icon.Load(TEXT("ITEM(ÇÊ»ì±â).png"));
	size = 30;
	num = i;
	Pos.x = 500;
	Pos.y = 780;
}

UI_LIFE::UI_LIFE(int i)
{
	Icon.Load(TEXT("player1.png"));
	size = 30;
	num = i;
	Pos.x = 500;
	Pos.y = 15;
}

UI_SCORE::UI_SCORE()
{
	score = 0;
	Pos.x = 15;
	Pos.y = 15;
}

void UI_SCORE::draw(HDC hdc)
{

	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("±Ã¼­"));
	oldFont = (HFONT)SelectObject(hdc, hFont);

	sprintf(str, "score : %d", score);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, Pos.x, Pos.y, str, strlen(str));

	SelectObject(hdc, oldFont);
	DeleteObject(hFont);

}