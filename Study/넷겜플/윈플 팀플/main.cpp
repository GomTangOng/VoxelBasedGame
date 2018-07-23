#include <Windows.h>
#include <vector>
#include <atlImage.h>
#include <math.h>
#include "resource.h"
#include "SpaceShip.h"
#include "scene.h"
#include "ITEM.h"
#include "UI.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib") 

using namespace std;

#define WndX 640
#define WndY 825
#define RAD(x) 3.141592 / 180 * (x)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = "ApiBase";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(
		lpszClass, // 윈도우 클래스 이름
		"Window Title Name", // 윈도우 타이틀 이름
		WS_POPUPWINDOW, // 윈도우 스타일
		440, // 윈도우 위치, x좌표 
		0, // 윈도우 위치, y좌표
		WndX, // 윈도우 가로(폭) 크기 
		WndY, // 윈도우 세로(높이) 크기
		NULL, // 부모 윈도우 핶들
		NULL, // 메뉴 핶들
		hInstance, // 응용 프로그램 인스턴스
		NULL);   // 생성된 윈도우 정보
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

vector<Bullet> player_bullet1;
vector<Bullet> enemy_bullet1;
vector<Bullet> enemy_bullet2[3];
vector<Bullet> enemy_bullet3[8];
vector<Bullet> boss1_bullet1[36];
vector<Bullet> boss1_bullet2[36];
vector<Bullet> boss1_bullet3[3];
vector<Bullet> boss1_bullet4[8];
vector<Bullet> boss2_bullet[36];

vector<Enemy> enemy1;
vector<Enemy> enemy2;
vector<Enemy> enemy3;
vector<Enemy> boss2;

vector<I_BULLET> I_bullet;
vector<I_SUB> I_sub;
vector<I_POWER> I_power;
vector<I_SKILL> I_skill;
vector<I_SHEILD> I_sheild;

Player player;
Boss boss1(1);
HBITMAP membit;
SCENE scene;
HBRUSH hBrush, oldBrush;
UI_LIFE Ui_Life(player.getHp());
UI_BOMB Ui_Bomb(player.getBomb());
UI_SCORE Ui_Score;

CImage back;
CImage warning;
CImage player_base[10];
CImage enemy_base[10];
CImage player_bullet[10];
CImage enemy_bullet[10];
CImage boss_bullet[10];
CImage Boomimg[4];
CImage Item[10];

int MapY1 = 0, MapY2 = 0, mx, my;
int wX = -WndX, wY = 250;
int Player_AutoMove = 0;
bool CrashtoPlayer = false;
bool GoLeft = false, GoRight = false, GoUp = false, GoDown = false;
void ImgLoad();
void Bullet_Check1(vector<Bullet>& Bullet, vector<Enemy>& Target);
void Bullet_Check2(HWND hwnd, vector<Bullet>& Bullet, Player& P);
void DrawEnemy(HDC hdc);
void DrawBullet(HDC hdc);
void DrawItem(HDC hdc);
void BoomEnemy(HDC hdc);
void ItemGet();
void ItemMove();
void AllDie();

int BulletCount = 0;
bool Boss1_Appear = false;
bool Boss2_Appear = true;
bool Boss1_Stop = false;
int radian[8] = { 0, 45, 90, 135, 180, 225, 270, 315 }; // 0 맨우측
int radian3[36] = { 5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185,
					195, 205, 215, 225, 235, 365, 255, 265, 275, 285, 295, 305, 315, 325, 335, 345, 355 };
int Boss1_BulletCount1 = 0;
int ItemTimeCount = 0;
int FinalScore = 0;

bool Skill_On = false;
bool Warning_On = false;
bool Warning_Next = false;

