#include "Tile.h"
#include "Renderer.h"


Tile::Tile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height, int index) 
{
	m_xPos = x;
	m_yPos = y;
	m_zPos = z;
	m_scale = scale;
	m_w = width;
	m_h = height;
	m_index = index;

	if (FAILED(CreateVertices(colour)))
	{
		DXTRACE_MSG("Failed to Initialise Graphics");
	}
}


Tile::~Tile()
{
	if (m_pConstantBuffer0) m_pConstantBuffer0->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pInputLayout)  m_pInputLayout->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader)  m_pPixelShader->Release();
}

void Tile::Draw(XMMATRIX view, XMMATRIX projection)
{
	XMMATRIX world, WVP;

	world = XMMatrixScaling(m_scale, m_scale, m_scale);

	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));

	world *= XMMatrixTranslation(m_xPos, m_yPos, m_zPos);



	WVP = world * view * projection;

	ENTITY_CONSTANT_BUFFER entity_cb_values;

	entity_cb_values.WorldViewProjection = WVP;

	Renderer::pImmediateContext->UpdateSubresource(m_pConstantBuffer0, 0, 0, &entity_cb_values, 0, 0);

	Renderer::pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	Renderer::pImmediateContext->VSSetShader(m_pVertexShader, 0, 0);
	Renderer::pImmediateContext->PSSetShader(m_pPixelShader, 0, 0);
	Renderer::pImmediateContext->IASetInputLayout(m_pInputLayout);

	//Set the vertex buffer //03-01
	UINT stride = sizeof(POS_COL_VERTEX);
	UINT offset = 0;
	Renderer::pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);



	// Select which primitive type to use //03-01
	Renderer::pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw the vertex buffer to the back buffer
	Renderer::pImmediateContext->Draw(6, 0);
}

void Tile::GetParameters(float &x, float &y, float &width, float &height)
{
	x = m_xPos;
	y = m_yPos;
	width = m_w;
	height = m_h;
}

int Tile::GetIndex()
{
	return m_index;
}

float Tile::GetXPos()
{
	return m_xPos;
}

float Tile::GetYPos()
{
	return m_yPos;
}

bool Tile::GetIsWalkable()
{
	return m_walkable;
}

void Tile::SetGValue(int value)
{
	m_gValue = value;
}

void Tile::SetHValue(int value)
{
	m_hValue = value;
}

void Tile::SetFValue(int value)
{
	m_fValue = value;
}

int Tile::Get_G_Value()
{
	return m_gValue;
}

int Tile::Get_H_Value()
{
	return m_hValue;
}

int Tile::Get_F_Value()
{
	return m_fValue;
}


HRESULT Tile::CreateVertices(XMFLOAT4 colour)
{
	HRESULT hr = S_OK;

	// Define vertices of a triangle - screen coordinates -1.0 to +1.0
	POS_COL_VERTEX vertices[] =
	{

		{ XMFLOAT3(0.9f,  0.9f, 1.0f), colour },
		{ XMFLOAT3(0.9f, -0.9f, 1.0f), colour },
		{ XMFLOAT3(-0.9f, -0.9f, 1.0f), colour },

		{ XMFLOAT3(-0.9f, -0.9f, 1.0f), colour },
		{ XMFLOAT3(-0.9f,  0.9f, 1.0f), colour },
		{ XMFLOAT3(0.9f,  0.9f, 1.0f), colour },

	};

	//Set up and create the vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //Used by the CPU and GPU
	bufferDesc.ByteWidth = sizeof(POS_COL_VERTEX) * 6; //Total size of buffer, 3 vertices
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
	constant_buffer_desc.ByteWidth = 64;
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
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0)// check for shader compliation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) //dont fail if error is just a warning
		{
			return hr;
		};
	}

	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

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

	// Set the shader objects as active
	Renderer::pImmediateContext->VSSetShader(m_pVertexShader, 0, 0);
	Renderer::pImmediateContext->PSSetShader(m_pPixelShader, 0, 0);

	// Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = Renderer::pD3DDevice->CreateInputLayout(iedesc, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	Renderer::pImmediateContext->IASetInputLayout(m_pInputLayout);

	return S_OK;
}
