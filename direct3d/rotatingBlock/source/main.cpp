#include "main.hpp"

LRESULT CALLBACK WindowProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	){

		switch( message ){

			case WM_DESTROY:
				PostQuitMessage( 0 );
				return 0;
				break;
			case WM_KEYDOWN:
			{
				switch( wParam ){
					case( VK_DOWN ):
						gAngle -= 0.20f;
						UpdateConstantBuffer();
						break;
					case( VK_UP ):
						gAngle += 0.20f;
						UpdateConstantBuffer();
						break;
					case( VK_RIGHT ):
						gYAngle -= 0.20f;
						UpdateConstantBuffer();
						break;
					case( VK_LEFT ):
						gYAngle += 0.20f;
						UpdateConstantBuffer();
						break;
				}
				gRotationMatrix = XMMatrixMultiply( 
							XMMatrixRotationX( gAngle ),
							XMMatrixRotationY( gYAngle )
						);
			}
		}

		return DefWindowProc( hWnd, message, wParam, lParam );
}


void InitWindows( HWND &hWnd, HINSTANCE hInstance, SINT nCmdShow, SLONG width, SLONG height ){
		
	    //registering windows class
		WNDCLASSEX windowsClass; // struct with windows class

		ZeroMemory( &windowsClass, sizeof( WNDCLASSEX ) );

		windowsClass.cbSize = sizeof( WNDCLASSEX );
		windowsClass.style = CS_HREDRAW | CS_VREDRAW; // redraw if height / width is changed
		windowsClass.lpfnWndProc = WindowProc; // pointer to windows procedure
		windowsClass.hInstance = hInstance;
		windowsClass.hCursor = LoadCursor( NULL, IDC_CROSS );
		windowsClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
		LPCSTR windowsClass1Name = "windowClass1";
		windowsClass.lpszClassName = windowsClass1Name;
		
		RegisterClassEx( &windowsClass );

		
		RECT windowsRect = { 0, 0, width, height };
		AdjustWindowRect( &windowsRect, WS_OVERLAPPEDWINDOW, false );

		//creating the window
		hWnd = CreateWindowEx( 
			NULL, //extention of more styles
			windowsClass1Name,
			"Spinning Thingie",
			WS_OVERLAPPEDWINDOW,
			300,
			300,
			windowsRect.right - windowsRect.left,
			windowsRect.bottom - windowsRect.top,
			NULL,
			NULL,
			hInstance,
			NULL // pointer to be passed to window when using createstruct
		);

}

void InitD3D( HWND hWnd, SFLOAT width, SFLOAT height ){

	//initializing swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

	swapChainDesc.BufferCount = 1; // number of back buffers
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //unsigned normalized values
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // draw graphics into back buffer
	swapChainDesc.OutputWindow = hWnd; // window to output to
	swapChainDesc.SampleDesc.Count = 4; // anti-aliasing that is done on the images
	swapChainDesc.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		//D3D_FEATURE_LEVEL_11_0
        D3D_FEATURE_LEVEL_10_1,
        //D3D_FEATURE_LEVEL_10_0,
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0, //flags for singlethreading/multithreading etc. stuff
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext
	);

	//setting the render target to the back buffer
	//get address of back buffer into renderTargetLocation
	ID3D11Texture2D *renderTargetAddress;
	gSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&renderTargetAddress );

	//using the back buffer address to create the render target
	gDevice->CreateRenderTargetView( renderTargetAddress, NULL, &gRenderTargetView );
	renderTargetAddress->Release();

	//set render target as the back buffer
	gDeviceContext->OMSetRenderTargets( 1, &gRenderTargetView, NULL );

	InitViewport( width, height );
	//InitRasterizer();
	InitGraphics( width, height );
	InitPipeline();

}

void InitViewport( SFLOAT width, SFLOAT height ){
	//setting the viewport, 
	//maps pixel coordinates to normalized coordinates
	D3D11_VIEWPORT viewport;
	ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	gDeviceContext->RSSetViewports( 1, &viewport );
}

void InitRasterizer(){
	//initializing the rasterizer stage
	ID3D11RasterizerState *rasterState;
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	gDevice->CreateRasterizerState( &rasterDesc, &rasterState );
	gDeviceContext->RSSetState( rasterState );
}

