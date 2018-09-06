#pragma once
//DirectX includes
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>

#include <DirectXMath.h>
using namespace DirectX;

class ObjFileModel;
class Math;

//Shader Constant buffer
struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection;
	XMMATRIX World;
};

//Light structures for the pixel shader
struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

struct cbPerFrame
{
	Light light;
};

__declspec(align(16)) class Mesh
{
public:

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		return _mm_free(p);
	}

	//Constructor uses the device and context from the renderer class
	Mesh(ID3D11Device* D3D11Device, ID3D11DeviceContext* ImmediateContext);
	~Mesh();

	Math math;

	//loads the mesh object
	int LoadObjModel(char* fileName);

	//draws the mesh
	void Draw(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);
	//adds a texture to the mesh
	void AddTexture(char* fileName);

	//Setters
	void SetXPos(float x_pos);
	void SetYPos(float y_pos);
	void SetZPos(float z_pos);
	void SetXAngle(float x_angle);
	void SetYAngle(float y_angle);
	void SetZAngle(float z_angle);
	void SetScale(float scale);

	//Getters
	float GetXPos();
	float GetYPos();
	float GetZPos();
	float GetXAngle();
	float GetYAndle();
	float GetZAngle();
	float GetScale();
	float GetBoundingSphereRadius();
	float GetBoundingSphere_x();
	float GetBoundingSphere_y();
	float GetBoundingSphere_z();

	bool GetIsSkyBox();
	ObjFileModel* GetObject();

	//use these to move scale and rotate object
	void UpdateXPos(float distance);
	void UpdateYPos(float distance);
	void UpdateZPos(float distance);
	void UpdateXAngle(float angle);
	void UpdateYAngle(float angle);
	void UpdateZAngle(float angle);
	void UpdateScale(float scale);
	XMVECTOR GetBoundingSphereWorldSpacePosition();

	//moves the object in the direction its facing
	void MoveForward(float speed);

	//Makes the mesh face the target points
	void Lookat_XZ(float targetX, float targetY, float targetZ);


	bool CheckCollision(Mesh* targetMesh);

	bool m_isSkyBox = false;


protected:
	//Variables used to initialise graphics
	ID3D11Device*             m_pD3D11Device;
	ID3D11DeviceContext*      m_pImmediateContext;

	ObjFileModel*		      m_pObject;
	ID3D11VertexShader*       m_pVShader;
	ID3D11PixelShader*	      m_pPShader;
	ID3D11PixelShader*	      m_D2D_PS;
	ID3D11InputLayout*	      m_pInputLayout;
	ID3D11Buffer*		      m_pConstantBuffer;

	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState*       m_pSampler0;

	//lighting variables
	ID3D11Buffer*             m_cbPerFrameBuffer;
	Light m_light;
	cbPerFrame m_constBuffPerFrame;

	

	float m_dx, m_dy, m_dz;

	float m_x, m_y, m_z;
	float m_xangle, m_zangle, m_yangle;
	float m_scale;

	float m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z;
	float m_bounding_sphere_radius;


	void CalculateModelCentrePoint();
	void CalculateBoundingSphereRadius();

};