void SetAllTimer(HWND hwnd);
void KillAllTimer(HWND hwnd);
void AllReset(HWND hwnd);

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc;

	switch (iMessage) {
	case WM_CREATE:
		PlaySound("Pentakill - Last Whisper", NULL, SND_ASYNC | SND_LOOP);
		ImgLoad();
		player_bullet1.reserve(1000); //미리 공간 만들어줌
		enemy_bullet1.reserve(100);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'V':
		case 'v':
			if (!Skill_On)
			{
				if (player.getBomb() > 0)
				{
					Skill_On = true;
					player.setBomb(player.getBomb() - 1);
					Ui_Bomb.setNum(player.getBomb());
				}
			}
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (scene.SceneCheck() == 0) 
		{
			if (scene.buttonCheck(mx, my, WndX, WndY) == 1) 
			{
				scene.SetIngame(true);
				SetAllTimer(hwnd);
				scene.SetScene(1);
				PlaySound(NULL, NULL, NULL);
				PlaySound("Pentakill - Deathfire Grasp", NULL, SND_ASYNC | SND_LOOP);
			}
			else if (scene.buttonCheck(mx, my, WndX, WndY) == 2)
				PostQuitMessage(0);
		}
		else if (scene.SceneCheck() == 2)
		{
			switch (scene.buttonCheck(mx, my, WndX, WndY))
			{
			case 1:
				scene.SetIngame(true);
				SetAllTimer(hwnd);
				scene.SetScene(1);
				break;
			case 2:
				PlaySound(NULL, NULL, NULL);
				PlaySound("Pentakill - Last Whisper", NULL, SND_ASYNC | SND_LOOP);
				AllReset(hwnd);
				break;
			case 3:
				PostQuitMessage(0);
				break;
			}
		}
		else if (scene.SceneCheck() == 3)
		{
			switch (scene.buttonCheck(mx, my, WndX, WndY))
			{
			case 1:
				PlaySound(NULL, NULL, NULL);
				PlaySound("Pentakill - Last Whisper", NULL, SND_ASYNC | SND_LOOP);
				AllReset(hwnd);
				break;
			case 2:
				PostQuitMessage(0);
				break;
			}
		}
		else if (scene.SceneCheck() == 4)
		{
			switch (scene.buttonCheck(mx, my, WndX, WndY))
			{
			case 1:
				PlaySound(NULL, NULL, NULL);
				PlaySound("Pentakill - Last Whisper", NULL, SND_ASYNC | SND_LOOP);
				AllReset(hwnd);
				break;
			case 2:
				PostQuitMessage(0);
				break;
			}
		}
		InvalidateRect(hwnd, NULL, false);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			GoUp = true;
			break;
		case VK_DOWN:
			GoDown = true;
			break;
		case VK_LEFT:
			GoLeft = true;
			player.setLR(2);
			break;
		case VK_RIGHT:
			GoRight = true;
			player.setLR(1);
			break;
		case VK_SPACE:
			BulletCount++;
			break;
		case VK_ESCAPE:
			if (scene.SceneCheck() == 1)
			{
				scene.SetIngame(false);
				scene.SetScene(2);
				KillAllTimer(hwnd);
			}
			else if (scene.SceneCheck() ==2)
			{
				scene.SetIngame(true);
				scene.SetScene(1);
				SetAllTimer(hwnd);
			}
			break;
		case 'Q':
			PostQuitMessage(0);
			break;
		}
		InvalidateRect(hwnd, NULL, false);
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_UP:
			GoUp = false;
			break;
		case VK_DOWN:
			GoDown = false;
			break;
		case VK_LEFT:
			GoLeft = false;
			player.setLR(0);
			break;
		case VK_RIGHT:
			GoRight = false;
			player.setLR(0);
			break;
		case VK_SPACE:
			BulletCount = 0;
			break;
		}
		InvalidateRect(hwnd, NULL, false);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			
			// ----------------------------------------------------------------------------------------------
			//											플레이어 이동
			// ----------------------------------------------------------------------------------------------
			
			if (Player_AutoMove < 20)
			{
				Player_AutoMove++;
				player.setPos(player.getPos().x, player.getPos().y - 5);
			}
			if (GoUp)
			{
				if (player.getPos().y > 0)
					player.setPos(player.getPos().x, player.getPos().y - 5);
			}
			if (GoDown)
			{
				if (player.getPos().y < 775)
					player.setPos(player.getPos().x, player.getPos().y + 5);
			}
			if (GoLeft)
			{
				if (player.getPos().x > 0)
					player.setPos(player.getPos().x - 5, player.getPos().y);
			}
			if (GoRight)
			{
				if (player.getPos().x < 590)
					player.setPos(player.getPos().x + 5, player.getPos().y);
			}

				// ----------------------------------------------------------------------------------------------
				//											맵 이동
				// ----------------------------------------------------------------------------------------------
			
			if (MapY1 <= WndY)
			{
				MapY1 += 7;
			}
			else
			{
				MapY1 = 0;
			}

				// ----------------------------------------------------------------------------------------------
				//											총알 이동
				// ----------------------------------------------------------------------------------------------
			
			for (auto p = player_bullet1.begin(); p < player_bullet1.end(); ++p)
				p->setyPos(p->getyPos() - 13);

			if (Boss1_Stop == false)
			{
				for (auto p = enemy_bullet1.begin(); p < enemy_bullet1.end(); ++p)
					p->setyPos(p->getyPos() + 7);

				for (int i = 0; i < 3; i++)
					for (auto p = enemy_bullet2[i].begin(); p < enemy_bullet2[i].end(); ++p)
					{
						switch (i)
						{
						case 0:
							p->setyPos(p->getyPos() + 7);
							p->setxPos(p->getxPos() - 2);
							break;
						case 1:
							p->setyPos(p->getyPos() + 7);
							break;
						case 2:
							p->setyPos(p->getyPos() + 7);
							p->setxPos(p->getxPos() + 2);
							break;
						}
					}

				for (int i = 0; i < 8; i++)
					for (auto p = enemy_bullet3[i].begin(); p < enemy_bullet3[i].end(); ++p)
					{
						float dx = cos(RAD(radian[i]));
						float dy = sin(RAD(radian[i]));
						p->setxPos(p->getxPos() + 6 * dx);
						p->setyPos(p->getyPos() + 6 * dy);
					}

				for (int i = 0; i < 36; i++)
					for (auto p = boss2_bullet[i].begin(); p < boss2_bullet[i].end(); ++p)
					{
						float dx = cos(RAD(radian3[i]));
						float dy = sin(RAD(radian3[i]));
						p->setxPos(p->getxPos() + 6 * dx);
						p->setyPos(p->getyPos() + 6 * dy);
					}
			}

			if (Boss1_Appear == true)
			{
				for (int i = 0; i < 36; i++)
					for (auto p = boss1_bullet1[i].begin(); p < boss1_bullet1[i].end(); ++p)
					{
						float dx = cos(RAD(radian3[i]));
						float dy = sin(RAD(radian3[i]));
						p->setxPos(p->getxPos() + 6 * dx);
						p->setyPos(p->getyPos() + 6 * dy);
					}

				for (int i = 0; i < 36; i++)
					for (auto p = boss1_bullet2[i].begin(); p < boss1_bullet2[i].end(); ++p)
					{
						float dx = cos(RAD(radian3[i]));
						float dy = sin(RAD(radian3[i]));
						p->setxPos(p->getxPos() + 6 * dx);
						p->setyPos(p->getyPos() + 6 * dy);
					}

				for (int i = 0; i < 3; i++)
					for (auto p = boss1_bullet3[i].begin(); p < boss1_bullet3[i].end(); ++p)
					{
						switch (i)
						{
						case 0:
							p->setyPos(p->getyPos() + 2);
							p->setxPos(p->getxPos() - 2);
							break;
						case 1:
							p->setyPos(p->getyPos() + 2);
							break;
						case 2:
							p->setyPos(p->getyPos() + 2);
							p->setxPos(p->getxPos() + 2);
							break;
						}
					}
			}

				// ----------------------------------------------------------------------------------------------
				//											적비행기 이동
				// ----------------------------------------------------------------------------------------------
			
			for (auto p = enemy1.begin(); p < enemy1.end(); ++p)
				p->setyPos(p->getyPos() + 2);
			for (auto p = enemy2.begin(); p < enemy2.end(); ++p)
				if (p->getyPos() < 100) p->setyPos(p->getyPos() + 3);
			for (auto p = enemy3.begin(); p < enemy3.end(); ++p)
				p->setyPos(p->getyPos() + 1);
			for (auto p = boss2.begin(); p < boss2.end(); ++p)
				if (p->getyPos() < 10) p->setyPos(p->getyPos() + 3);
			if (Boss1_Appear == true && boss1.getyPos() < -70)
				boss1.setyPos(boss1.getyPos() + 1);
			else if (Boss1_Appear == true && boss1.getyPos() >= -70)
				Boss1_Stop = true;

				// ----------------------------------------------------------------------------------------------
				//											총알 충돌체크
				// ----------------------------------------------------------------------------------------------

			Bullet_Check1(player_bullet1, enemy1);
			Bullet_Check1(player_bullet1, enemy2);
			Bullet_Check1(player_bullet1, enemy3);
			Bullet_Check1(player_bullet1, boss2);
			for (auto p = player_bullet1.begin(); p < player_bullet1.end();)
			{
				if (p->getyPos() < 250 && p->getxPos() > 270 && p->getxPos() < 350
					&& Boss1_Stop == true)
				{
					Ui_Score.setScore(Ui_Score.getScore() + 1);
					boss1.setHp(boss1.getHp() - 1);
					p = player_bullet1.erase(p);
					if (boss1.getHp() <= 0 && scene.SceneCheck() != 3)
					{
						FinalScore = Ui_Score.getScore();
						scene.SetScene(3);
					}
				}
				else
					++p;
			}

			Bullet_Check2(hwnd, enemy_bullet1, player);
			for (int i = 0; i < 3; ++i)
			{
				Bullet_Check2(hwnd, enemy_bullet2[i], player);
				Bullet_Check2(hwnd, boss1_bullet3[i], player);
			}
			for (int i = 0; i < 8; ++i)
			{
				Bullet_Check2(hwnd, enemy_bullet3[i], player);
				Bullet_Check2(hwnd, boss1_bullet4[i], player);
			}
			for (int i = 0; i < 36; ++i)
			{
				Bullet_Check2(hwnd, boss2_bullet[i], player);
				Bullet_Check2(hwnd, boss1_bullet1[i], player);
				Bullet_Check2(hwnd, boss1_bullet2[i], player);
			}

				// ----------------------------------------------------------------------------------------------
				//											아이템 생성
				// ----------------------------------------------------------------------------------------------

			ItemTimeCount++;
			if (ItemTimeCount % 3900 == 0)
				I_power.emplace_back(&Item[2]);
			if (ItemTimeCount % 4800 == 0)
				I_skill.emplace_back(&Item[3]);
			if (ItemTimeCount % 1250 == 0)
				I_bullet.emplace_back(&Item[0]);
			if (ItemTimeCount % 3200 == 0)
				I_sub.emplace_back(&Item[1]);
			if (ItemTimeCount % 5800 == 0)
				I_sheild.emplace_back(&Item[4]);

			ItemMove();
			ItemGet();

			if (Skill_On)
			{
				player.setSkillyPos(player.getSkillyPos() - 7);
				if (player.getSkillyPos() < -750)
				{
					Skill_On = false;
					player.setSkillyPos(850);
				}
				AllDie();
			}

			if (player.getPowerUp())
			{
				if (player.getPowerUpCount() < 1000)
					player.SetPowerUpCount(player.getPowerUpCount() + 1);
				else
				{
					player.setPowerUp(false);
					player.SetPowerUpCount(0);
				}
			}

			if (player.getBarrier())
			{
				if (player.getBarrierCount() < 500)
					player.SetBarrierCount(player.getBarrierCount() + 1);
				else
				{
					player.setBarrier(false);
					player.SetBarrierCount(0);
				}
			}
			break;
			
		case 2:		// 플레이어 총알 생성
			if (player.haveSub())
			{
				POINT tmp = player.getPos();
				tmp.x = player.getxPos() - 40;
				tmp.y = player.getyPos() + 25;
				player_bullet1.emplace_back(tmp, &player_bullet[3], -1);
				tmp.x = player.getxPos() + 40;
				player_bullet1.emplace_back(tmp, &player_bullet[3], -1);
			}

			if (BulletCount != 0) {
				if (!player.getPowerUp())
				{
					if (player.getBullet() == 1)
						player_bullet1.emplace_back(player.getPos(), &player_bullet[1], 0);
					else if (player.getBullet() == 2)
					{
						POINT tmp = player.getPos();
						tmp.x = player.getxPos() - 20;
						player_bullet1.emplace_back(tmp, &player_bullet[1], 0);
						tmp.x = player.getxPos() + 20;
						player_bullet1.emplace_back(tmp, &player_bullet[1], 0);
					}
					else
					{
						POINT tmp = player.getPos();
						tmp.y = player.getyPos() - 30;
						player_bullet1.emplace_back(tmp, &player_bullet[1], 0);
						tmp.y = player.getyPos() - 20;
						tmp.x = player.getxPos() - 20;
						player_bullet1.emplace_back(tmp, &player_bullet[1], 0);
						tmp.x = player.getxPos() + 20;
						player_bullet1.emplace_back(tmp, &player_bullet[1], 0);
					}
				}
				else
				{
					if (player.getBullet() == 1)
						player_bullet1.emplace_back(player.getPos(), &player_bullet[2], 0);
					else if (player.getBullet() == 2)
					{
						POINT tmp = player.getPos();
						tmp.x = player.getxPos() - 20;
						player_bullet1.emplace_back(tmp, &player_bullet[2], 0);
						tmp.x = player.getxPos() + 20;
						player_bullet1.emplace_back(tmp, &player_bullet[2], 0);
					}
					else
					{
						POINT tmp = player.getPos();
						tmp.y = player.getyPos() - 30;
						player_bullet1.emplace_back(tmp, &player_bullet[2], 0);
						tmp.y = player.getyPos() - 20;
						tmp.x = player.getxPos() - 20;
						player_bullet1.emplace_back(tmp, &player_bullet[2], 0);
						tmp.x = player.getxPos() + 20;
						player_bullet1.emplace_back(tmp, &player_bullet[2], 0);
					}
				}

				for (auto p = player_bullet1.begin(); p < player_bullet1.end();)
				{
					if (p->getyPos() < 0)
						p = player_bullet1.erase(p);
					else
						++p;
				}
			}
			break;

		case 3:		// 적 비행기1 생성
			enemy1.emplace_back(&enemy_base[1], 1);
			for (auto p = enemy1.begin(); p < enemy1.end();)
			{
				if (p->getyPos() > 1000)
					p = enemy1.erase(p);
				else
					++p;
			}
			break;

		case 4:		// 적 비행기2 생성
			enemy2.emplace_back(&enemy_base[2], 2);
			break;

		case 5:		// 적 총알1 생성
			for (auto p = enemy1.begin(); p < enemy1.end(); ++p)
				enemy_bullet1.emplace_back(p->getPos(), &enemy_bullet[1], 1);

			for (auto p = enemy_bullet1.begin(); p < enemy_bullet1.end();)
			{
				if (p->getyPos() > 1000)
					p = enemy_bullet1.erase(p);
				else
					++p;
			}
			break;

		case 6: // 적 총알2 생성
			for (int i = 0; i < 3; i++)
			{
				for (auto p = enemy2.begin(); p < enemy2.end(); ++p)
				{
					enemy_bullet2[i].emplace_back(p->getPos(), &enemy_bullet[2], 2);
				}

				for (auto p = enemy_bullet2[i].begin(); p < enemy_bullet2[i].end();)
				{
					if (p->getyPos() > 1000)
						p = enemy_bullet2[i].erase(p);
					else
						++p;
				}
			}
			break;

		case 7:	// 적 비행기3 생성
			enemy3.emplace_back(&enemy_base[3], 3);
			for (auto p = enemy3.begin(); p < enemy3.end();)
			{
				if (p->getyPos() > 1000)
					p = enemy3.erase(p);
				else
					++p;
			}
			break;

		case 8:	// 적 총알3 생성
			for (int i = 0; i < 8; i++)
			{
				for (auto p = enemy3.begin(); p < enemy3.end(); ++p)
					enemy_bullet3[i].emplace_back(p->getPos(), &enemy_bullet[3], 3);

				for (auto p = enemy_bullet3[i].begin(); p < enemy_bullet3[i].end();)
				{
					if (p->getyPos() > 1000 || p->getyPos() < 0
						|| p->getxPos() > 1000 || p->getxPos() < 0)
						p = enemy_bullet3[i].erase(p);
					else
						++p;
				}
			}
			break;

		case 9:	// 보스 생성
			if (!Boss1_Appear) {
				Boss1_Appear = true;
				Warning_On = true;
				PlaySound("NEXT - Lazenca Save Us", NULL, SND_ASYNC | SND_LOOP);
			}
			for (int i = 3; i < 9; i++)
				KillTimer(hwnd, i);
			break;

		case 10:	// 보스, 적 피격 애니메이션
			if (Boss1_Appear == true)
			{
				if (boss1.getImg() < 7)
					boss1.setImg(boss1.getImg() + 1);
				else
					boss1.setImg(1);
			}

			for (auto p = enemy1.begin(); p < enemy1.end(); ++p)
			{
				if (p->IsBoom())
				{
					if (p->getBoomCount() < 4)
						p->SetBoomCount(p->getBoomCount() + 1);
				}
			}

			for (auto p = enemy2.begin(); p < enemy2.end(); ++p)
			{
				if (p->IsBoom())
				{
					if (p->getBoomCount() < 4)
						p->SetBoomCount(p->getBoomCount() + 1);
				}
			}

			for (auto p = enemy3.begin(); p < enemy3.end(); ++p)
			{
				if (p->IsBoom())
				{
					if (p->getBoomCount() < 4)
						p->SetBoomCount(p->getBoomCount() + 1);
				}
			}

			for (auto p = boss2.begin(); p < boss2.end(); ++p)
			{
				if (p->IsBoom())
				{
					if (p->getBoomCount() < 4)
						p->SetBoomCount(p->getBoomCount() + 1);
				}
			}
			break;

		case 11:	// 보스 총알1 생성
			for (int i = 0; i < 24; i++)
			{
				if (Boss1_Stop == true && boss1.getHp() > 300 && boss1.getHp() <= 600)
				{
					boss1_bullet1[i].emplace_back(boss1.getPos(), &boss_bullet[1], 4);
				}

				for (auto p = boss1_bullet1[i].begin(); p < boss1_bullet1[i].end();)
				{
					if (p->getyPos() > 1000 || p->getyPos() < 0
						|| p->getxPos() > 1000 || p->getxPos() < 0)
						p = boss1_bullet1[i].erase(p);
					else
						++p;
				}
			}
			break;

		case 12:	// 보스 총알2 생성
			for (int i = 0; i < 8; i++)
				for (auto p = boss1_bullet4[i].begin(); p < boss1_bullet4[i].end(); ++p)
				{
					float dx = cos(RAD(radian[i]));
					float dy = sin(RAD(radian[i]));
					p->setxPos(p->getxPos() + 6 * dx);
					p->setyPos(p->getyPos() + 6 * dy);
				}
			
			Boss1_BulletCount1++;
			if (Boss1_BulletCount1 == 35) Boss1_BulletCount1 = 0;
			for (int i = Boss1_BulletCount1; i < Boss1_BulletCount1 + 1; i++)
				if (Boss1_Stop == true && boss1.getHp() > 600 && boss1.getHp() <= 900)
				{
					boss1_bullet2[i].emplace_back(boss1.getPos(), &boss_bullet[1], 4);
				}
			for (int i = 0; i < 36; i++)
				for (auto p = boss1_bullet2[i].begin(); p < boss1_bullet2[i].end();)
				{
					if (p->getyPos() > 1000 || p->getyPos() < 0
						|| p->getxPos() > 1000 || p->getxPos() < 0)
						p = boss1_bullet2[i].erase(p);
					else
						++p;
				}
			break;

		case 13: // 보스 총알3 생성
			for (int i = 0; i < 3; i++)
			{
				if (Boss1_Stop == true && boss1.getHp() > 0 && boss1.getHp() <= 300)
				{
					boss1_bullet3[i].emplace_back(boss1.getPos(), &boss_bullet[2], 6);
				}

				for (auto p = boss1_bullet3[i].begin(); p < boss1_bullet3[i].end();)
				{
					if (p->getyPos() > 800)
						p = boss1_bullet3[i].erase(p);
					else
						++p;
				}
			}
			break;

		case 14:	// 보스 총알3 - 1 생성
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 3; j++)
				{
					if(j != 1 && Boss1_Stop == true && boss1.getHp() > 0 && boss1.getHp() <= 300)
					for (auto p = boss1_bullet3[j].begin(); p < boss1_bullet3[j].end(); ++p)
						boss1_bullet4[i].emplace_back(p->getPos(), &boss_bullet[1], 5);

					for (auto p = boss1_bullet4[i].begin(); p < boss1_bullet4[i].end();)
					{
						if (p->getyPos() > 800 || p->getyPos() < 0
							|| p->getxPos() > 800 || p->getxPos() < -200)
							p = boss1_bullet4[i].erase(p);
						else
							++p;
					}
				}
			break;

		case 15:	// 보스 경고문 이동1
			if (Warning_On == true && wX < 0) wX += 160;
			if (Warning_On == true && wX == 0) {
				SetTimer(hwnd, 16, 2000, NULL); // 보스 경고문 이동2
				wX++;
			}
			if (Warning_On == true && Warning_Next == true) wX += 160;
			if (wX > 1000)
			{
				Warning_On = false;
				KillTimer(hwnd, 15);
				KillTimer(hwnd, 16);
			}
			break;

		case 16:	// 보스 경고문 이동2
			Warning_Next = true;
			break;

		case 17:	// 중간보스 생성
			if (Boss2_Appear == true)
			{
				boss2.emplace_back(&enemy_base[4], 4);
				Boss2_Appear = false;
			}
			break;

		case 18:	// 중간보스 총알 생성
			for (int i = 0; i < 24; i++)
			{
				for (auto p = boss2.begin(); p < boss2.end(); ++p)
					boss2_bullet[i].emplace_back(p->getPos(), &enemy_bullet[4], 7);

				for (auto p = boss2_bullet[i].begin(); p < boss2_bullet[i].end();)
				{
					if (p->getyPos() > 1000 || p->getyPos() < 0
						|| p->getxPos() > 1000 || p->getxPos() < 0)
						p = boss2_bullet[i].erase(p);
					else
						++p;
				}
			}
			break;
		case 19:	// 플레이어 피격 애니메이션
			CrashtoPlayer = false;
			KillTimer(hwnd, 19);
			break;
		}
		InvalidateRect(hwnd, NULL, false);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		membit = CreateCompatibleBitmap(hdc, WndX, WndY);
		SelectObject(memdc, membit);

		scene.draw(memdc, MapY1, MapY1, WndX, WndY, FinalScore);
		if (Warning_On == true) warning.Draw(memdc, wX, wY, WndX, 300);
		
		if (scene.Isplay() && scene.SceneCheck() == 1) {
			DrawEnemy(memdc);
			DrawBullet(memdc);
			if (Skill_On)
				player.usingBomb(memdc);
			BoomEnemy(memdc);
			DrawItem(memdc);
			player.Draw(memdc);
			if (player.getBarrier())
				player.usingBarrier(memdc);

			Ui_Life.draw(memdc);
			Ui_Bomb.draw(memdc);
			Ui_Score.draw(memdc);

			if (CrashtoPlayer == true)
			{
				hBrush = CreateSolidBrush(RGB(255, 0, 0));
				oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
				Rectangle(memdc, -10, -10, WndX, WndY);
				SelectObject(memdc, oldBrush);
				DeleteObject(hBrush);
			}
		}
		
		BitBlt(hdc, 0, 0, WndX, WndY, memdc, 0, 0, SRCCOPY);

		DeleteObject(membit);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		KillAllTimer(hwnd);

		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}

