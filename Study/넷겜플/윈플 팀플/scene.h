#pragma once

class SCENE {
	CImage Start;
	CImage Play;
	CImage StartButton, ExitButton;
	//CImage 
	HFONT hFont, oldFont;
	char str[50];
	bool Ingame;
	int Scene; // 0 - ����ȭ��, 1 - �÷���ȭ�� , 2 - �Ͻ�����, 3 - ���ӿ���
public:
	SCENE();
	void SetIngame(bool b);
	bool Isplay();
	void SetScene(int i) { Scene = i; }
	int SceneCheck() const { return Scene; }
	int buttonCheck(int mx, int my, int WndX, int WndY);
	void draw(HDC memdc, int MapY1, int MapY2, int WndX, int WndY, int score);
};