//
// Game.cpp
//

#include "pch.h"
#include "Game.h"


extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

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
    auto kb = m_keyboard->GetState();

    if (kb.Space)
    {
        OutputDebugString(L"space\n");
    }

    if (kb.W)
    {
        if (sprite->get_loc().y > 0) {
            sprite->set_loc(sprite->get_loc().x, sprite->get_loc().y - 2.0f);
        }
    }

    if (kb.S)
    {
        if (sprite->get_loc().y < dimensions.y) {
            sprite->set_loc(sprite->get_loc().x, sprite->get_loc().y + 2.0f);
        }  
    }
    
    if (kb.A)
    {
        if (sprite->get_loc().x > 0) {
            sprite->set_loc(sprite->get_loc().x - 2.0f, sprite->get_loc().y);
        }
    }

    if (kb.D)
    {
        if (sprite->get_loc().x < dimensions.x) {
            sprite->set_loc(sprite->get_loc().x + 2.0f, sprite->get_loc().y);
        }
    }

    auto mouse = m_mouse->GetState();
    if (mouse.leftButton)
    {
        OutputDebugString(L"space\n");
    }

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

    m_spriteBatch->Begin();

    m_spriteBatch->Draw(m_background.Get(), m_fullscreenRect, &subrect);

    //m_spriteBatch->Draw(m_texture.Get(), m_screenPos, nullptr,
        //Colors::White, 0.f, m_origin);

    
    m_spriteBatch->Draw(sprite->get_frame(m_timer.GetTotalSeconds()).Get(), TranslateCoordinates(sprite->get_loc(), true), nullptr,
        Colors::White, 0.f, sprite->get_origin(), sprite->get_scale() * ratio);

    m_spriteBatch->Draw(sprite_2->get_frame(m_timer.GetTotalSeconds()).Get(), sprite_2->get_loc(), nullptr,
        Colors::White, 0.f, sprite_2->get_origin(), sprite_2->get_scale() * ratio);

    m_spriteBatch->Draw(sprite_3->get_frame(m_timer.GetTotalSeconds()).Get(), TranslateCoordinates(sprite_3->get_loc(), false), nullptr,
        Colors::White, 0.f, sprite_3->get_origin(), sprite_3->get_scale() * ratio);

    m_spriteBatch->End();

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

