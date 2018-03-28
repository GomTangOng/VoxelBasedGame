#include "stdafx.h"
#include "CGameFramework.h"


CGameFramework::CGameFramework()
{
	m_pdxgiFactory = NULL;
	m_pdxgiSwapChain = NULL;
	m_pd3Device = NULL;

	m_pd3dCommandAllocator = NULL;
	m_pd3dCommandQueue = NULL;
	//m_pd3dPipelineState = NULL;
	m_pd3dCommandList = NULL;

	for(int i=0;i<m_nSwapChainBuffers;i++) m_ppdRenderTargetBuffer[i] = NULL;
	m_pd3dRtvDescriptorHeap = NULL;
	//m_nRtvDescriptorIncrementSize = 0;

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDsvDescriptorHeap = NULL;
	m_nDsvDescriptorIncrementSize = 0;

	m_nSwapChainBufferIndex = 0;

	m_hFenceEvent = NULL;
	m_pd3dFence = NULL;
	m_nFenceValue = 0;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;
}


CGameFramework::~CGameFramework()
{
}

bool CGameFramework::onCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	//Direct3D 디바이스 명령 큐와 리스트, 스왑 체인등을 생성하는 함수를 호출한다.

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateSwapChain();
	CreateRtvAndDsvDescriptorHeaps();
	CreateRenderTargetView();
	CreateDepthStencilView();

	BuildObjects();
	// 렌더링할 객체 (게임월드객체)를 생성한다.

	return (true);

}

void CGameFramework::OnDestroy()
{
	RealeaseObjects();

	::CloseHandle(m_hFenceEvent);
#if defined(_DEBUG)
	if (m_pd3dDebugController) m_pd3dDebugController->Release();
#endif

	for (int i = 0; i < m_nSwapChainBuffers; i++)
	{
		if (m_ppdRenderTargetBuffer[i]) m_ppdRenderTargetBuffer[i]->Release();
	}

	if (m_pd3dDepthStencilBuffer) m_pd3dRtvDescriptorHeap->Release();
	if (m_pd3dDsvDescriptorHeap) m_pd3dDsvDescriptorHeap->Release();

	if (m_pd3dCommandAllocator) m_pd3dCommandAllocator->Release();
	if (m_pd3dCommandQueue) m_pd3dCommandQueue->Release();
	if (m_pd3dCommandList) m_pd3dCommandList->Release();

	if (m_pd3dFence) m_pd3dFence->Release();

	m_pdxgiSwapChain->SetFullscreenState(FALSE, NULL);

	
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3Device) m_pd3Device->Release();
	if (m_pdxgiFactory) m_pdxgiFactory->Release();

}