void ImgLoad()
{
	player_bullet[1].Load(TEXT("총알기본.png"));
	player_bullet[2].Load(TEXT("강화총알.png"));
	player_bullet[3].Load(TEXT("총알서브.png"));
	enemy_bullet[1].Load(TEXT("적총알기본.png"));
	enemy_bullet[2].Load(TEXT("적총알2.png"));
	enemy_bullet[3].Load(TEXT("적총알2.png"));
	enemy_bullet[4].Load(TEXT("적총알5.png"));
	boss_bullet[1].Load(TEXT("적총알3.png"));
	boss_bullet[2].Load(TEXT("적총알4.png"));

	enemy_base[1].Load(TEXT("적비행기1.png"));
	enemy_base[2].Load(TEXT("적비행기2.png"));
	enemy_base[3].Load(TEXT("적비행기3.png"));
	enemy_base[4].Load(TEXT("보스2.png"));

	Item[0].Load(TEXT("총알기본.png"));
	Item[1].Load(TEXT("ITEM(보조2).png"));
	Item[2].Load(TEXT("강화총알.png"));
	Item[3].Load(TEXT("ITEM(필살기).png"));
	Item[4].Load(TEXT("ITEM(방어막).png"));
	Item[5].Load(TEXT("필살기.png"));

	warning.Load(TEXT("보스경고.png"));

	Boomimg[0].Load(TEXT("폭발1.png"));
	Boomimg[1].Load(TEXT("폭발2.png"));
	Boomimg[2].Load(TEXT("폭발3.png"));
	Boomimg[3].Load(TEXT("폭발4.png"));
}

