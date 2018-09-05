#include "Projectile.h"
#include "Renderer.h"
#include "Tile.h"

Projectile::Projectile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height):
	Entity(colour, x, y, x, scale, width, height)
{

}


Projectile::~Projectile()
{

}


void Projectile::Update(float deltaTime)
{
	if (m_isFired)
	{
		m_xPos += (-m_dirX * m_velocity) * deltaTime;
		m_yPos += (m_dirY * m_velocity) * deltaTime;
		
	}

}

void Projectile::Draw(XMMATRIX view, XMMATRIX projection)
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
	Renderer::pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw the vertex buffer to the back buffer
	Renderer::pImmediateContext->Draw(6, 0);
}

bool Projectile::GetIsFired()
{
	return m_isFired;
}

void Projectile::SetXPos(float x)
{
	m_xPos = x;
}

void Projectile::SetYPos(float y)
{
	m_yPos = y;
}

void Projectile::SetIsFired(bool isFired)
{
	m_isFired = isFired;
}

void Projectile::SetDirection(float dx, float dy)
{
	m_dirX = dx;
	m_dirY = dy;
}

void Projectile::SetRotation(float angle)
{
	m_rotation = angle;
}


