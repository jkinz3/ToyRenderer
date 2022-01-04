#include "pch.h"
#include "SkyboxEffect.h"
#include "ReadData.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace
{
	constexpr uint32_t DirtyConstantBuffer = 0x1;
	constexpr uint32_t DirtyWVPMatrix = 0x2;
}

SkyboxEffect::SkyboxEffect(ID3D11Device* device) :
	m_dirtyFlags(uint32_t(-1)),
	m_constantBuffer(device)
{
	m_vsBlob = DX::ReadData(L"SkyboxEffect_VS.cso");

	DX::ThrowIfFailed(
		device->CreateVertexShader(m_vsBlob.data(), m_vsBlob.size(), nullptr, m_vs.ReleaseAndGetAddressOf())
	);
	auto psBlob = DX::ReadData(L"SkyboxEffect_PS.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_ps.ReleaseAndGetAddressOf())
	);


}

void SkyboxEffect::Apply(ID3D11DeviceContext* deviceContext)
{

	if(m_dirtyFlags & DirtyWVPMatrix)
	{
		XMMATRIX view = m_view;
		view.r[3] = g_XMIdentityR3;
		m_worldViewProj = XMMatrixMultiply(view, m_proj);

		m_dirtyFlags &= ~DirtyWVPMatrix;
		m_dirtyFlags |= DirtyConstantBuffer;
	}
	if(m_dirtyFlags & DirtyConstantBuffer)
	{
		SkyboxEffectConstants constants;
		constants.worldViewProj = XMMatrixTranspose(m_worldViewProj);
		m_constantBuffer.SetData(deviceContext, constants);

		m_dirtyFlags &= ~DirtyConstantBuffer;
	}
	auto cb = m_constantBuffer.GetBuffer();

	deviceContext->VSSetConstantBuffers(0, 1, &cb);


	deviceContext->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	deviceContext->VSSetShader(m_vs.Get(), nullptr, 0);
	deviceContext->PSSetShader(m_ps.Get(), nullptr, 0);

}

void SkyboxEffect::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
{
	assert(pShaderByteCode != nullptr && pByteCodeLength != nullptr);
	*pShaderByteCode = m_vsBlob.data();
	*pByteCodeLength = m_vsBlob.size();
}

void SkyboxEffect::SetTexture(ID3D11ShaderResourceView* value)
{
	m_texture = value;
}

void XM_CALLCONV SkyboxEffect::SetWorld(DirectX::FXMMATRIX value)
{

}

void XM_CALLCONV SkyboxEffect::SetView(DirectX::FXMMATRIX value)
{
	m_view = value;
	m_dirtyFlags |= DirtyWVPMatrix;
}

void XM_CALLCONV SkyboxEffect::SetProjection(DirectX::FXMMATRIX value)
{
	m_proj = value;
	m_dirtyFlags |= DirtyWVPMatrix;
}

void XM_CALLCONV SkyboxEffect::SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection)
{
	m_view = view;
	m_proj = projection;
	m_dirtyFlags |= DirtyWVPMatrix;
}