void Bullet_Check1(vector<Bullet>& Bullet, vector<Enemy>& Target)
{
	for (auto p = Bullet.begin(); p < Bullet.end(); )
	{
		for (auto q = Target.begin(); q < Target.end();)
		{
			if (p->IsCrashtoEnemy(*q))
			{
				if (p->getType() == -1)
					q->setHp(q->getHp() - 1);
				else if (p->getType() == 0)
				{
					if (!player.getPowerUp())
						q->setHp(q->getHp() - 2);
					else
						q->setHp(q->getHp() - 3);
				}
				if (!q->IsBoom()) 
				{
					Ui_Score.setScore(Ui_Score.getScore() + 1);
					p = Bullet.erase(p);
				}
				else
					++p;
				if (p == Bullet.end())
					break;
			}
			else
				++q;
		}
		if (p < Bullet.end())
			++p;
	}

	for (auto p = Target.begin(); p < Target.end();)
	{
		if (p->getHp() <= 0)
		{
			if (p->getBoomCount() > 3)
			{
				switch (p->getType())
				{
				case 1:
					Ui_Score.setScore(Ui_Score.getScore() + 20);
					break;
				case 2:
					Ui_Score.setScore(Ui_Score.getScore() + 100);
					break;
				case 3:
					Ui_Score.setScore(Ui_Score.getScore() + 200);
					break;
				case 4:
					Ui_Score.setScore(Ui_Score.getScore() + 500);
					break;
				}
			}
			if(!p->IsBoom())
				p->BOOM();
			if (p->getBoomCount() > 3)
				p = Target.erase(p);
			else
				++p;
		}
		else
			++p;
	}
}

