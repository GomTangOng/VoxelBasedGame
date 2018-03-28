// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#pragma comment(lib, "d3dcompiler.lib") 
#pragma comment(lib, "d3d12.lib") 
#pragma comment(lib, "dxgi.lib")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define FRAME_BUFFER_WIDTH 640 
#define FRAME_BUFFER_HEIGHT 480
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string> 
#include <wrl.h> 
#include <shellapi.h>
#include <d3d12.h> 
#include <dxgi1_4.h> 
#include <D3Dcompiler.h>
#include <DirectXMath.h> 
#include <DirectXPackedVector.h> 
#include <DirectXColors.h> 
#include <DirectXCollision.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using Microsoft::WRL::ComPtr;


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
