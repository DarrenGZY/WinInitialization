#include "stdafx.h"
#include "D3DManager.h"


D3DManager::D3DManager()
{
	g_device = nullptr;
	g_devcontext = nullptr;
	g_swapchain = nullptr;
	g_factory = nullptr;
}


D3DManager::~D3DManager()
{
	if (g_device)
	{
		g_device->Release();
		g_device = nullptr;
	}

	if (g_devcontext)
	{
		g_devcontext->Release();
		g_devcontext = nullptr;
	}

	if (g_swapchain)
	{
		g_swapchain->Release();
		g_swapchain = nullptr;
	}

	if (g_factory)
	{
		g_factory->Release();
		g_factory = nullptr;
	}
}

void D3DManager::Init(HWND hwnd)
{
	HRESULT hr;

	hr = D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&g_device,
		nullptr,
		&g_devcontext);

	if (FAILED(hr))
	{
		// handle error
		return ;
	}

	// Get DXGI factory
	IDXGIDevice* DxgiDevice = nullptr;
	hr = g_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DxgiDevice));
	if (FAILED(hr))
	{
		return;
	}

	IDXGIAdapter* DxgiAdapter = nullptr;
	hr = DxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&DxgiAdapter));
	DxgiDevice->Release();
	DxgiDevice = nullptr;
	if (FAILED(hr))
	{
		return;
	}

	hr = DxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&g_factory));
	DxgiAdapter->Release();
	DxgiAdapter = nullptr;
	if (FAILED(hr))
	{
		return;
	}

	// Get Window Size
	RECT WindowRect;
	GetClientRect(hwnd, &WindowRect);
	UINT Width = WindowRect.right - WindowRect.left;
	UINT Height = WindowRect.bottom - WindowRect.top;

	//Create swapchain
	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc;
	RtlZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	SwapChainDesc.BufferCount = 2;
	SwapChainDesc.Width = Width;
	SwapChainDesc.Height = Height;
	SwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;

	hr = g_factory->CreateSwapChainForHwnd(g_device, hwnd, &SwapChainDesc, nullptr, nullptr, &g_swapchain);
	if (FAILED(hr))
	{
		return;
	}
	
	// Create render target view

}
