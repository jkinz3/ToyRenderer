#pragma once
class BaseEffect : public DirectX::IEffect, public DirectX::IEffectMatrices
{
public:
	explicit BaseEffect(ID3D11Device* device);
	BaseEffect() = default;
	virtual void Apply(_In_ ID3D11DeviceContext* deviceContext) override;
	virtual void GetVertexShaderBytecode(_Out_ void const** pShaderByteCode, _Out_ size_t* pByteCodeLength) override;
	virtual void SetTexture(ID3D11ShaderResourceView* value);


	void XM_CALLCONV SetWorld(DirectX::FXMMATRIX value) override;
	void XM_CALLCONV SetView(DirectX::FXMMATRIX value) override;
	void XM_CALLCONV SetProjection(DirectX::FXMMATRIX value) override;
	void XM_CALLCONV SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection) override;

protected:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	std::vector<uint8_t> m_vsBlob;


	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	DirectX::SimpleMath::Matrix m_worldViewProj;


	uint32_t m_dirtyFlags;
};