void Bullet_Check2(HWND hwnd, vector<Bullet>& Bullet, Player& P)
{
	for (auto p = Bullet.begin(); p < Bullet.end(); )
	{
		if (p->IsCrashtoPlayer(P))
		{
			
			if (!P.getBarrier() && !Skill_On)
			{
				SetTimer(hwnd, 19, 50, NULL); // 플레이어 피격 애니메이션
				CrashtoPlayer = true;
				P.setHp(P.getHp() - 1);
				P.setBullet(1);
				P.setSub(false);
				P.setPowerUp(false);
				Ui_Life.setNum(P.getHp());
				if (P.getHp() <= 0 && scene.SceneCheck() != 3)
				{
					FinalScore = Ui_Score.getScore();
					scene.SetScene(4);
					return;
				}
			}
			p = Bullet.erase(p);
		}
		else
			++p;
	}
}

void DrawEnemy(HDC hdc)
{
	for (auto p = boss2.begin(); p < boss2.end(); ++p)
		p->Draw(hdc);
	for (auto p = enemy1.begin(); p < enemy1.end(); ++p)
		p->Draw(hdc);
	for (auto p = enemy2.begin(); p < enemy2.end(); ++p)
		p->Draw(hdc);
	for (auto p = enemy3.begin(); p < enemy3.end(); ++p)
		p->Draw(hdc);
	if (Boss1_Appear == true)
		boss1.Draw(hdc);
}