void InitGraphics( SFLOAT width, SFLOAT height ){

	//creating vertex buffer
	Vertex vertices[] = 
	{
		Vertex( 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f ),
		Vertex( 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f ),
		Vertex( -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f ),
		Vertex( -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f ),
		Vertex( 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f ),
		Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f ),
		Vertex( -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f ),
		Vertex( -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f )
	};

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof( vertices );
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	gDevice->CreateBuffer( &bufferDesc, NULL, &gVBuffer );

	D3D11_MAPPED_SUBRESOURCE mappedSR;
	gDeviceContext->Map( gVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSR );
	memcpy( mappedSR.pData, vertices, sizeof( vertices) );
	gDeviceContext->Unmap( gVBuffer, NULL );

	UINT stride = sizeof( Vertex );
	UINT offset = 0;

	gDeviceContext->IASetVertexBuffers( 0, 1, &gVBuffer, &stride, &offset );

	//indexBuffer
	
	SSHORT indices[] = {
		7, 4, 5,
		7, 5, 6,

		0, 3, 2,
		0, 2, 1,

		4, 0, 1,
		4, 1, 5,

		3, 7, 6,
		3, 6, 2,

		3, 0, 4,
		3, 4, 7,

		6, 5, 1,
		6, 1, 2

	};

	gNumTriangles = ( sizeof(indices)/sizeof(SSHORT) ) / 3;
	
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof( indices );
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	gDevice->CreateBuffer( &bufferDesc, NULL, &gIBuffer );

	gDeviceContext->Map( gIBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSR );
	memcpy( mappedSR.pData, indices, sizeof( indices ) );
	gDeviceContext->Unmap( gIBuffer, NULL );

	gDeviceContext->IASetIndexBuffer( gIBuffer , DXGI_FORMAT_R16_UINT, 0 );

	//create the constant buffer
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( ConstantBuffer );
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	gDevice->CreateBuffer( &bufferDesc, NULL, &gConstantBuffer );
	gDeviceContext->VSSetConstantBuffers( 0, 1, &gConstantBuffer );

	// initialize the world matrix
	gRotationMatrix = XMMatrixIdentity();
	gTranslationMatrix = XMMatrixIdentity();

	//initializing the view matrix
	XMVECTOR eye = XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f );
	XMVECTOR at = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
	XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	gViewMatrix = XMMatrixLookAtLH( eye, at, up );

	//initializing the projection matrixf
	gProjectionMatrix = XMMatrixPerspectiveFovLH(
			XM_PIDIV2,
			width / (FLOAT)height,
			0.01f, 
			100.0f
		);

	UpdateConstantBuffer();
}

void CleanD3D(){

	gSwapChain->Release();
	gDevice->Release();
	gDeviceContext->Release();
	gRenderTargetView->Release();
	gVBuffer->Release();
	gLayout->Release();
	gVS->Release();
	gPS->Release();

}


void InitShaders(){
	//1 load and compile vertex and pixel shaders from the hlsl file
	ID3D10Blob *VS, *PS;
	
	//vertex shader
	HRESULT hr = D3DX11CompileFromFile( 
		"../../source/shaders/vertexShader.hlsl", //name of file
		0,				//shader macros
		0,				//includes
		"VShader",		//main function name
		"vs_4_0",		//shader model
		0,				//compilation configurations
		0,				//more compilation stuff
		0,
		&VS,			//shader program location
		0,				
		0
	);

	//pixel shader
	hr = D3DX11CompileFromFile( 
		"../../source/shaders/PixelShader.hlsl",	0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0
	);

	//2 encapsulate both shaders into shader objects
	volatile HRESULT hr1 = gDevice->CreateVertexShader( VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &gVS );
	volatile HRESULT hr2 = gDevice->CreatePixelShader( PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &gPS );

	//3 set both shaders to be active shaders
	gDeviceContext->VSSetShader( gVS, 0, 0 );
	gDeviceContext->PSSetShader( gPS, 0, 0 );

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	gDevice->CreateInputLayout( inputElementDesc, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &gLayout );
	gDeviceContext->IASetInputLayout( gLayout );
}
void InitPipeline(){
	InitShaders();
	//initializing other things later
}

void UpdateConstantBuffer(){
	
	//Update variables
	ConstantBuffer cb;
	cb.rotationMatrix = XMMatrixTranspose( gRotationMatrix );
	cb.viewMatrix = XMMatrixTranspose( gViewMatrix );
	cb.projectionMatrix = XMMatrixTranspose( gProjectionMatrix );
	cb.translationMatrix = XMMatrixTranspose( gTranslationMatrix );

	gDeviceContext->UpdateSubresource( 
			gConstantBuffer,
			0,
			NULL,
			&cb,
			0,
			0
		);
}

void DrawSquare(){
	
	gDeviceContext->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	gDeviceContext->DrawIndexed( gNumTriangles * 3 , 0, 0 );
	Sleep( 50 );

}

void Render(){

	//clear the back buffer
	gDeviceContext->ClearRenderTargetView( gRenderTargetView, D3DXCOLOR( 0.0f, 0.2, 0.4f, 1.0f ) );
	DrawSquare();
	//switch the buffers
	gSwapChain->Present( 0, 0 );
}

SINT WINAPI WinMain( 
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance ,
		LPSTR lpCmdLine,
		SINT nCmdShow // how the window will be shown
	){
		SLONG width = 800;
		SLONG height = 600;

		HWND hWnd;

		InitWindows( hWnd, hInstance, nCmdShow, width, height );
		InitD3D( hWnd, (SFLOAT)width, (SFLOAT)height );

		//show the window
		ShowWindow( hWnd, nCmdShow );
		MSG msg;

		while( TRUE ){ //min and max are limitations on which messages we want to get from the queue

			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
				
				TranslateMessage( &msg );
				DispatchMessage( &msg ); // sends message to WindowProc

				if( msg.message == WM_QUIT ){
					break;
				}

			} else {
				Render();
			}
				
		}

		//cleaning up D3D
		CleanD3D();

		return msg.wParam;

}

