#include "pch.h"
#include "SkyboxEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace
{
	constexpr uint32_t DirtyConstantBuffer = 0x1;
	constexpr uint32_t DirtyWVPMatrix = 0x2;
}

SkyboxEffect::SkyboxEffect(ID3D11Device* device) :
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