void BoomEnemy(HDC hdc)
{
	for (auto p = enemy1.begin(); p < enemy1.end(); ++p)
	{
		if (p->IsBoom())
		{
			if(p->getBoomCount() < 4)
				p->BoomDraw(hdc, &Boomimg[p->getBoomCount()]);
		}
	}

	for (auto p = enemy2.begin(); p < enemy2.end(); ++p)
	{
		if (p->IsBoom())
		{
			if (p->getBoomCount() < 4)
				p->BoomDraw(hdc, &Boomimg[p->getBoomCount()]);
		}
	}

	for (auto p = enemy3.begin(); p < enemy3.end(); ++p)
	{
		if (p->IsBoom())
		{
			if (p->getBoomCount() < 4)
				p->BoomDraw(hdc, &Boomimg[p->getBoomCount()]);
		}
	}

	for (auto p = boss2.begin(); p < boss2.end(); ++p)
	{
		if (p->IsBoom())
		{
			if (p->getBoomCount() < 4)
				p->BoomDraw(hdc, &Boomimg[p->getBoomCount()]);
		}
	}
}

void DrawBullet(HDC hdc)
{
	for (auto p = player_bullet1.begin(); p < player_bullet1.end(); ++p)
		p->Draw(hdc);
	for (auto p = enemy_bullet1.begin(); p < enemy_bullet1.end(); ++p)
		p->Draw(hdc);
	for (int i = 0; i < 3; i++)
		for (auto p = enemy_bullet2[i].begin(); p < enemy_bullet2[i].end(); ++p)
			p->Draw(hdc);
	for (int i = 0; i < 8; i++)
		for (auto p = enemy_bullet3[i].begin(); p < enemy_bullet3[i].end(); ++p)
			p->Draw(hdc);
	for (int i = 0; i < 24; i++)
		for (auto p = boss2_bullet[i].begin(); p < boss2_bullet[i].end(); ++p)
			p->Draw(hdc);
	for (int i = 0; i < 24; i++)
		for (auto p = boss1_bullet1[i].begin(); p < boss1_bullet1[i].end(); ++p)
			p->Draw(hdc);
	for (int i = 0; i < 24; i++)
		for (auto p = boss1_bullet2[i].begin(); p < boss1_bullet2[i].end(); ++p)
			p->Draw(hdc);
	for (int i = 0; i < 3; i++)
		for (auto p = boss1_bullet3[i].begin(); p < boss1_bullet3[i].end(); ++p)
			p->Draw(hdc);
	for (int i = 0; i < 8; i++)
		for (auto p = boss1_bullet4[i].begin(); p < boss1_bullet4[i].end(); ++p)
			p->Draw(hdc);
}
void DrawItem(HDC hdc)
{
	for (auto p = I_bullet.begin(); p < I_bullet.end(); ++p)
		p->draw(hdc);
	for (auto p = I_sub.begin(); p < I_sub.end(); ++p)
		p->draw(hdc);
	for (auto p = I_power.begin(); p < I_power.end(); ++p)
		p->draw(hdc);
	for (auto p = I_skill.begin(); p < I_skill.end(); ++p)
		p->draw(hdc);
	for (auto p = I_sheild.begin(); p < I_sheild.end(); ++p)
		p->draw(hdc);
}
void ItemMove()
{
	for (auto p = I_power.begin(); p < I_power.end(); ++p) // 파워 아이템 이동
	{
		if (p->getxPos() + p->getSize() > WndX)
			p->SetDir('x', false);
		else if (p->getxPos() < 0)
			p->SetDir('x', true);

		if (p->getyPos() + p->getSize() > WndY)
			p->SetDir('y', false);
		else if (p->getyPos() < 0)
			p->SetDir('y', true);

		if (p->getDir('x'))
			p->setxPos(p->getxPos() + 3);
		else
			p->setxPos(p->getxPos() - 3);

		if (p->getDir('y'))
			p->setyPos(p->getyPos() + 3);
		else
			p->setyPos(p->getyPos() - 3);
	}
	for (auto p = I_skill.begin(); p < I_skill.end(); ++p) // 필살기 아이템 이동
	{
		if (p->getxPos() + p->getSize() > WndX)
			p->SetDir('x', false);
		else if (p->getxPos() < 0)
			p->SetDir('x', true);

		if (p->getyPos() + p->getSize() > WndY)
			p->SetDir('y', false);
		else if (p->getyPos() < 0)
			p->SetDir('y', true);

		if (p->getDir('x'))
			p->setxPos(p->getxPos() + 3);
		else
			p->setxPos(p->getxPos() - 3);

		if (p->getDir('y'))
			p->setyPos(p->getyPos() + 3);
		else
			p->setyPos(p->getyPos() - 3);
	}

	for (auto p = I_bullet.begin(); p < I_bullet.end(); ++p) // 총알 아이템 이동
	{
		if (p->getxPos() + p->getSize() > WndX)
			p->SetDir('x', false);
		else if (p->getxPos() < 0)
			p->SetDir('x', true);

		if (p->getyPos() + p->getSize() > WndY)
			p->SetDir('y', false);
		else if (p->getyPos() < 0)
			p->SetDir('y', true);

		if (p->getDir('x'))
			p->setxPos(p->getxPos() + 3);
		else
			p->setxPos(p->getxPos() - 3);

		if (p->getDir('y'))
			p->setyPos(p->getyPos() + 3);
		else
			p->setyPos(p->getyPos() - 3);
	}

	for (auto p = I_sub.begin(); p < I_sub.end(); ++p) // 보조 아이템 이동
	{
		if (p->getxPos() + p->getSize() > WndX)
			p->SetDir('x', false);
		else if (p->getxPos() < 0)
			p->SetDir('x', true);

		if (p->getyPos() + p->getSize() > WndY)
			p->SetDir('y', false);
		else if (p->getyPos() < 0)
			p->SetDir('y', true);

		if (p->getDir('x'))
			p->setxPos(p->getxPos() + 3);
		else
			p->setxPos(p->getxPos() - 3);

		if (p->getDir('y'))
			p->setyPos(p->getyPos() + 3);
		else
			p->setyPos(p->getyPos() - 3);
	}

	for (auto p = I_sheild.begin(); p < I_sheild.end(); ++p) // 방어 아이템 이동
	{
		if (p->getxPos() + p->getSize() > WndX)
			p->SetDir('x', false);
		else if (p->getxPos() < 0)
			p->SetDir('x', true);

		if (p->getyPos() + p->getSize() > WndY)
			p->SetDir('y', false);
		else if (p->getyPos() < 0)
			p->SetDir('y', true);

		if (p->getDir('x'))
			p->setxPos(p->getxPos() + 3);
		else
			p->setxPos(p->getxPos() - 3);

		if (p->getDir('y'))
			p->setyPos(p->getyPos() + 3);
		else
			p->setyPos(p->getyPos() - 3);
	}
}

