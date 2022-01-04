//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();


	m_Camera = std::make_unique<Camera>();

	m_Camera->Initialize(width, height, Vector3(2.f, 2.f, 2.f), 0.f, 0.f);

	CreateWindowSizeDependentResources();
	m_font = std::make_unique<DirectX::SpriteFont>(m_deviceResources->GetD3DDevice(), L"Content/Fonts/myfile.spritefont");

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.


	
    elapsedTime;
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

	DrawSky();
	DrawModel();
	DrawFPS();
    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

void Game::DrawFPS()
{
	m_SpriteBatch->Begin();
	float fps = m_timer.GetFramesPerSecond();
	wchar_t szState[64] = {};
	swprintf_s(szState, L"FPS: %f", fps);

	m_font->DrawString(m_SpriteBatch.get(), szState, m_FontPos, Colors::White);
	m_SpriteBatch->End();
	
}

void Game::DrawModel()
{
	Matrix View = m_Camera->GetViewMatrix();
	Matrix Projection = m_Camera->GetProjectionMatrix();
	m_effect->SetWorld(m_World);
	m_shape->Draw(m_effect.get(), m_inputLayout.Get());
}

void Game::DrawSky()
{
	m_skyEffect->SetView(m_Camera->GetViewMatrix());
	m_sky->Draw(m_skyEffect.get(), m_skyInputLayout.Get());
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::DarkSlateGray);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
	height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

	auto context = m_deviceResources->GetD3DDeviceContext();

	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	m_shape = GeometricPrimitive::CreateTeapot(context);

	m_World = Matrix::Identity;

	m_effect = std::make_unique<DirectX::BasicEffect>(device);
	m_effect->SetTextureEnabled(true);
	m_effect->SetPerPixelLighting(true);
	m_effect->SetLightingEnabled(true);
	m_effect->SetLightEnabled(0, true);
	m_effect->SetLightDiffuseColor(0, Colors::White);
	m_effect->SetLightDirection(0, -Vector3::UnitZ);

	m_shape->CreateInputLayout(m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf());

	m_sky = GeometricPrimitive::CreateGeoSphere(context, 2.f, 3, false);

	m_skyEffect = std::make_unique<SkyboxEffect>(device);
	m_sky->CreateInputLayout(m_skyEffect.get(), m_skyInputLayout.ReleaseAndGetAddressOf());

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Content/Textures/lobbycube.dds", nullptr, m_cubemap.ReleaseAndGetAddressOf())
	);

	m_skyEffect->SetTexture(m_cubemap.Get());

    // TODO: Initialize device dependent objects here (independent of window size).
    device;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
	auto size = m_deviceResources->GetOutputSize();
	m_FontPos.x = float(size.right) / 6.f;
	m_FontPos.y = float(size.bottom) / 1.1f;

	m_Camera->SetViewSize(size.right, size.bottom);
	m_effect->SetView(m_Camera->GetViewMatrix());
	m_effect->SetProjection(m_Camera->GetProjectionMatrix());
	m_skyEffect->SetProjection(m_Camera->GetProjectionMatrix());


}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
	m_font.reset();
	m_SpriteBatch.reset();
	m_shape.reset();
	m_effect.reset();
	m_inputLayout.Reset();
	m_sky.reset();
	m_skyEffect.reset();
	m_skyInputLayout.Reset();
	m_cubemap.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
