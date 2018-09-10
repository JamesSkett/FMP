//#include "Entity.h"
#include "Renderer.h"
#include "Math.h"
#include "Player.h"
#include "Tile.h"

Entity::Entity(float x, float y, float z, float scale, float width, float height)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_scale = scale;

	m_startPos = { x, y };

	m_width = width;
	m_height = height;

	/*if (FAILED(CreateVertices(colour)))
	{
		DXTRACE_MSG("Failed to Initialise Graphics");
	}*/
}


Entity::~Entity()
{
	if (m_pConstantBuffer0) m_pConstantBuffer0->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pInputLayout)  m_pInputLayout->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader)  m_pPixelShader->Release();
}

void Entity::Draw(XMMATRIX view, XMMATRIX projection)
{
	
	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendEnable, 0, 0xffffffff);
	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendDisable, 0, 0xffffffff);

	XMMATRIX world, WVP;

	world = XMMatrixScaling(m_scale, m_scale, m_scale);

	world *= XMMatrixRotationZ(m_zangle);

	world *= XMMatrixTranslation(m_x, m_y, m_z);
	


	WVP = world * view * projection;

	XMVECTOR posVec = WVP.r[3];

	m_screenSpaceX = XMVectorGetX(posVec);
	m_screenSpaceY = XMVectorGetY(posVec);

	ENTITY_CONSTANT_BUFFER entity_cb_values;

	entity_cb_values.WorldViewProjection = WVP;

	float x = (Player::s_playerPos.x + 1) / 2.0f;
	float y = 1 - (Player::s_playerPos.y + 1) / 2.0f;
	
	x *= 1920.f;
	y *= 1080.f;
	
	entity_cb_values.playerPos = XMFLOAT2(x, y);
	entity_cb_values.playerRotation = Player::s_rotation;
	entity_cb_values.range = 1.0f;
	entity_cb_values.fog = Renderer::s_FogOfWar;

	Renderer::pImmediateContext->UpdateSubresource(m_pConstantBuffer0, 0, 0, &entity_cb_values, 0, 0);

	Renderer::pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	Renderer::pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	Renderer::pImmediateContext->VSSetShader(m_pVertexShader, 0, 0);
	Renderer::pImmediateContext->PSSetShader(m_pPixelShader, 0, 0);
	Renderer::pImmediateContext->IASetInputLayout(m_pInputLayout);

	//Set the vertex buffer //03-01
	UINT stride = sizeof(POS_COL_VERTEX);
	UINT offset = 0;
	Renderer::pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);



	// Select which primitive type to use //03-01
	Renderer::pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw the vertex buffer to the back buffer
	Renderer::pImmediateContext->Draw(6, 0);
}

void Entity::SetWalkedThroughDoor(bool value)
{
	m_walkedThroughDoor = value;
}

void Entity::GetColBoxParameters(float &x, float &y, float &w, float &h)
{
	x = m_x;
	y = m_y;
	w = m_width;
	h = m_height;
}

bool Entity::GetWalkedThroughDoor()
{
	return m_walkedThroughDoor;
}

XMFLOAT2 Entity::GetDoorPos()
{
	return m_doorPos;
}

HRESULT Entity::CreateVertices(XMFLOAT4 colour)
{
	HRESULT hr = S_OK;

	// Define vertices of a triangle - screen coordinates -1.0 to +1.0
	POS_COL_VERTEX vertices[] =
	{

		{ XMFLOAT3( 0.0f,  0.9f, 1.0f), colour },
		{ XMFLOAT3( 0.9f, -0.9f, 1.0f), colour },
		{ XMFLOAT3(-0.9f, -0.9f, 1.0f), colour },
								 

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

bool Entity::CollisionCheck(std::vector <Tile*> tilemap)
{
	for (unsigned int i = 0; i < tilemap.size(); i++)
	{
		if (tilemap[i]->GetIndex() == 2)
		{
			float box1x = m_x - (m_width / 2);
			float box1y = m_y - (m_height / 2);
			float box1w = m_width;
			float box1h = m_height;

			float box2x, box2y;
			float box2w, box2h;

			tilemap[i]->GetParameters(box2x, box2y, box2w, box2h);

			box2x = box2x - (box2w / 2);
			box2y = box2y - (box2h / 2);

			if ((box1x < box2x + box2w) && (box1x + box1w > box2x) && (box1y < box2y + box2h) && (box1h + box1y > box2y))
			{

				return true;
			}
		}
		else if (tilemap[i]->GetIndex() == 3)
		{
			float box1x = m_x - (m_width / 2);
			float box1y = m_y - (m_height / 2);
			float box1w = m_width;
			float box1h = m_height;

			float box2x, box2y;
			float box2w, box2h;

			tilemap[i]->GetParameters(box2x, box2y, box2w, box2h);

			box2x = box2x - (box2w / 2);
			box2y = box2y - (box2h / 2);

			if ((box1x < box2x + box2w) && (box1x + box1w > box2x) && (box1y < box2y + box2h) && (box1h + box1y > box2y))
			{
				SetWalkedThroughDoor(true);
				m_doorPos = XMFLOAT2(tilemap[i]->GetXPos(), tilemap[i]->GetYPos());
			}
		}
	}

	return false;
}

void Entity::SetViewCone(Asset* viewCone)
{
	m_viewCone = viewCone;
}

void Entity::Reset()
{
	m_x = m_startPos.x;
	m_y = m_startPos.y;
}

bool Entity::CollisionCheck(Entity* colObject)
{
	float box1x = m_x - (m_width / 2);
	float box1y = m_y - (m_height / 2);
	float box1w = m_width;
	float box1h = m_height;

	float box2x, box2y;
	float box2w, box2h;

	colObject->GetColBoxParameters(box2x, box2y, box2w, box2h);

	box2x = box2x - (box2w / 2);
	box2y = box2y - (box2h / 2);

	if ((box1x < box2x + box2w) && (box1x + box1w > box2x) && (box1y < box2y + box2h) && (box1h + box1y > box2y))
	{
		return true;
	}

	return false;
}

