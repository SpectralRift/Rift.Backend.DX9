#define D3D_DEBUG_INFO

#include <d3d9.h>
#include <stdexcept>
#include <sstream>

#include <Engine/Backend/DX9/DX9_Backend.hpp>
#include <Engine/Backend/DX9/DX9_Shader.hpp>
#include <Engine/Backend/DX9/DX9_ShaderProgram.hpp>
#include <Engine/Backend/DX9/DX9_VertexBuffer.hpp>
#include <Engine/Backend/DX9/DX9_Texture.hpp>

namespace engine::backend::dx9 {
    bool DX9Backend::Initialize() {
        if (!h_DX9Device) {
            printf("DX9Backend: device is NULL during initialization.\n");
            return false;
        }

        printf("DX9Backend: DX9 backend initialized!\n");

        return true;
    }

    void DX9Backend::Shutdown() {
        h_DX9Device = nullptr;
    }

    std::string DX9Backend::GetName() const {
        return "DirectX 9";
    }

    std::string DX9Backend::GetIdentifier() const {
        return "dx9";
    }

    void DX9Backend::SetViewport(core::math::Vector2 pos, core::math::Vector2 size) {
        if (!h_DX9Device) {
            printf("DX9Backend: Cannot set viewport, device is not initialized.\n");

        }

        D3DVIEWPORT9 viewport = {};
        viewport.X = static_cast<DWORD>(pos.x);
        viewport.Y = static_cast<DWORD>(pos.y);
        viewport.Width = static_cast<DWORD>(size.x);
        viewport.Height = static_cast<DWORD>(size.y);
        viewport.MinZ = 0.0f;
        viewport.MaxZ = 1.0f;

        HRESULT hr = h_DX9Device->SetViewport(&viewport);
        if (FAILED(hr)) {
            printf("DX9Backend: Failed to set viewport.\n");
            return;
        }
    }

    void DX9Backend::SetScissor(core::math::Vector2 start, core::math::Vector2 size) {
        if (!h_DX9Device) {
            printf("DX9Backend: Cannot set scissor, device is not initialized.\n");
            return;
        }

        RECT scissorRect;
        scissorRect.left = static_cast<LONG>(start.x);
        scissorRect.top = static_cast<LONG>(start.y);
        scissorRect.right = static_cast<LONG>(start.x + size.x);
        scissorRect.bottom = static_cast<LONG>(start.y + size.y);

        HRESULT hr = h_DX9Device->SetScissorRect(&scissorRect);
        if (FAILED(hr)) {
            printf("DX9Backend: Failed to set scissor rect.\n");
            return;
        }

        hr = h_DX9Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
        if (FAILED(hr)) {
            printf("DX9Backend: Failed to enable scissor test.\n");
            return;
        }
    }

    void DX9Backend::Clear(core::runtime::graphics::Color color) {
        if (!h_DX9Device) {
            printf("DX9Backend: Cannot clear, device is not initialized.\n");
            return;
        }

        D3DCOLOR dxColor = D3DCOLOR_ARGB(
                color.a,
                color.r,
                color.g,
                color.b
        );

//        h_DX9Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//        h_DX9Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

        HRESULT hr = h_DX9Device->Clear(0, nullptr, D3DCLEAR_TARGET, dxColor, 1.0f, 0);
        if (FAILED(hr)) {
            printf("DX9Backend: Failed to clear the render target.\n");
            return;
        }
    }

    void DX9Backend::EnableFeatures(core::runtime::graphics::BackendFeature featuresMask) {
        if (featuresMask & core::runtime::graphics::BACKEND_FEATURE_SCISSOR_TEST) {
            h_DX9Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
        }

        if (featuresMask & core::runtime::graphics::BACKEND_FEATURE_ALPHA_BLENDING) {
//            h_DX9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//            h_DX9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//            h_DX9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//            h_DX9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//            h_DX9Device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
//            h_DX9Device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
        }

        m_ActiveFeatures |= featuresMask;
    }

    void DX9Backend::DisableFeatures(core::runtime::graphics::BackendFeature featuresMask) {
        if ((featuresMask & core::runtime::graphics::BACKEND_FEATURE_SCISSOR_TEST) &&
            (m_ActiveFeatures & core::runtime::graphics::BACKEND_FEATURE_SCISSOR_TEST)) {
            h_DX9Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
        }

        if ((featuresMask & core::runtime::graphics::BACKEND_FEATURE_ALPHA_BLENDING) &&
            (m_ActiveFeatures & core::runtime::graphics::BACKEND_FEATURE_ALPHA_BLENDING)) {
            h_DX9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        }

        m_ActiveFeatures &= ~featuresMask;
    }

    core::runtime::graphics::BackendFeature DX9Backend::GetActiveFeatures() {
        return static_cast<core::runtime::graphics::BackendFeature>(m_ActiveFeatures);
    }

    std::unique_ptr<core::runtime::graphics::IVertexBuffer> DX9Backend::CreateVertexBuffer() {
        return std::make_unique<DX9VertexBuffer>(h_DX9Device);
    }

    std::unique_ptr<core::runtime::graphics::IShader> DX9Backend::CreateShader() {
        return std::make_unique<DX9Shader>(h_DX9Device);
    }

    std::unique_ptr<core::runtime::graphics::IShaderProgram> DX9Backend::CreateShaderProgram() {
        return std::make_unique<DX9ShaderProgram>(h_DX9Device);
    }

    std::unique_ptr<core::runtime::graphics::ITexture> DX9Backend::CreateTexture() {
        return std::make_unique<DX9Texture>(h_DX9Device);
    }
}