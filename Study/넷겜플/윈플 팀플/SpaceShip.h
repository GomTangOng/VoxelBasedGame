#pragma once

class SpaceShip
{
protected:
	POINT Pos;
	int hp;
	int size;
public:
	void Draw(HDC memdc) {};
	void setPos(int x, int y) { Pos.x = x, Pos.y = y; }
	POINT getPos() { return Pos; }
	int getxPos() const { return Pos.x; }
	void setxPos(int x) { Pos.x = x; }
	void setxPos(float x) { Pos.x = x; }
	int getyPos() const { return Pos.y; }
	void setyPos(int y) { Pos.y = y; }
	void setyPos(float y) { Pos.y = y; }
	int getHp() { return hp; }
	void setHp(int h) { hp = h; }
	int getSize() { return size; }
	void setSize(int i) { size = i; }
};

class Player : public SpaceShip
{
private:
	CImage pImg;
	CImage pImgL;
	CImage pImgR;
	CImage Skillimg;
	CImage Subimg;
	CImage Barrierimg;
	POINT SkillPos;

	bool Sub;
	bool PowerUp;
	bool Barrier;

	int SkillSize;
	int SubSize;
	int PlayerLR = 0;
	int bullet; // ÃÑ¾Ë °¹¼ö
	int bomb;	// ÆøÅº °¹¼ö
	int PowerUpCount;
	int BarrierCount;
public:
	Player();
	void setSkillyPos(int p) { SkillPos.y = p; }
	int getSkillyPos() const { return SkillPos.y; }
	void setSub(bool b) { Sub = b; }
	bool haveSub() const { return Sub; }
	void setPowerUp(bool b) { PowerUp = b; }
	bool getPowerUp() const { return PowerUp; }
	void setBullet(int b) { bullet = b; }
	int getBullet() const { return bullet; }
	void setBomb(int b) { bomb = b; }
	int getBomb() const { return bomb; }
	void setBarrier(bool b) { Barrier = b; }
	bool getBarrier() const { return Barrier; }
	int getPowerUpCount() const { return PowerUpCount; }
	void SetPowerUpCount(int i) { PowerUpCount = i; }
	int getBarrierCount() const { return BarrierCount; }
	void SetBarrierCount(int i) { BarrierCount = i; }
	void usingBomb(HDC hdc);
	void usingBarrier(HDC hdc);
	void setLR(int i)
	{
		if (i == 0)
		{
			PlayerLR = 0;
		}
		if (i == 1) // 1 - R , 2 - L
		{
			PlayerLR = 1;
		}
		if (i == 2)
		{
			PlayerLR = 2;
		}
	}
	void Draw(HDC memdc);
};

class Enemy : public SpaceShip
{
private:
	int type;
	int BoomCount;
	bool Boom;
	CImage* EnemyImg;
public:
	Enemy() {};
	Enemy(CImage* img, int i)
	{
		EnemyImg = img;
		switch (i)
		{
		case 1:
			Pos.x = rand() % 500;
			Pos.y = -70;
			BoomCount = 0;
			Boom = false;
			type = 1;
			hp = 5;
			size = 60;
			
			break;
		case 2:
			Pos.x = rand() % 500;
			Pos.y = -70;
			BoomCount = 0;
			Boom = false;
			type = 2;
			hp = 25;
			size = 150;
	
			break;
		case 3:
			Pos.x = 220;
			Pos.y = -190;
			BoomCount = 0;
			Boom = false;
			type = 3;
			hp = 60;
			size = 200;
			
			break;
		case 4:
			Pos.x = 170;
			Pos.y = -500;
			BoomCount = 0;
			Boom = false;
			type = 4;
			hp = 2000;
			size = 300;
			
			break;
		}
	}
	void Draw(HDC memdc);
	int getType() const;
	void BoomDraw(HDC hdc, CImage* img);
	void SetBoomCount(int i) { BoomCount = i; }
	int getBoomCount() const { return BoomCount; }
	void BOOM() { Boom = true; }
	bool IsBoom() const { return Boom; }
};

class Bullet : public SpaceShip
{
	CImage* BulletImg;
	int type; // -2 - °­È­ÃÑ¾Ë, -1 - º¸Á¶ ÃÑ¾Ë, 0 - ÇÃ·¹ÀÌ¾î ÃÑ¾Ë, 1 - Àû1 ÃÑ¾Ë , 2 - Àû2 ÃÑ¾Ë , 3 - Àû3 ÃÑ¾Ë, 4 - º¸½º ÃÑ¾Ë
public:
	Bullet() {}
	Bullet(POINT p, CImage* img, int i) {
		Pos.x = p.x;
		Pos.y = p.y;
		BulletImg = img;
		type = i;
		switch (i)
		{
		case 0:
		case -1:
		case -2:
			Pos.x += 14;
			size = 20;
			break;
		case 1: 
			Pos.x += 15;
			size = 15;
			break;
		case 2:
			Pos.x += 63;
			Pos.y += 25;
			size = 20;
			break;
		case 3:
			Pos.x += 91;
			Pos.y += 150;
			size = 20;
			break;
		case 4:
			Pos.x += 285;
			Pos.y += 280;
			size = 25;
			break;
		case 5:
			Pos.x += 0;
			Pos.y += 15;
			size = 25;
			break;
		case 6:
			Pos.x += 270;
			Pos.y += 280;
			size = 50;
			break;
		case 7:
			Pos.x += 145;
			Pos.y += 130;
			size = 20;
			break;
		}
	}
	void Draw(HDC memdc);
	int getType() const { return type; }
	bool IsCrashtoEnemy(Enemy e);
	bool IsCrashtoPlayer(Player& P);
};

class Boss : public SpaceShip
{
private:
	int img_count = 1;
	int type;
	CImage boss_base[8];
public:
	Boss() {}
	Boss(int i);
	void Draw(HDC memdc);
	void setImg(int i);
	int getImg() const;
};