void ItemGet()
{
	for (auto p = I_power.begin(); p < I_power.end();)
	{
		if (p->IsGet(player))
		{
			if (!player.getPowerUp())
				player.setPowerUp(true);
			Ui_Score.setScore(Ui_Score.getScore() + 50);
			p = I_power.erase(p);
		}
		else
			++p;
	}

	for (auto p = I_skill.begin(); p < I_skill.end();)
	{
		if (p->IsGet(player))
		{
			Ui_Score.setScore(Ui_Score.getScore() + 50);
			p = I_skill.erase(p);
			if (player.getBomb() < 3)
			{
				player.setBomb(player.getBomb() + 1);
				Ui_Bomb.setNum(player.getBomb());
			}
		}
		else
			++p;
	}

	for (auto p = I_bullet.begin(); p < I_bullet.end();)
	{
		if (p->IsGet(player))
		{
			Ui_Score.setScore(Ui_Score.getScore() + 50);
			p = I_bullet.erase(p);
			if (player.getBullet() < 3)
				player.setBullet(player.getBullet() + 1);
		}
		else
			++p;
	}

	for (auto p = I_sub.begin(); p < I_sub.end();)
	{
		if (p->IsGet(player))
		{
			Ui_Score.setScore(Ui_Score.getScore() + 50);
			player.setSub(true);
			p = I_sub.erase(p);
		}
		else
			++p;
	}

	for (auto p = I_sheild.begin(); p < I_sheild.end();)
	{
		if (p->IsGet(player))
		{
			Ui_Score.setScore(Ui_Score.getScore() + 50);
			if (!player.getBarrier())
				player.setBarrier(true);
			p = I_sheild.erase(p);
		}
		else
			++p;
	}
}

