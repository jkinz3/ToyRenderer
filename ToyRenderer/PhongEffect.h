#pragma once
#include "BaseEffect.h"
class PhongEffect : public BaseEffect
{
	explicit PhongEffect(ID3D11Device* device);


public:
	void Apply(_In_ ID3D11DeviceContext* deviceContext) override;

	struct __declspec(align(16)) PhongEffectConstants
	{
		DirectX::XMMATRIX MVP;
	};

	DirectX::ConstantBuffer<PhongEffectConstants> m_constantBuffer;

};

