#pragma once
#include <vector>
#include "BaseEffect.h"
class SkyboxEffect : public DirectX::IEffect, public BaseEffect
{
public:
	explicit SkyboxEffect(ID3D11Device* device);



	void SetTexture(ID3D11ShaderResourceView* value);

private:

	struct __declspec(align(16)) SkyboxEffectConstants
	{
		DirectX::XMMATRIX worldViewProj;
	};

	DirectX::ConstantBuffer<SkyboxEffectConstants> m_constantBuffer;

};

