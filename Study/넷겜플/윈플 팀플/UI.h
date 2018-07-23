#pragma once

class UI {
protected:
	CImage Icon;
	POINT Pos;
	int size;
	int num;
public:
	int getxPos() const { return Pos.x; }
	void setxPos(int i) { Pos.x = i; }
	int getyPos() const { return Pos.y; }
	void setyPos(int i) { Pos.y = i; }
	int getNum() const { return num; }
	void setNum(int i) { num = i; }
	void draw(HDC hdc);
};

class UI_BOMB : public UI {

public:
	UI_BOMB(int i);

};

class UI_LIFE : public UI {

public:
	UI_LIFE(int i);

};

class UI_SCORE : public UI {
	HFONT hFont, oldFont;
	char str[50];
	int score;
public:
	UI_SCORE();
	int getScore() const { return score; }
	void setScore(int i) { score = i; }
	void draw(HDC hdc);
};