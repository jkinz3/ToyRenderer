#include "pch.h"
#include "PhongEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace
{
	constexpr uint32_t DirtyConstantBuffer = 0x1;
	constexpr uint32_t DirtyMVPMatrix = 0x2;
}

PhongEffect::PhongEffect(ID3D11Device* device)
	:m_constantBuffer(device)
{
	static_assert((sizeof(PhongEffect::PhongEffectConstants) % 16) == 0, "CB Size Alignment");
	m_vsBlob = DX::ReadData(L"Shaders/PhongVS.cso");

	DX::ThrowIfFailed(
		device->CreateVertexShader(m_vsBlob.data(), m_vsBlob.size(), nullptr, m_vs.ReleaseAndGetAddressOf())
	);

	auto ps_blob = DX::ReadData(L"Shaders/PhongPS.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps_blob.data(), ps_blob.size(), nullptr, m_ps.ReleaseAndGetAddressOf())
	);

}

void PhongEffect::Apply(_In_ ID3D11DeviceContext* deviceContext)
{

	if (m_dirtyFlags & DirtyMVPMatrix)
	{
		// Skybox ignores m_world matrix and the translation of m_view
		XMMATRIX view = m_view;
		view.r[3] = g_XMIdentityR3;
		m_worldViewProj = XMMatrixMultiply(view, m_proj);

		m_dirtyFlags &= ~DirtyMVPMatrix;
		m_dirtyFlags |= DirtyConstantBuffer;
	}

	if (m_dirtyFlags & DirtyConstantBuffer)
	{
		PhongEffectConstants constants;
		constants.MVP = XMMatrixTranspose(m_worldViewProj);
		m_constantBuffer.SetData(deviceContext, constants);

		m_dirtyFlags &= ~DirtyConstantBuffer;
	}
	deviceContext->PSGetShaderResources(0, 1, m_texture.GetAddressOf());

	deviceContext->VSSetShader(m_vs.Get(), nullptr, 0);
	deviceContext->PSSetShader(m_ps.Get(), nullptr, 0);
}

