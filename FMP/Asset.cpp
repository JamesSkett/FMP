#include "Asset.h"


Asset::Asset(char* filename, float x, float y, float z, float scale, float width, float height)
{
	m_xPos = x;
	m_yPos = y;
	m_zPos = z;
	m_scale = scale;

	m_width = width;
	m_height = height;

	m_colour = Renderer::colour.Black;

	//Create the object vertices
	if (FAILED(CreateVertices(filename)))
	{
		DXTRACE_MSG("Failed to Initialise Graphics");
	}
}


Asset::~Asset()
{
	if (m_pConstantBuffer0) m_pConstantBuffer0->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pInputLayout)  m_pInputLayout->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader)  m_pPixelShader->Release();
	if (m_pTexture)     m_pTexture->Release();
	//if (m_pTexture1 && m_isTexture1)     m_pTexture->Release();
	//if (m_pTexture2 && m_isTexture2)     m_pTexture->Release();
	if (m_pSampler0)     m_pSampler0->Release();
}

void Asset::Draw(XMMATRIX view, XMMATRIX projection)
{
	XMMATRIX world, WVP;

	world = XMMatrixScaling(m_scale, m_scale, m_scale);

	world *= XMMatrixRotationZ(m_rotation);

	world *= XMMatrixTranslation(m_xPos, m_yPos, m_zPos);



	WVP = world * view * projection;

	XMVECTOR posVec = WVP.r[3];

	m_screenSpaceX = XMVectorGetX(posVec);
	m_screenSpaceY = XMVectorGetY(posVec);

	ENTITY_CONSTANT_BUFFER entity_cb_values;

	entity_cb_values.WorldViewProjection = WVP;
	entity_cb_values.colour = m_colour;

	Renderer::pImmediateContext->UpdateSubresource(m_pConstantBuffer0, 0, 0, &entity_cb_values, 0, 0);

	
	Renderer::pImmediateContext->VSSetShader(m_pVertexShader, 0, 0);
	Renderer::pImmediateContext->PSSetShader(m_pPixelShader, 0, 0);
	Renderer::pImmediateContext->IASetInputLayout(m_pInputLayout);
	Renderer::pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	Renderer::pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);

	Renderer::pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture);
	//else if (m_isTexture1) Renderer::pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture1);
	//else if (m_isTexture2) Renderer::pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture2);

	//Set the vertex buffer //03-01
	UINT stride = sizeof(POS_TEX_VERTEX);
	UINT offset = 0;
	Renderer::pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);



	// Select which primitive type to use //03-01
	Renderer::pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw the vertex buffer to the back buffer
	Renderer::pImmediateContext->Draw(6, 0);
}

void Asset::SetPos(float x, float y)
{
	m_xPos = x;
	m_yPos = y;
}

void Asset::SetRotation(float rotation)
{
	m_rotation = rotation;
}

void Asset::UpdateRotation(float speed)
{
	m_rotation += speed;
}

void Asset::SetColour(XMFLOAT4 colour)
{
	m_colour = colour;
}

HRESULT Asset::CreateVertices(char* filename)
{
	HRESULT hr = S_OK;

	// Define vertices of a triangle - screen coordinates -1.0 to +1.0
	POS_TEX_VERTEX vertices[] =
	{

		{ XMFLOAT3( 0.9f,  0.9f, 1.0f),  XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3( 0.9f, -0.9f, 1.0f),  XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.9f, -0.9f, 1.0f),  XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-0.9f, -0.9f, 1.0f),  XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.9f,  0.9f, 1.0f),  XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3( 0.9f,  0.9f, 1.0f),  XMFLOAT2(1.0f, 0.0f) },

	};

	//Set up and create the vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //Used by the CPU and GPU
	bufferDesc.ByteWidth = sizeof(POS_TEX_VERTEX) * 6; //Total size of buffer, 3 vertices
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //Use as vertex buffer
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; //Allow CPU access
	hr = Renderer::pD3DDevice->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer); //Create the buffer

	if (FAILED(hr))
	{
		return hr;
	}

	//Create the constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //Can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = sizeof(ENTITY_CONSTANT_BUFFER);
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = Renderer::pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer0);
	if (FAILED(hr)) return hr;

	//Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	// Lock the buffer to allow writing
	Renderer::pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	//Copy the data
	memcpy(ms.pData, vertices, sizeof(vertices));

	//unlock the data
	Renderer::pImmediateContext->Unmap(m_pVertexBuffer, NULL);

	//Load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("AssetShader.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0)// check for shader compliation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) //dont fail if error is just a warning
		{
			return hr;
		};
	}

	hr = D3DX11CompileFromFile("AssetShader.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0)// check for shader compliation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) //dont fail if error is just a warning
		{
			return hr;
		};
	}

	//Create shader objects
	hr = Renderer::pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVertexShader);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = Renderer::pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPixelShader);

	if (FAILED(hr))
	{
		return hr;
	}

	D3DX11CreateShaderResourceViewFromFile(Renderer::pD3DDevice, filename, NULL, NULL, &m_pTexture, NULL);

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	Renderer::pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler0);

	// Set the shader objects as active
	Renderer::pImmediateContext->VSSetShader(m_pVertexShader, 0, 0);
	Renderer::pImmediateContext->PSSetShader(m_pPixelShader, 0, 0);

	// Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0,    D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = Renderer::pD3DDevice->CreateInputLayout(iedesc, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	Renderer::pImmediateContext->IASetInputLayout(m_pInputLayout);

	return S_OK;
}
