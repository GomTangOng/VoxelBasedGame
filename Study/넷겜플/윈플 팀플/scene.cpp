#include <Windows.h>
#include <atlImage.h>
#include "resource.h"
#include "scene.h"

SCENE::SCENE() {
	Start.Load(TEXT("StartBackground.png"));
	Play.Load(TEXT("background.png"));
	StartButton.Load(TEXT("STARTBUTTON.png"));
	ExitButton.Load(TEXT("EXITBUTTON.png"));
	Ingame = false;
}
void SCENE::SetIngame(bool b) {
	Ingame = b;
}
bool SCENE::Isplay() {
	return Ingame;
}
int SCENE::buttonCheck(int mx, int my, int WndX, int WndY) {
	switch (Scene)
	{
	case 0:
		if (mx < WndX - 400 && mx > WndX - 600 && my < WndY - 150 && my > WndY - 250)
			return 1;
		else if (mx < WndX - 50 && mx > WndX - 250 && my < WndY - 150 && my > WndY - 250)
			return 2;
		break;
	case 2:
		if (mx < 430 && mx > 200 && my < 400 && my > 350)
			return 1;
		else if (mx < 400 && mx > 230 && my < 450 && my > 400)
			return 2;
		else if (mx < 410 && mx > 220 && my < 500 && my > 450)
			return 3;
		else
			return 0;
		break;
	case 3:
	case 4:
		if (mx < 280 && mx > 100 && my < 500 && my > 450)
			return 1;
		else if (mx < 290 && mx > 100 && my < 550 && my > 500)
			return 2;
		break;
	}
}
void SCENE::draw(HDC memdc,int MapY1, int MapY2, int WndX, int WndY, int score) {

	switch (Scene)
	{
	case 0:
		Start.Draw(memdc, 0, 0, WndX, WndY);
		StartButton.Draw(memdc, WndX - 600, WndY - 250, 200, 100);
		ExitButton.Draw(memdc, WndX - 250, WndY - 250, 200, 100);
		break;
	case 1:
		Play.Draw(memdc, 0, MapY1, WndX, WndY);
		Play.Draw(memdc, 0, MapY2 - WndY, WndX, WndY);
		break;
	case 2:
	
		hFont = CreateFont(40, 0, 0, 0, 0, true, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("»ﬁ∏’∆Ì¡ˆ√º"));
		oldFont = (HFONT)SelectObject(memdc, hFont);

		sprintf(str, "%s", "Return to Game");
		SetTextColor(memdc, RGB(255, 255, 255));
		SetBkMode(memdc, OPAQUE);
		SetBkColor(memdc, RGB(100, 100, 100));
		TextOut(memdc, 200, 350, str, strlen(str));

		sprintf(str, "%s", "Go to Main");
		TextOut(memdc, 230, 400, str, strlen(str));

		sprintf(str, "%s", "Exit Program");
		TextOut(memdc, 220, 450, str, strlen(str));

		SelectObject(memdc, oldFont);
		DeleteObject(hFont);
		break;
	case 3:
		hFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("»ﬁ∏’∆Ì¡ˆ√º"));
		oldFont = (HFONT)SelectObject(memdc, hFont);

		sprintf(str, "%s", "CLEAR!");
		SetTextColor(memdc, RGB(255, 255, 255));
		SetBkMode(memdc, TRANSPARENT);
		TextOut(memdc, 100, 150, str, strlen(str));

		hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("»ﬁ∏’∆Ì¡ˆ√º"));
		oldFont = (HFONT)SelectObject(memdc, hFont);

		sprintf(str, "score : %d", score);
		SetTextColor(memdc, RGB(255, 255, 255));
		SetBkMode(memdc, TRANSPARENT);
		TextOut(memdc, 100, 250, str, strlen(str));

		hFont = CreateFont(40, 0, 0, 0, 0, true, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("»ﬁ∏’∆Ì¡ˆ√º"));
		oldFont = (HFONT)SelectObject(memdc, hFont);
		SetBkMode(memdc, OPAQUE);
		SetBkColor(memdc, RGB(100, 100, 100));

		sprintf(str, "%s", "Go to Main");
		TextOut(memdc, 100, 450, str, strlen(str));
		
		sprintf(str, "%s", "Exit Program");
		TextOut(memdc, 100, 500, str, strlen(str));

		SelectObject(memdc, oldFont);
		DeleteObject(hFont);
		break;

	case 4:
		hFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("»ﬁ∏’∆Ì¡ˆ√º"));
		oldFont = (HFONT)SelectObject(memdc, hFont);

		sprintf(str, "%s", "GAME OVER");
		SetTextColor(memdc, RGB(255, 255, 255));
		SetBkMode(memdc, TRANSPARENT);
		TextOut(memdc, 100, 150, str, strlen(str));

		hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("»ﬁ∏’∆Ì¡ˆ√º"));
		oldFont = (HFONT)SelectObject(memdc, hFont);

		sprintf(str, "score : %d", score);
		SetTextColor(memdc, RGB(255, 255, 255));
		SetBkMode(memdc, TRANSPARENT);
		TextOut(memdc, 100, 250, str, strlen(str));

		hFont = CreateFont(40, 0, 0, 0, 0, true, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("»ﬁ∏’∆Ì¡ˆ√º"));
		oldFont = (HFONT)SelectObject(memdc, hFont);
		SetBkMode(memdc, OPAQUE);
		SetBkColor(memdc, RGB(100, 100, 100));

		sprintf(str, "%s", "Go to Main");
		TextOut(memdc, 100, 450, str, strlen(str));

		sprintf(str, "%s", "Exit Program");
		TextOut(memdc, 100, 500, str, strlen(str));

		SelectObject(memdc, oldFont);
		DeleteObject(hFont);
		break;
	}
	
}