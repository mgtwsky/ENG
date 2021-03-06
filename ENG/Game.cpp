//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

static const XMVECTORF32 START_POSITION = { 0.f, 1.5f, 0.f, 0.f };
static const float ROTATION_GAIN = 0.004f;
static const float MOVEMENT_GAIN = 10.f;

Game::Game() :
	m_window(0),
	m_outputWidth(800),
	m_outputHeight(600),
	m_outputRotation(DXGI_MODE_ROTATION_IDENTITY),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1) {
	m_gamestate.player.position = START_POSITION.v;
	m_camera.camera_pos = m_gamestate.player.position;
	m_camera.pitch = 0;
	m_camera.yaw = 0;
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation) {
	m_window = window;
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);
	m_outputRotation = rotation;
	CreateDevice();
	CreateResources();
	m_timer.SetFixedTimeStep(false);
	m_keyboard = std::make_unique<Keyboard>();
	m_keyboard->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));
}

// Executes the basic game loop.
void Game::Tick() {
	m_timer.Tick([&]() {
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer) {
	float elapsedTime = float(timer.GetElapsedSeconds());
	auto kb = m_keyboard->GetState();
	if (kb.Escape)
		Windows::ApplicationModel::Core::CoreApplication::Exit();
	auto mouse = m_mouse->GetState();
	if (mouse.positionMode == Mouse::MODE_RELATIVE) {
		Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0.f)
			* ROTATION_GAIN;
		m_camera.pitch -= delta.y;
		m_camera.yaw -= delta.x;
		float limit = XM_PI / 2.0f - 0.01f;
		m_camera.pitch = std::max(-limit, m_camera.pitch);
		m_camera.pitch = std::min(+limit, m_camera.pitch);
		if (m_camera.yaw > XM_PI) { m_camera.yaw -= XM_PI * 2.0f; } else if (m_camera.yaw < -XM_PI) { m_camera.yaw += XM_PI * 2.0f; }
	}
	m_gamestate.player.look_direction = m_camera.CreateLookDirectionVec();
	m_mouse->SetMode(Mouse::MODE_RELATIVE);
	if (kb.Home) {
		m_gamestate.player.position = START_POSITION.v;
		m_gamestate.player.hitbox.SetPosition(START_POSITION.v);
		m_camera.pitch = m_camera.yaw = 0;
	}
	Vector3 move = Vector3::Zero;
	if (kb.Left || kb.A)		move.x += 1.f;
	if (kb.Right || kb.D)		move.x -= 1.f;
	if (kb.Up || kb.W)	        move.z += 1.f;
	if (kb.Down || kb.S)		move.z -= 1.f;
	if (kb.LeftShift)           move.y += 1.f;
	if (kb.LeftControl)         move.y -= 1.f;
	if (kb.OemOpenBrackets)     m_gamestate.DecreaseBulletSize(0.1f);
	if (kb.OemCloseBrackets)    m_gamestate.IncreaseBulletSize(0.1f);
	if (kb.OemSemicolon)        m_gamestate.DecreaseBulletSpeed(0.5f);
	if (kb.OemQuotes)           m_gamestate.IncreaseBulletSpeed(0.5f);
	if (kb.K)                   m_gamestate.AddBot();
	if (kb.L)                   m_gamestate.RemoveBot();
	if (kb.OemPeriod)           m_gamestate.logger.Start();
	if (kb.OemQuestion)         m_gamestate.logger.Stop();
	if (kb.U) m_gamestate.creation_bullet_type = BallisticsType::SIMPLE;
	if (kb.I) m_gamestate.creation_bullet_type = BallisticsType::NORMAL;
	if (kb.O) m_gamestate.creation_bullet_type = BallisticsType::ADVANCED;
	if (kb.P) m_gamestate.creation_bullet_type = BallisticsType::REALISTIC;
	Quaternion q = Quaternion::CreateFromYawPitchRoll(m_camera.yaw, -m_camera.pitch, 0.f);
	move = Vector3::Transform(move, q) * MOVEMENT_GAIN * elapsedTime;
	m_gamestate.player.position += move;
	m_gamestate.player.hitbox.SetPosition(m_gamestate.player.position);
	m_camera.camera_pos = m_gamestate.player.position;
	if (mouse.leftButton) m_gamestate.CreateBullet(m_gamestate.player.position + m_gamestate.player.look_direction, m_gamestate.player.look_direction, m_gamestate.creation_bullet_type);
#pragma region Updating Bullets
	m_gamestate.DestroyDeadBullets();
	m_gamestate.UpdateBullets(elapsedTime);
#pragma endregion Logic of bullets
	if (m_gamestate.IsPlayerInHeavyLoadRoom() && m_gamestate.bots.empty())
		m_gamestate.CreateHeavyLoadBots();
	if (m_gamestate.IsPlayerInMultiplePPLRoom() && m_gamestate.bots.empty())
		m_gamestate.CreateMultiplePPLBots();
	if (!m_gamestate.IsPlayerInHeavyLoadRoom() && !m_gamestate.IsPlayerInMultiplePPLRoom() && !m_gamestate.bots.empty())
		m_gamestate.ClearBots();
	m_gamestate.UpdateBots(elapsedTime);
	m_gamestate.logger.Update(elapsedTime);
}

// Draws the scene.
void Game::Render() {
	if (m_timer.GetFrameCount() == 0) {
		return;
	}

	Clear();

	XMVECTOR lookAt = m_camera.camera_pos + m_gamestate.player.look_direction;

	XMMATRIX view = XMMatrixLookAtRH(m_camera.camera_pos, lookAt, Vector3::Up);

	for (auto const & wall : m_gamestate.walls) {
		wall.Render(view, m_camera.proj);
	}
	for (auto const & bullet : m_gamestate.bullets) {
		bullet.Render(view, m_camera.proj, m_bullet_shape.get(), m_hitbox_shape.get());
	}

	for (auto const & wind : m_gamestate.winds) {
		wind.Render(view, m_camera.proj, m_hitbox_shape.get());
	}

	m_gamestate.DrawRoomHitboxes(view, m_camera.proj, m_hitbox_shape.get());
	m_gamestate.DrawBots(view, m_camera.proj, m_hitbox_shape.get());
	m_gamestate.DrawInfo(m_spriteBatch.get(), m_font.get(), m_timer);

	Present();
}

// Helper method to clear the back buffers.
void Game::Clear() {
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
	m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present() {
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// Discard the contents of the render target.
	// This is a valid operation only when the existing contents will be entirely
	// overwritten. If dirty or scroll rects are used, this call should be removed.
	m_d3dContext->DiscardView(m_renderTargetView.Get());

	// Discard the contents of the depth stencil.
	m_d3dContext->DiscardView(m_depthStencilView.Get());

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
		OnDeviceLost();
	} else {
		DX::ThrowIfFailed(hr);
	}
}

// Message handlers
void Game::OnActivated() {}

void Game::OnDeactivated() {}

void Game::OnSuspending() {
	m_d3dContext->ClearState();

	ComPtr<IDXGIDevice3> dxgiDevice;
	if (SUCCEEDED(m_d3dDevice.As(&dxgiDevice))) {
		dxgiDevice->Trim();
	}
}

void Game::OnResuming() {
	m_timer.ResetElapsedTime();
}

void Game::OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation) {
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);
	m_outputRotation = rotation;

	CreateResources();
}

