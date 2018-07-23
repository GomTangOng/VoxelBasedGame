#include <Windows.h>
#include <atlImage.h>
#include"resource.h"
#include "SpaceShip.h"

Player::Player()
{
	pImg.Load(TEXT("player1.png"));
	pImgR.Load(TEXT("player2.png"));
	pImgL.Load(TEXT("player3.png"));
	Skillimg.Load(TEXT("필살기.png"));
	Subimg.Load(TEXT("ITEM(보조).png"));
	Barrierimg.Load(TEXT("ITEM(방어막).png"));
	Sub = false;
	PowerUp = false;
	Barrier = false;
	SubSize = 25;
	SkillPos.x = 180;
	SkillPos.y = 850;
	SkillSize = 700;
	Pos.x = 300;
	Pos.y = 825; // 775
	hp = 5;
	size = 50;
	bullet = 1;
	bomb = 3;
	PowerUpCount = 0;
	BarrierCount = 0;
}

void Player::Draw(HDC memdc)
{
	if (PlayerLR == 0)
		pImg.Draw(memdc, Pos.x, Pos.y, size, size);
	else if (PlayerLR == 1)
		pImgR.Draw(memdc, Pos.x, Pos.y, size, size);
	else if (PlayerLR == 2)
		pImgL.Draw(memdc, Pos.x, Pos.y, size, size);

	if (Sub)
	{
		Subimg.Draw(memdc, Pos.x - SubSize, Pos.y + SubSize, SubSize, SubSize);
		Subimg.Draw(memdc, Pos.x + size, Pos.y + SubSize, SubSize, SubSize);
	}
}

void Player::usingBomb(HDC hdc)
{
	Skillimg.Draw(hdc, -40, SkillPos.y, SkillSize, SkillSize);
}

void Player::usingBarrier(HDC hdc)
{
	Barrierimg.Draw(hdc, Pos.x - 10, Pos.y - 10, size + 20, size + 20);
}

void Enemy::Draw(HDC memdc)
{
	switch (type)
	{
	case 1:
		EnemyImg->Draw(memdc, Pos.x, Pos.y, size, size + 10);
		break;
	case 2:
		EnemyImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	case 3:
		EnemyImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	case 4:
		EnemyImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	}
}

int Enemy::getType() const
{
	return type;
}

void Enemy::BoomDraw(HDC hdc, CImage* img)
{
	img->Draw(hdc, Pos.x, Pos.y, size, size);
}

void Bullet::Draw(HDC memdc)
{
	switch (type)
	{
	case 0:
	case -1:
	case -2:
		BulletImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	case 1:
		BulletImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	case 2:
		BulletImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	case 3:
		BulletImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	case 4:
		BulletImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	case 5:
		BulletImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	case 6:
		BulletImg->Draw(memdc, Pos.x, Pos.y, size, size);
	case 7:
		BulletImg->Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	}
}

void Boss::Draw(HDC memdc)
{
	switch (type)
	{
	case 1:
		boss_base[img_count].Draw(memdc, Pos.x, Pos.y, size, size);
		break;
	case 2:
		break;
	}
}

void Boss::setImg(int i)
{
	img_count = i;
}

int Boss::getImg() const
{
	return img_count;
}

bool Bullet::IsCrashtoEnemy(Enemy e) 
{
	RECT rt1, rt2, rt3;
	rt1.top = Pos.y, rt1.bottom = Pos.y + size, rt1.left = Pos.x, rt1.right = Pos.x + size;
	switch (e.getType())
	{
	case 1:
		rt2.top = e.getyPos(), rt2.bottom = e.getyPos() + e.getSize(), rt2.left = e.getxPos(), rt2.right = e.getxPos() + e.getSize();
		break;
	case 2:
		rt2.top = e.getyPos(), rt2.bottom = e.getyPos() + e.getSize() - 50, rt2.left = e.getxPos(), rt2.right = e.getxPos() + e.getSize();
		break;
	case 3:
		rt2.top = e.getyPos(), rt2.bottom = e.getyPos() + e.getSize() - 50, rt2.left = e.getxPos() + 50, rt2.right = e.getxPos() + e.getSize() - 50;
		break;
	case 4:
		rt2.top = e.getyPos(), rt2.bottom = e.getyPos() + e.getSize() - 200, rt2.left = e.getxPos() + 50, rt2.right = e.getxPos() + e.getSize() - 50;
		break;
	}
	
	if (IntersectRect(&rt3, &rt1, &rt2))
		return true;
	else
		return false;
}

bool Bullet::IsCrashtoPlayer(Player& P)
{
	RECT rt1, rt2, rt3;
	rt1.top = Pos.y, rt1.bottom = Pos.y + size, rt1.left = Pos.x , rt1.right = Pos.x + size ;
	rt2.top = P.getyPos() + 25, rt2.bottom = P.getyPos() + P.getSize(), rt2.left = P.getxPos() + 5 , rt2.right = P.getxPos() + P.getSize() - 5;

	if (IntersectRect(&rt3, &rt1, &rt2))
		return true;
	else
		return false;
}

Boss::Boss(int i) 
{
	switch (i)
	{
	case 1:
		boss_base[1].Load(TEXT("보스1-1.png"));
		boss_base[2].Load(TEXT("보스1-2.png"));
		boss_base[3].Load(TEXT("보스1-3.png"));
		boss_base[4].Load(TEXT("보스1-4.png"));
		boss_base[5].Load(TEXT("보스1-5.png"));
		boss_base[6].Load(TEXT("보스1-6.png"));
		boss_base[7].Load(TEXT("보스1-7.png"));

		Pos.x = 35;
		Pos.y = -1853;

		type = 1;
		hp = 900;
		size = 600;
		break;
	case 2:
		type = 2;
		hp = 12;
		size = 150;
		break;
	}
}