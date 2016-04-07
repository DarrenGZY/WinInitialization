#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
class D3DManager
{
public:
	D3DManager();
	void Init(HWND hwnd);
	~D3DManager();

private:
	ID3D11Device *g_device;
	ID3D11DeviceContext *g_devcontext;
	IDXGISwapChain1 *g_swapchain;
	IDXGIFactory2 *g_factory;
};

