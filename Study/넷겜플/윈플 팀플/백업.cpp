// 2018 06 14 수정본
#include <Windows.h>
#include <vector>
#include <atlImage.h>
#include <math.h>
#include "resource.h"
#include "SpaceShip.h"
#include "scene.h"
#include "ITEM.h"

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

vector<Enemy> enemy1;
vector<Enemy> enemy2;
vector<Enemy> enemy3;

vector<I_BULLET> I_bullet;
vector<I_SUB> I_sub;
vector<I_POWER> I_power;
vector<I_SKILL> I_skill;
vector<I_SHEILD> I_sheild;

Player player;
Boss boss1(1);
HBITMAP membit;
SCENE scene;

CImage back;
CImage warning;
CImage player_base[10];
CImage enemy_base[10];
CImage player_bullet[10];
CImage enemy_bullet[10];
CImage boss_bullet[10];

CImage Item[10];

int MapY1 = 0, MapY2 = 0, mx, my;
int wX = -WndX, wY = 250;
int Player_AutoMove = 0;
int SceneCheck = 0;
bool GoLeft = false, GoRight = false, GoUp = false, GoDown = false;
void ImgLoad();
void Bullet_Check1(vector<Bullet>& Bullet, vector<Enemy>& Target);
void Bullet_Check2(HWND hwnd, vector<Bullet>& Bullet, Player& P);
void DrawEnemy(HDC hdc);
void DrawBullet(HDC hdc);
void DrawItem(HDC hdc);
void ItemGet();
void ItemMove();
void AllDie();

int BulletCount = 0;
bool Boss1_Appear = false;
bool Boss1_Stop = false;
int radian[8] = { 0, 45, 90, 135, 180, 225, 270, 315 }; // 0 맨우측
int radian3[36] = { 5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185,
195, 205, 215, 225, 235, 245, 255, 265, 275, 285, 295, 305, 315, 325, 335, 345, 355 };
int Boss1_BulletCount1 = 0;

int ItemTimeCount = 0;

