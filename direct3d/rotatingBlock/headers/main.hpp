#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <Windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3dx9math.h>
#include <D3Dcompiler.h>
#include <iostream>
#include <xnamath.h> 
#include <math.h>
#include "graphicsStructures.hpp"

IDXGISwapChain *gSwapChain; 
ID3D11Device *gDevice;
ID3D11DeviceContext *gDeviceContext; //mostly manages GPU and rendering pipeline
ID3D11RenderTargetView *gRenderTargetView;
ID3D11VertexShader *gVS;
ID3D11PixelShader *gPS;
ID3D11Buffer *gVBuffer;
ID3D11Buffer *gIBuffer;	
ID3D11Buffer *gConstantBuffer;
ID3D11InputLayout *gLayout;
XMMATRIX gRotationMatrix;
XMMATRIX gViewMatrix;
XMMATRIX gProjectionMatrix;
XMMATRIX gTranslationMatrix;

SLONG gNumTriangles;
SFLOAT gAngle = 0.0f;
SFLOAT gYAngle = 0.0f;
SFLOAT gZAngle = 0.0f;

void InitViewport( SFLOAT width, SFLOAT height );
void InitRasterizer();
void InitGraphics( SFLOAT width, SFLOAT height );
void InitPipeline();
void UpdateConstantBuffer();

#endif