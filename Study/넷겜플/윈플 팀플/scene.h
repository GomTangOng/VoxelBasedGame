#pragma once

class SCENE {
	CImage Start;
	CImage Play;
	CImage StartButton, ExitButton;
	//CImage 
	HFONT hFont, oldFont;
	char str[50];
	bool Ingame;
	int Scene; // 0 - 시작화면, 1 - 플레이화면 , 2 - 일시정지, 3 - 게임오버
public:
	SCENE();
	void SetIngame(bool b);
	bool Isplay();
	void SetScene(int i) { Scene = i; }
	int SceneCheck() const { return Scene; }
	int buttonCheck(int mx, int my, int WndX, int WndY);
	void draw(HDC memdc, int MapY1, int MapY2, int WndX, int WndY, int score);
};