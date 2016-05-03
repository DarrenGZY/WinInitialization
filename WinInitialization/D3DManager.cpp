#include "stdafx.h"
#include "D3DManager.h"


D3DManager::D3DManager()
{
	g_device = nullptr;
	g_devcontext = nullptr;
	g_swapchain = nullptr;
	g_factory = nullptr;
	g_RTV = nullptr;
	g_zbuffer = nullptr;
	g_cbuffer = nullptr;
	g_Sampler = nullptr;
}


D3DManager::~D3DManager()
{
	SafeRelease(g_device);
	SafeRelease(g_devcontext);
	SafeRelease(g_swapchain);
	SafeRelease(g_factory);
	SafeRelease(g_RTV);
	SafeRelease(g_zbuffer);
	SafeRelease(g_cbuffer);
	SafeRelease(g_Sampler);
}

void D3DManager::Init(HWND hwnd)
{
	HRESULT hr;

	//////////////////////////////////////
	// Create Device and Device Context
	//////////////////////////////////////
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

	//////////////////////////////////////
	// Create SwapChain
	//////////////////////////////////////
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

	///////////////////////////////////////////////////////
	//Create Render Target View
	///////////////////////////////////////////////////////

	// Get backbuffer
	ID3D11Texture2D* backbuffer = nullptr;
	hr = g_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer));
	if (FAILED(hr))
	{
		return;
	}

	hr = g_device->CreateRenderTargetView(backbuffer, nullptr, &g_RTV);
	
	SafeRelease(backbuffer);
	
	if (FAILED(hr))
	{
		return;
	}

	///////////////////////////////////////////////////////
	//Create Depth Stencil View
	///////////////////////////////////////////////////////

	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));

	texd.Width = Width;
	texd.Height = Height;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 1;
	texd.Format = DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D *pDepthBuffer = nullptr;
	hr = g_device->CreateTexture2D(&texd, NULL, &pDepthBuffer);
	if (FAILED(hr))
	{
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	hr = g_device->CreateDepthStencilView(pDepthBuffer, &dsvd, &g_zbuffer);
	if (FAILED(hr))
	{
		return;
	}

	SafeRelease(pDepthBuffer);

	///////////////////////////////////////////////////////
	//Create Constant Buffer
	///////////////////////////////////////////////////////
	D3D11_BUFFER_DESC cbufferdesc;
	ZeroMemory(&cbufferdesc, sizeof(cbufferdesc));

	cbufferdesc.Usage = D3D11_USAGE_DEFAULT;
	cbufferdesc.ByteWidth = sizeof(CBUFFER);
	cbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = g_device->CreateBuffer(&cbufferdesc, NULL, &g_cbuffer);
	if (FAILED(hr))
	{
		return;
	}

	///////////////////////////////////////////////////////
	//Create Sampler
	///////////////////////////////////////////////////////
	D3D11_SAMPLER_DESC SampDesc;
	ZeroMemory(&SampDesc, sizeof(SampDesc));
	SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SampDesc.MinLOD = 0;
	SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_device->CreateSamplerState(&SampDesc, &g_Sampler);
	if (FAILED(hr))
	{
		return;
	}
}

void D3DManager::SafeRelease(IUnknown *object)
{
	if (object)
	{
		object->Release();
		object = nullptr;
	}
}
