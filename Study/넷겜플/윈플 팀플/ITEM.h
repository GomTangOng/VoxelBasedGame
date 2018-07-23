#pragma once
#include "SpaceShip.h"

class ITEM {
protected:
	POINT Pos;
	int Size = 50;
	bool XDir, yDir = true;
	CImage* ItemImg;
public:
	void setPos(int x, int y) { Pos.x = x, Pos.y = y; }
	POINT getPos() { return Pos; }
	int getxPos() const { return Pos.x; }
	void setxPos(int x) { Pos.x = x; }
	int getyPos() const { return Pos.y; }
	void setyPos(int y) { Pos.y = y; }
	int getSize() const { return Size; }
	void SetDir(char c, bool b);
	bool getDir(char c) const;
	void draw(HDC hdc);
	bool IsGet(Player& p);
};	

class I_BULLET : public ITEM {
	
public:
	I_BULLET(CImage* img);
};

class I_SUB : public  ITEM {

public:
	I_SUB(CImage* img);
};

class I_POWER : public ITEM {

public:
	I_POWER(CImage* img);
};

class I_SKILL : public  ITEM {

public:
	I_SKILL(CImage* img);
	
};

class I_SHEILD : public  ITEM {

public:
	I_SHEILD(CImage* img);
};