bool Skill_On = false;
bool Warning_On = false;
bool Warning_Next = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM
	wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc;

	switch (iMessage) {
	case WM_CREATE:
		ImgLoad();
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'v':
			if (!Skill_On)
			{
				if (player.getBomb() > 0)
				{
					Skill_On = true;
					player.setBomb(player.getBomb() - 1);
				}
			}
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (SceneCheck == 0) {
			if (scene.buttonCheck(mx, my, WndX, WndY) == 1) {
				scene.SetIngame(true);
				SetTimer(hwnd, 1, 1, NULL); // 플레이어 이동
				SetTimer(hwnd, 2, 1, NULL); // 맵이동
				SetTimer(hwnd, 3, 1, NULL); // 총알 이동
				SetTimer(hwnd, 4, 150, NULL); // 플레이어 총알 생성
				SetTimer(hwnd, 5, 1, NULL); // 적 비행기 이동
				SetTimer(hwnd, 6, 3000, NULL); // 적 비행기1 생성
				SetTimer(hwnd, 7, 8000, NULL); // 적 비행기2 생성
				SetTimer(hwnd, 8, 1500, NULL); // 적 총알 생성
				SetTimer(hwnd, 9, 2000, NULL); // 적 총알2 생성
				SetTimer(hwnd, 10, 1, NULL); // 총알 충돌체크
				SetTimer(hwnd, 11, 20000, NULL); // 적 비행기3 생성
				SetTimer(hwnd, 12, 1500, NULL); // 적 총알3 생성
				SetTimer(hwnd, 13, 60000, NULL); // 보스1 생성
				SetTimer(hwnd, 14, 90, NULL); // 보스1 애니메이션
				SetTimer(hwnd, 15, 2000, NULL); // 보스 총알1 생성
				SetTimer(hwnd, 16, 1, NULL); // 아이템 생성
				SetTimer(hwnd, 17, 10, NULL); // 보스 총알2 생성
				SetTimer(hwnd, 18, 2000, NULL); // 보스 총알3 생성
				SetTimer(hwnd, 19, 1500, NULL); // 보스 총알3 - 1 생성
				SetTimer(hwnd, 20, 1, NULL); // 보스 경고문 이동1
											 // 보스 경고문 이동2 ( 21번째 )
				SceneCheck = 1;
			}
			else if (scene.buttonCheck(mx, my, WndX, WndY) == 2)
				PostQuitMessage(0);
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
		case 1:		// 플레이어 이동
			if (Player_AutoMove < 20)
			{
				Player_AutoMove++;
				player.setPos(player.getPos().x, player.getPos().y - 8);
			}
			if (GoUp)
			{
				if (player.getPos().y > 0)
					player.setPos(player.getPos().x, player.getPos().y - 8);
			}
			if (GoDown)
			{
				if (player.getPos().y < 775)
					player.setPos(player.getPos().x, player.getPos().y + 8);
			}
			if (GoLeft)
			{
				if (player.getPos().x > 0)
					player.setPos(player.getPos().x - 8, player.getPos().y);
			}
			if (GoRight)
			{
				if (player.getPos().x < 590)
					player.setPos(player.getPos().x + 8, player.getPos().y);
			}
			break;

		case 2:		 // 맵이동
			if (MapY1 != WndY)
			{
				MapY1 += 5;
				MapY2 += 5;
			}
			else
			{
				MapY1 = 0;
				MapY2 = 0;
			}
			break;

		case 3:		// 총알 이동
			for (auto p = player_bullet1.begin(); p < player_bullet1.end(); ++p)
				p->setyPos(p->getyPos() - 20);

			for (auto p = enemy_bullet1.begin(); p < enemy_bullet1.end(); ++p)
				p->setyPos(p->getyPos() + 10);

			for (int i = 0; i < 3; i++)
				for (auto p = enemy_bullet2[i].begin(); p < enemy_bullet2[i].end(); ++p)
				{
					switch (i)
					{
					case 0:
						p->setyPos(p->getyPos() + 10);
						p->setxPos(p->getxPos() - 3);
						break;
					case 1:
						p->setyPos(p->getyPos() + 10);
						break;
					case 2:
						p->setyPos(p->getyPos() + 10);
						p->setxPos(p->getxPos() + 3);
						break;
					}
				}

			for (int i = 0; i < 8; i++)
				for (auto p = enemy_bullet3[i].begin(); p < enemy_bullet3[i].end(); ++p)
				{
					float dx = cos(RAD(radian[i]));
					float dy = sin(RAD(radian[i]));
					p->setxPos(p->getxPos() + 8 * dx);
					p->setyPos(p->getyPos() + 8 * dy);
				}

			for (int i = 0; i < 36; i++)
				for (auto p = boss1_bullet1[i].begin(); p < boss1_bullet1[i].end(); ++p)
				{
					double dx = cos(RAD(radian3[i]));
					double dy = sin(RAD(radian3[i]));
					p->setxPos(p->getxPos() + 6 * dx);
					p->setyPos(p->getyPos() + 6 * dy);
				}

			for (int i = 0; i < 36; i++)
				for (auto p = boss1_bullet2[i].begin(); p < boss1_bullet2[i].end(); ++p)
				{
					double dx = cos(RAD(radian3[i]));
					double dy = sin(RAD(radian3[i]));
					p->setxPos(p->getxPos() + 6 * dx);
					p->setyPos(p->getyPos() + 6 * dy);
				}

			for (int i = 0; i < 3; i++)
				for (auto p = boss1_bullet3[i].begin(); p < boss1_bullet3[i].end(); ++p)
				{
					switch (i)
					{
					case 0:
						p->setyPos(p->getyPos() + 5);
						p->setxPos(p->getxPos() - 4);
						break;
					case 1:
						p->setyPos(p->getyPos() + 5);
						break;
					case 2:
						p->setyPos(p->getyPos() + 5);
						p->setxPos(p->getxPos() + 4);
						break;
					}
				}

			for (int i = 0; i < 8; i++)
				for (auto p = boss1_bullet4[i].begin(); p < boss1_bullet4[i].end(); ++p)
				{
					float dx = cos(RAD(radian[i]));
					float dy = sin(RAD(radian[i]));
					p->setxPos(p->getxPos() + 8 * dx);
					p->setyPos(p->getyPos() + 8 * dy);
				}
			break;

		case 4:		// 플레이어 총알 생성
			if (player.haveSub())
			{
				POINT tmp = player.getPos();
				tmp.x = player.getxPos() - 40;
				tmp.y = player.getyPos() + 25;
				player_bullet1.emplace_back(tmp, &player_bullet[1], -1);
				tmp.x = player.getxPos() + 40;
				player_bullet1.emplace_back(tmp, &player_bullet[1], -1);
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

		case 5:		// 적 비행기 이동
			for (auto p = enemy1.begin(); p < enemy1.end(); ++p)
				p->setyPos(p->getyPos() + 3);
			for (auto p = enemy2.begin(); p < enemy2.end(); ++p)
				if (p->getyPos() < 100) p->setyPos(p->getyPos() + 5);
			for (auto p = enemy3.begin(); p < enemy3.end(); ++p)
				p->setyPos(p->getyPos() + 1);
			if (Boss1_Appear == true && boss1.getyPos() < -70)
				boss1.setyPos(boss1.getyPos() + 5);
			else if (Boss1_Appear == true && boss1.getyPos() >= -70)
				Boss1_Stop = true;
			break;

		case 6:		// 적 비행기1 생성
			enemy1.emplace_back(&enemy_base[1], 1);
			for (auto p = enemy1.begin(); p < enemy1.end();)
			{
				if (p->getyPos() > 800)
					p = enemy1.erase(p);
				else
					++p;
			}
			break;

		case 7:		// 적 비행기2 생성
			enemy2.emplace_back(&enemy_base[2], 2);
			break;

		case 8:		// 적 총알1 생성
			for (auto p = enemy1.begin(); p < enemy1.end(); ++p)
				enemy_bullet1.emplace_back(p->getPos(), &enemy_bullet[1], 1);

			for (auto p = enemy_bullet1.begin(); p < enemy_bullet1.end();)
			{
				if (p->getyPos() > 800)
					p = enemy_bullet1.erase(p);
				else
					++p;
			}
			break;

		case 9: // 적 총알2 생성
			for (int i = 0; i < 3; i++)
			{
				for (auto p = enemy2.begin(); p < enemy2.end(); ++p)
				{
					enemy_bullet2[i].emplace_back(p->getPos(), &enemy_bullet[2], 2);
				}

				for (auto p = enemy_bullet2[i].begin(); p < enemy_bullet2[i].end();)
				{
					if (p->getyPos() > 800)
						p = enemy_bullet2[i].erase(p);
					else
						++p;
				}
			}
			break;

		case 10:	// 총알 충돌체크
			Bullet_Check1(player_bullet1, enemy1);
			Bullet_Check1(player_bullet1, enemy2);
			Bullet_Check1(player_bullet1, enemy3);
			for (auto p = player_bullet1.begin(); p < player_bullet1.end();)
			{
				if (p->getyPos() < 250 && p->getxPos() > 270 && p->getxPos() < 350
					&& Boss1_Stop == true)
				{
					boss1.setHp(boss1.getHp() - 1);
					p = player_bullet1.erase(p);
					if (boss1.getHp() <= 0) PostQuitMessage(0);
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
				Bullet_Check2(hwnd, boss1_bullet1[i], player);
				Bullet_Check2(hwnd, boss1_bullet2[i], player);
			}
			break;

		case 11:	// 적 비행기3 생성
			enemy3.emplace_back(&enemy_base[3], 3);
			for (auto p = enemy3.begin(); p < enemy3.end();)
			{
				if (p->getyPos() > 800)
					p = enemy3.erase(p);
				else
					++p;
			}
			break;

		case 12:	// 적 총알3 생성
			for (int i = 0; i < 8; i++)
			{
				for (auto p = enemy3.begin(); p < enemy3.end(); ++p)
					enemy_bullet3[i].emplace_back(p->getPos(), &enemy_bullet[3], 3);

				for (auto p = enemy_bullet3[i].begin(); p < enemy_bullet3[i].end();)
				{
					if (p->getyPos() > 800 || p->getyPos() < 0
						|| p->getxPos() > 800 || p->getxPos() < 0)
						p = enemy_bullet3[i].erase(p);
					else
						++p;
				}
			}
			break;

		case 13:	// 보스1 생성
			Boss1_Appear = true;
			Warning_On = true;
			KillTimer(hwnd, 6);
			KillTimer(hwnd, 7);
			KillTimer(hwnd, 8);
			KillTimer(hwnd, 9);
			KillTimer(hwnd, 11);
			KillTimer(hwnd, 12);
			KillTimer(hwnd, 13);
			break;

		case 14:	// 보스1 애니메이션
			if (boss1.getImg() < 7)
				boss1.setImg(boss1.getImg() + 1);
			else
				boss1.setImg(1);
			break;

		case 15:	// 보스 총알1 생성
			for (int i = 0; i < 36; i++)
			{
				if (Boss1_Stop == true && boss1.getHp() > 600 && boss1.getHp() <= 900)
				{
					boss1_bullet1[i].emplace_back(boss1.getPos(), &boss_bullet[1], 4);
				}

				for (auto p = boss1_bullet1[i].begin(); p < boss1_bullet1[i].end();)
				{
					if (p->getyPos() > 800 || p->getyPos() < 0
						|| p->getxPos() > 800 || p->getxPos() < 0)
						p = boss1_bullet1[i].erase(p);
					else
						++p;
				}
			}
			break;

		case 16:	// 아이템 생성
			ItemTimeCount++;
			if (ItemTimeCount % 5000 == 0)
				I_power.emplace_back(&Item[2]);
			if (ItemTimeCount % 3000 == 0)
				I_skill.emplace_back(&Item[3]);
			if (ItemTimeCount % 2000 == 0)
				I_bullet.emplace_back(&Item[0]);
			if (ItemTimeCount % 3000 == 0)
				I_sub.emplace_back(&Item[1]);
			if (ItemTimeCount % 10000 == 0)
				I_sheild.emplace_back(&Item[4]);

			ItemMove();
			ItemGet();

			if (Skill_On)
			{
				player.setSkillyPos(player.getSkillyPos() - 10);
				if (player.getSkillyPos() < -300)
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
			break;

		case 17:	// 보스 총알2 생성
			Boss1_BulletCount1++;
			if (Boss1_BulletCount1 == 35) Boss1_BulletCount1 = 0;
			for (int i = Boss1_BulletCount1; i < Boss1_BulletCount1 + 1; i++)
				if (Boss1_Stop == true && boss1.getHp() > 300 && boss1.getHp() <= 600)
				{
					boss1_bullet2[i].emplace_back(boss1.getPos(), &boss_bullet[1], 4);
				}
			for (int i = 0; i < 36; i++)
				for (auto p = boss1_bullet2[i].begin(); p < boss1_bullet2[i].end();)
				{
					if (p->getyPos() > 800 || p->getyPos() < 0
						|| p->getxPos() > 800 || p->getxPos() < 0)
						p = boss1_bullet2[i].erase(p);
					else
						++p;
				}
			break;

		case 18: // 보스 총알3 생성
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

		case 19:	// 보스 총알3 - 1 생성
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 3; j++)
				{
					if (j != 1 && Boss1_Stop == true && boss1.getHp() > 0 && boss1.getHp() <= 300)
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

		case 20:	// 보스 경고문 이동1
			if (Warning_On == true && wX < 0) wX += 160;
			if (Warning_On == true && wX == 0) {
				SetTimer(hwnd, 21, 2000, NULL); // 보스 경고문 이동2
				wX++;
			}
			if (Warning_On == true && Warning_Next == true) wX += 160;
			if (wX > 1000)
			{
				Warning_On = false;
				KillTimer(hwnd, 20);
				KillTimer(hwnd, 21);
			}
			break;

		case 21:	// 보스 경고문 이동2
			Warning_Next = true;
			break;
		}
		InvalidateRect(hwnd, NULL, false);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		membit = CreateCompatibleBitmap(hdc, WndX, WndY);
		SelectObject(memdc, membit);

		scene.draw(memdc, MapY1, MapY2, WndX, WndY);
		if (Warning_On == true) warning.Draw(memdc, wX, wY, WndX, 300);
		if (scene.Isplay()) {
			DrawEnemy(memdc);
			DrawBullet(memdc);
			if (Skill_On)
				player.usingBomb(memdc);
			DrawItem(memdc);
			player.Draw(memdc);
		}

		BitBlt(hdc, 0, 0, WndX, WndY, memdc, 0, 0, SRCCOPY);

		DeleteObject(membit);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		KillTimer(hwnd, 3);
		KillTimer(hwnd, 4);
		KillTimer(hwnd, 5);
		KillTimer(hwnd, 6);
		KillTimer(hwnd, 7);
		KillTimer(hwnd, 8);
		KillTimer(hwnd, 9);
		KillTimer(hwnd, 10);
		KillTimer(hwnd, 11);
		KillTimer(hwnd, 12);
		KillTimer(hwnd, 13);
		KillTimer(hwnd, 14);
		KillTimer(hwnd, 15);
		KillTimer(hwnd, 16);
		KillTimer(hwnd, 17);
		KillTimer(hwnd, 18);
		KillTimer(hwnd, 19);
		KillTimer(hwnd, 20);
		KillTimer(hwnd, 21);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}

void ImgLoad()
{
	player_bullet[1].Load(TEXT("총알기본.png"));
	player_bullet[2].Load(TEXT("강화총알.png"));
	enemy_bullet[1].Load(TEXT("적총알기본.png"));
	enemy_bullet[2].Load(TEXT("적총알2.png"));
	enemy_bullet[3].Load(TEXT("적총알2.png"));
	boss_bullet[1].Load(TEXT("적총알3.png"));
	boss_bullet[2].Load(TEXT("적총알4.png"));

	enemy_base[1].Load(TEXT("적비행기1.png"));
	enemy_base[2].Load(TEXT("적비행기2.png"));
	enemy_base[3].Load(TEXT("적비행기3.png"));

	Item[0].Load(TEXT("ITEM(총알).png"));
	Item[1].Load(TEXT("ITEM(보조).png"));
	Item[2].Load(TEXT("ITEM(파워).png"));
	Item[3].Load(TEXT("ITEM(필살기).png"));
	Item[4].Load(TEXT("ITEM(방어막).png"));

	warning.Load(TEXT("보스경고.png"));
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

				p = Bullet.erase(p);
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
			p = Target.erase(p);
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
			P.setHp(P.getHp() - 1);
			P.setBullet(1);
			P.setSub(false);
			P.setPowerUp(false);
			p = Bullet.erase(p);
		}
		else
			++p;
	}
}

void DrawEnemy(HDC hdc)
{
	for (auto p = enemy1.begin(); p < enemy1.end(); ++p)
		p->Draw(hdc);
	for (auto p = enemy2.begin(); p < enemy2.end(); ++p)
		p->Draw(hdc);
	for (auto p = enemy3.begin(); p < enemy3.end(); ++p)
		p->Draw(hdc);
	if (Boss1_Appear == true)
		boss1.Draw(hdc);
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
	for (int i = 0; i < 36; i++)
		for (auto p = boss1_bullet1[i].begin(); p < boss1_bullet1[i].end(); ++p)
			p->Draw(hdc);
	for (int i = 0; i < 36; i++)
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
			p = I_power.erase(p);
		}
		else
			++p;
	}

	for (auto p = I_skill.begin(); p < I_skill.end();)
	{
		if (p->IsGet(player))
		{
			p = I_skill.erase(p);
			if (player.getBomb() < 3)
				player.setBomb(player.getBomb() + 1);
		}
		else
			++p;
	}

	for (auto p = I_bullet.begin(); p < I_bullet.end();)
	{
		if (p->IsGet(player))
		{
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
			p = enemy1.erase(p);
		else
			++p;
	}

	for (auto p = enemy2.begin(); p < enemy2.end();)
	{
		if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			p = enemy2.erase(p);
		else
			++p;
	}

	for (auto p = enemy3.begin(); p < enemy3.end();)
	{
		if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			p = enemy3.erase(p);
		else
			++p;
	}

	for (auto p = enemy_bullet1.begin(); p < enemy_bullet1.end();)
	{
		if (player.getSkillyPos() <= p->getyPos() + p->getSize())
			p = enemy_bullet1.erase(p);
		else
			++p;
	}

	for (int i = 0; i < 3; ++i)
	{
		for (auto p = enemy_bullet2[i].begin(); p < enemy_bullet2[i].end();)
		{
			if (player.getSkillyPos() <= p->getyPos() + p->getSize())
				p = enemy_bullet2[i].erase(p);
			else
				++p;
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		for (auto p = enemy_bullet3[i].begin(); p < enemy_bullet3[i].end();)
		{
			if (player.getSkillyPos() <= p->getyPos() + p->getSize())
				p = enemy_bullet3[i].erase(p);
			else
				++p;
		}
	}
}