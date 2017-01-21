//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "GameState.h"
#include "Camera.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game {
public:

	Game();

	// Initialization and management
	void Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation);

	// Basic game loop
	void Tick();
	void Render();

	// Rendering helpers
	void Clear();
	void Present();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation);
	void ValidateDevice();

	// Properties
	void GetDefaultSize(int& width, int& height) const;

private:

	void Update(DX::StepTimer const& timer);

	void CreateDevice();
	void CreateResources();

	void OnDeviceLost();

	// Device resources.
	IUnknown*											m_window;
	int													m_outputWidth;
	int													m_outputHeight;
	DXGI_MODE_ROTATION									m_outputRotation;

	D3D_FEATURE_LEVEL									m_featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device3>				m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext3>		m_d3dContext;

	Microsoft::WRL::ComPtr<IDXGISwapChain3>				m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_depthStencilView;
	std::unique_ptr<DirectX::Keyboard>					m_keyboard;
	std::unique_ptr<DirectX::Mouse>						m_mouse;
	GameState											m_gamestate;
	std::unique_ptr<DirectX::GeometricPrimitive>		m_hitbox_shape;
	std::unique_ptr<DirectX::GeometricPrimitive>		m_bullet_shape;
	Camera												m_camera;

	// Rendering loop timer.
	DX::StepTimer										m_timer;
};