void Game::ValidateDevice() {
	// The D3D Device is no longer valid if the default adapter changed since the device
	// was created or if the device has been removed.

	DXGI_ADAPTER_DESC previousDesc;
	{
		ComPtr<IDXGIDevice3> dxgiDevice;
		DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

		ComPtr<IDXGIAdapter> deviceAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(deviceAdapter.GetAddressOf()));

		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(deviceAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		ComPtr<IDXGIAdapter1> previousDefaultAdapter;
		DX::ThrowIfFailed(dxgiFactory->EnumAdapters1(0, previousDefaultAdapter.GetAddressOf()));

		DX::ThrowIfFailed(previousDefaultAdapter->GetDesc(&previousDesc));
	}

	DXGI_ADAPTER_DESC currentDesc;
	{
		ComPtr<IDXGIFactory2> currentFactory;
		DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(currentFactory.GetAddressOf())));

		ComPtr<IDXGIAdapter1> currentDefaultAdapter;
		DX::ThrowIfFailed(currentFactory->EnumAdapters1(0, currentDefaultAdapter.GetAddressOf()));

		DX::ThrowIfFailed(currentDefaultAdapter->GetDesc(&currentDesc));
	}

	// If the adapter LUIDs don't match, or if the device reports that it has been removed,
	// a new D3D device must be created.

	if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart
		|| previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart
		|| FAILED(m_d3dDevice->GetDeviceRemovedReason())) {
		// Create a new device and swap chain.
		OnDeviceLost();
	}
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const {
	width = 1024;
	height = 768;
}

