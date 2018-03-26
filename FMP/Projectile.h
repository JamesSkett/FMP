#pragma once
#include "Entity.h"


class Projectile : public Entity
{
public:
	Projectile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Projectile();

	void Update(float dirX, float dirY);

	bool GetIsFired();

	void SetXPos(float x);
	void SetYPos(float y);
	void SetIsFired(bool isFired);

	struct POS_COL_VERTEX
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Col;
	};

	struct ENTITY_CONSTANT_BUFFER
	{
		XMMATRIX WorldViewProjection;
	};

private:
	float m_velocity = 0.1f;

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11Buffer*			m_pConstantBuffer0;

	float m_xPos, m_yPos, m_zPos, m_scale;
	float m_xAngle, m_yAngle, m_zAngle;
	float m_width, m_height;

	float m_dirX = 0.0f;
	float m_dirY = 0.0f;

	float m_isFired = false;

	
};