DirectX::SimpleMath::Vector2 Game::TranslateCoordinates(DirectX::SimpleMath::Vector2 loc, BOOL is_player)
{
    DirectX::SimpleMath::Vector2 screen_loc;
    if (is_player) {
        if (loc.x < (dimensions.x - subRectDim.x) && loc.x > subRectDim.x) {
            screen_loc.x = m_deviceResources->GetOutputSize().right / 2.0f;
            centre.x = loc.x;
            subrect.right = centre.x + subRectDim.x;
            subrect.left = centre.x - subRectDim.x;
        }
        else if (loc.x > dimensions.x - subRectDim.x) {
            screen_loc.x = m_deviceResources->GetOutputSize().right / 2.0f + (m_deviceResources->GetOutputSize().right / 2.0f * ((loc.x - dimensions.x + subRectDim.x) / subRectDim.x));
        }
        else {
            screen_loc.x = m_deviceResources->GetOutputSize().right / 2.0f * (loc.x / subRectDim.x);
        }

        if (loc.y < dimensions.y - subRectDim.y && loc.y > subRectDim.y) {
            screen_loc.y = m_deviceResources->GetOutputSize().bottom / 2.0f;
            centre.y = loc.y;
            subrect.top = centre.y - subRectDim.y;
            subrect.bottom = centre.y + subRectDim.y;
        }
        else if (loc.y > dimensions.y - subRectDim.y) {
            screen_loc.y = m_deviceResources->GetOutputSize().bottom / 2.0f + (m_deviceResources->GetOutputSize().bottom / 2.0f * ((loc.y - dimensions.y + subRectDim.y) / subRectDim.y));
        }
        else {
            screen_loc.y = m_deviceResources->GetOutputSize().bottom / 2.0f * (loc.y / subRectDim.y);
        }
    }
    else {
        /*if (loc.x < centre.x - subRectDim.x) {
            screen_loc.x = loc.x - (centre.x - subRectDim.x);
        }
        else if (loc.x > centre.x + subRectDim.x) {
            screen_loc.x = loc.x - (centre.x - subRectDim.x);
        }
        else {
            screen_loc.x = loc.x - (centre.x - subRectDim.x);
        }*/
        screen_loc.x = (dimensions.x / (subRectDim.x * 2)) * (loc.x - (centre.x - subRectDim.x));

        //screen_loc.x = 115;

        /*if (loc.y < centre.y - subRectDim.y) {
            screen_loc.y = loc.y - (centre.y - subRectDim.y);
        }
        else if (loc.y > centre.y + subRectDim.y) {
            screen_loc.y = loc.y - (centre.y - subRectDim.y);
        }
        else {
            screen_loc.y = loc.y - (centre.y - subRectDim.y);
        }*/
        screen_loc.y = (dimensions.y / (subRectDim.y * 2)) * (loc.y - (centre.y - subRectDim.y));
    }
    return screen_loc;
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
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
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
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
    height = 480;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // TODO: Initialize device dependent objects here (independent of window size).
    //device;
    auto context = m_deviceResources->GetD3DDeviceContext();
    m_spriteBatch = std::make_unique<SpriteBatch>(context);

    //ComPtr<ID3D11Resource> resource;
    //DX::ThrowIfFailed(
       // CreateDDSTextureFromFile(device, L"cat.dds",
            //resource.GetAddressOf(),
           // m_texture.ReleaseAndGetAddressOf()));

    //ComPtr<ID3D11Texture2D> cat;
   // DX::ThrowIfFailed(resource.As(&cat));

  // CD3D11_TEXTURE2D_DESC catDesc;
   // cat->GetDesc(&catDesc);

   // m_origin.x = float(catDesc.Width * 2);
   // m_origin.y = float(catDesc.Height * 2);

    m_states = std::make_unique<CommonStates>(device);

    DX::ThrowIfFailed(
        CreateWICTextureFromFile(device, L"sunset.jpg", nullptr,
            m_background.ReleaseAndGetAddressOf()));
    dimensions.x = 800;
    dimensions.y = 480;

    subrect.left = 100;
    subrect.right = 300;
    subRectDim.x = 100;

    subrect.top = 100;
    subrect.bottom = 220;
    subRectDim.y = 60;

    centre.x = 175;
    centre.y = 175;

    std::vector<std::vector<std::string>> name = { { "frame1.png", "frame2.png" } };

    sprite = std::make_unique<Entity>(name,
        m_deviceResources,
        0.33,
        m_timer.GetTotalSeconds(),
        175,
        175,
        25,
        25);

    std::vector<std::vector<std::string>> name_2 = { { "cat.png" } };

    sprite_2 = std::make_unique<Entity>(name_2,
        m_deviceResources,
        0.33,
        m_timer.GetTotalSeconds(),
        50,
        50,
        50,
        50);

    std::vector<std::vector<std::string>> name_3 = { { "grass.png" } };

    sprite_3 = std::make_unique<Entity>(name_3,
        m_deviceResources,
        0.33,
        m_timer.GetTotalSeconds(),
        800,
        480,
        95,
        97);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    float old = m_fullscreenRect.right;
    auto viewSize = m_deviceResources->GetOutputSize();
    m_screenPos.x = float(viewSize.right) / 2.f;
    m_screenPos.y = float(viewSize.bottom) / 2.f;

    m_fullscreenRect = m_deviceResources->GetOutputSize();

    ratio = m_fullscreenRect.right / 800;
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_texture.Reset();
    m_spriteBatch.reset();
    m_states.reset();
    m_background.Reset();
    sprite.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