// These are the resources that depend on the device.
void Game::CreateDevice() {
	// This flag adds support for surfaces with a different color channel ordering than the API default.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		// TODO: Modify for supported Direct3D feature levels.
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Create the DX11 API device object, and get a corresponding context.
	m_d3dDevice.Reset();
	m_d3dContext.Reset();

	ComPtr<ID3D11Device> d3dDevice;
	ComPtr<ID3D11DeviceContext> d3dContext;
	DX::ThrowIfFailed(D3D11CreateDevice(
		nullptr,                    // specify nullptr to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		d3dDevice.GetAddressOf(),   // returns the Direct3D device created
		&m_featureLevel,            // returns feature level of device created
		d3dContext.GetAddressOf()   // returns the device immediate context
	));

	DX::ThrowIfFailed(d3dDevice.As(&m_d3dDevice));
	DX::ThrowIfFailed(d3dContext.As(&m_d3dContext));

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(m_d3dDevice.As(&d3dDebug))) {
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue))) {
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

#ifdef _DEBUG
	{
		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf())))) {
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		}
	}
#endif

	// TODO: Initialize device dependent objects here (independent of window size).
	Wall::CreateWalls(m_d3dContext.Get(), m_gamestate.walls);
	m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dContext.Get());
	m_font = std::make_unique<SpriteFont>(m_d3dDevice.Get(), L"font.spritefont");
	m_hitbox_shape = GeometricPrimitive::CreateBox(m_d3dContext.Get(), { 1.f, 1.f, 1.f });
	m_bullet_shape = GeometricPrimitive::CreateSphere(m_d3dContext.Get(), 1.f);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources() {
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_d3dContext->Flush();

	UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
	UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backBufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one.
	if (m_swapChain) {
		HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		} else {
			DX::ThrowIfFailed(hr);
		}
	} else {
		// First, retrieve the underlying DXGI Device from the D3D Device
		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = backBufferCount;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		ComPtr<IDXGISwapChain1> swapChain;
		DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForCoreWindow(
			m_d3dDevice.Get(),
			m_window,
			&swapChainDesc,
			nullptr,
			swapChain.GetAddressOf()
		));

		DX::ThrowIfFailed(swapChain.As(&m_swapChain));

		DX::ThrowIfFailed(dxgiDevice->SetMaximumFrameLatency(1));
	}

	DX::ThrowIfFailed(m_swapChain->SetRotation(m_outputRotation));

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

	// TODO: Initialize windows-size dependent objects here.
	m_camera.proj = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(65.f),
														 float(backBufferWidth) / float(backBufferHeight), 0.01f, 100.f);
}

void Game::OnDeviceLost() {
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_d3dContext.Reset();
	m_d3dDevice.Reset();
	m_font.reset();
	m_spriteBatch.reset();

	CreateDevice();

	CreateResources();
}