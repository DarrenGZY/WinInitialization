#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
#include "CommonTypes.h"

class D3DManager
{
public:
	D3DManager();
	void Init(HWND hwnd);
	~D3DManager();

private:
	void SafeRelease(IUnknown *object);
	ID3D11Device *g_device;						// Device
	ID3D11DeviceContext *g_devcontext;			// Device Context
	IDXGISwapChain1 *g_swapchain;				// SwapChain
	IDXGIFactory2 *g_factory;					// Factory
	ID3D11RenderTargetView* g_RTV;				// Render Target View
	ID3D11DepthStencilView* g_zbuffer;			// Depth Buffer
	ID3D11Buffer *g_cbuffer;					// Constant Buffer
	ID3D11SamplerState *g_Sampler;				// Sampler for Pixel Shader
};

