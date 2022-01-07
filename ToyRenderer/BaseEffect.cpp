#include "pch.h"
#include "BaseEffect.h"


namespace
{
	constexpr uint32_t DirtyConstantBuffer = 0x1;
	constexpr uint32_t DirtyMVPMatrix = 0x2;
}

void BaseEffect::SetTexture(ID3D11ShaderResourceView* value)
{
	m_texture = value;
	m_dirtyFlags |= DirtyMVPMatrix;
}

void XM_CALLCONV BaseEffect::SetWorld(DirectX::FXMMATRIX value)
{
	m_world = value;
	m_dirtyFlags |= DirtyMVPMatrix;
}

void XM_CALLCONV BaseEffect::SetView(DirectX::FXMMATRIX value)
{
	m_view = value;
	m_dirtyFlags |= DirtyMVPMatrix;
}

void XM_CALLCONV BaseEffect::SetProjection(DirectX::FXMMATRIX value)
{
	m_proj = value;
	m_dirtyFlags |= DirtyMVPMatrix;
}

void XM_CALLCONV BaseEffect::SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection)
{
	m_view = view;
	m_proj = projection;
	m_world = world;
	m_dirtyFlags |= DirtyMVPMatrix;
}

BaseEffect::BaseEffect(ID3D11Device* device):
	m_dirtyFlags(uint32_t(-1))
{
	m_vsBlob = DX::ReadData(L"Shaders/PhongVS.cso");

	DX::ThrowIfFailed(
		device->CreateVertexShader(m_vsBlob.data(), m_vsBlob.size(), nullptr, m_vs.ReleaseAndGetAddressOf())
	);

	auto ps_blob = DX::ReadData(L"Shaders/PhongPS.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps_blob.data(), ps_blob.size(), nullptr, m_ps.ReleaseAndGetAddressOf())
	);


}

void BaseEffect::Apply(_In_ ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSGetShaderResources(0, 1, m_texture.GetAddressOf());

	deviceContext->VSSetShader(m_vs.Get(), nullptr, 0);
	deviceContext->PSSetShader(m_ps.Get(), nullptr, 0);

}

void BaseEffect::GetVertexShaderBytecode(_Out_ void const** pShaderByteCode, _Out_ size_t* pByteCodeLength) 
{
	assert(pShaderByteCode != nullptr && pByteCodeLength != nullptr);
	*pShaderByteCode = m_vsBlob.data();
	*pByteCodeLength = m_vsBlob.size();
}