void AllDie()
{
	for (auto p = enemy1.begin(); p < enemy1.end();)
	{
		if (player.getSkillyPos() <= p->getyPos() + p->getSize()) 
		{
			if (p->IsBoom())
			{
				if (p->getBoomCount() > 3)
				{
					if (scene.Isplay())
						Ui_Score.setScore(Ui_Score.getScore() + 20);
					p = enemy1.erase(p);
				}
				else
					++p;
			}
			else
			{
				p->BOOM();
				++p;
			}
			
		}
		else
			++p;
	}

	for (auto p = enemy2.begin(); p < enemy2.end();)
	{
		if (player.getSkillyPos() <= p->getyPos() + p->getSize())
		{
			if (p->IsBoom())
			{
				if (p->getBoomCount() > 3)
				{
					if (scene.Isplay())
						Ui_Score.setScore(Ui_Score.getScore() + 100);
					p = enemy2.erase(p);
				}
				else
					++p;
			}
			else
			{
				p->BOOM();
				++p;
			}

		}
		else
			++p;
	}

	for (auto p = enemy3.begin(); p < enemy3.end();)
	{
		if (player.getSkillyPos() <= p->getyPos() + p->getSize())
		{
			if (p->IsBoom())
			{
				if (p->getBoomCount() > 3)
				{
					if (scene.Isplay())
						Ui_Score.setScore(Ui_Score.getScore() + 200);
					p = enemy3.erase(p);
				}
				else
					++p;
			}
			else
			{
				p->BOOM();
				++p;
			}

		}
		else
			++p;
	}

	for (auto p = enemy_bullet1.begin(); p < enemy_bullet1.end();)
	{
		if (player.getSkillyPos() <= p->getyPos() + p->getSize())
		{
			if (scene.Isplay())
				Ui_Score.setScore(Ui_Score.getScore() + 10);
			p = enemy_bullet1.erase(p);
		}
		else
			++p;
	}

	for (int i = 0; i < 3; ++i)
	{
		for (auto p = enemy_bullet2[i].begin(); p < enemy_bullet2[i].end();)
		{
			if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			{
				if (scene.Isplay())
					Ui_Score.setScore(Ui_Score.getScore() + 10);
				p = enemy_bullet2[i].erase(p);
			}
			else
				++p;
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		for (auto p = enemy_bullet3[i].begin(); p < enemy_bullet3[i].end();)
		{
			if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			{
				if (scene.Isplay())
					Ui_Score.setScore(Ui_Score.getScore() + 10);
				p = enemy_bullet3[i].erase(p);
			}
			else
				++p;
		}
	}

	for (int i = 0; i < 36; ++i)
	{
		for (auto p = boss1_bullet1[i].begin(); p < boss1_bullet1[i].end();)
		{
			if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			{
				if (scene.Isplay())
					Ui_Score.setScore(Ui_Score.getScore() + 10);
				p = boss1_bullet1[i].erase(p);
			}
			else
				++p;
		}
	}

	for (int i = 0; i < 36; ++i)
	{
		for (auto p = boss1_bullet2[i].begin(); p < boss1_bullet2[i].end();)
		{
			if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			{
				if (scene.Isplay())
					Ui_Score.setScore(Ui_Score.getScore() + 10);
				p = boss1_bullet2[i].erase(p);
			}
			else
				++p;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (auto p = boss1_bullet3[i].begin(); p < boss1_bullet3[i].end();)
		{
			if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			{
				if (scene.Isplay())
					Ui_Score.setScore(Ui_Score.getScore() + 10);
				p = boss1_bullet3[i].erase(p);
			}
			else
				++p;
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		for (auto p = boss1_bullet4[i].begin(); p < boss1_bullet4[i].end();)
		{
			if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			{
				if (scene.Isplay())
					Ui_Score.setScore(Ui_Score.getScore() + 10);
				p = boss1_bullet4[i].erase(p);
			}
			else
				++p;
		}
	}

	for (int i = 0; i < 36; ++i)
	{
		for (auto p = boss2_bullet[i].begin(); p < boss2_bullet[i].end();)
		{
			if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			{
				if (scene.Isplay())
					Ui_Score.setScore(Ui_Score.getScore() + 10);
				p = boss2_bullet[i].erase(p);
			}
			else
				++p;
		}
	}

}

void SetAllTimer(HWND hwnd)
{
	SetTimer(hwnd, 1, 1, NULL); // 종합
	SetTimer(hwnd, 2, 200, NULL); // 플레이어 총알 생성
	SetTimer(hwnd, 3, 3000, NULL); // 적 비행기1 생성
	SetTimer(hwnd, 4, 8000, NULL); // 적 비행기2 생성
	SetTimer(hwnd, 5, 3500, NULL); // 적 총알 생성
	SetTimer(hwnd, 6, 2500, NULL); // 적 총알2 생성
	SetTimer(hwnd, 7, 20000, NULL); // 적 비행기3 생성
	SetTimer(hwnd, 8, 3000, NULL); // 적 총알3 생성
	SetTimer(hwnd, 9, 180000, NULL); // 보스 생성
	SetTimer(hwnd, 10, 90, NULL); // 보스, 적 피격 애니메이션
	SetTimer(hwnd, 11, 2000, NULL); // 보스 총알1 생성 
	SetTimer(hwnd, 12, 20, NULL); // 보스 총알2 생성
	SetTimer(hwnd, 13, 3000, NULL); // 보스 총알3 생성
	SetTimer(hwnd, 14, 3500, NULL); // 보스 총알3 - 1 생성
	SetTimer(hwnd, 15, 1, NULL); // 보스 경고문 이동1
								 // 보스 경고문 이동2 ( 16번째 )
	SetTimer(hwnd, 17, 100000, NULL); // 중간보스 생성
	SetTimer(hwnd, 18, 4000, NULL); // 중간보스 총알 생성
									// 플레이어 피격 애니메이션 ( 19번째 )
}

void KillAllTimer(HWND hwnd)
{
	for (int i = 1; i < 19; ++i)
		KillTimer(hwnd, i);
}

void AllReset(HWND hwnd)
{
	AllDie();
	Ui_Score.setScore(0);
	KillAllTimer(hwnd);
	Player_AutoMove = 0;
	Skill_On = false;
	Boss1_Appear = false;
	Boss2_Appear = true;
	Boss1_Stop = false;
	Warning_On = false;
	Warning_Next = false;
	CrashtoPlayer = false;
	BulletCount = 0;
	Boss1_BulletCount1 = 0;
	ItemTimeCount = 0;
	MapY1 = 0, MapY2 = 0;
	wX = -WndX, wY = 250;
	FinalScore = 0;

	player_bullet1.clear();
	enemy_bullet1.clear();
	for (int i = 0; i < 3; ++i)
		enemy_bullet2[i].clear();
	for (int i = 0; i < 8; ++i)
		enemy_bullet3[i].clear();
	for (int i = 0; i < 24; ++i)
		boss1_bullet1[i].clear();
	for (int i = 0; i < 24; ++i)
		boss1_bullet2[i].clear();
	for (int i = 0; i < 3; ++i)
		boss1_bullet3[i].clear();
	for (int i = 0; i < 8; ++i)
		boss1_bullet4[i].clear();
	for (int i = 0; i < 24; ++i)
		boss2_bullet[i].clear();

	enemy1.clear();
	enemy2.clear();
	enemy3.clear();
	boss2.clear();
	boss1.setPos(35, -1853);
	boss1.setHp(900);

	I_bullet.clear();
	I_sub.clear();
	I_power.clear();
	I_skill.clear();
	I_sheild.clear();

	player.setSkillyPos(850);
	player.setSub(false);
	player.setPowerUp(false);
	player.setBarrier(false);
	player.setPos(300, 825);
	player.setHp(3);
	player.setBullet(1);
	player.setBomb(3);
	player.SetPowerUpCount(0);
	player.SetBarrierCount(0);

	Ui_Life.setNum(player.getHp());
	Ui_Bomb.setNum(player.getBomb());
	scene.SetScene(0);
}