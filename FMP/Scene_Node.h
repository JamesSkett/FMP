#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;

#include <vector>

using namespace std;

class Mesh;
class ObjFileModel;
class Tile;

__declspec(align(16)) class Scene_Node
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

	Scene_Node();
	~Scene_Node();

	//Getters
	float GetXPos() const;
	float GetYPos() const;
	float GetZPos() const;
	float GetXAngle() const;
	float GetYAngle() const;
	float GetZAngle() const;
	float GetScale() const;
	float GetXDir() const;
	float GetYDir() const;
	float GetZDir() const;
	float GetWidth() const;
	float GetHeight() const;
	float GetDepth() const;

	//Setters
	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);
	void SetXAngle(float xAngle);
	void SetYAngle(float yAngle);
	void SetZAngle(float zAngle);
	void SetScale(float scale);

	bool UpdateXPos(float distance, Scene_Node* rootNode);
	bool UpdateYPos(float distance, Scene_Node* rootNode);
	bool UpdateZPos(float distance, Scene_Node* rootNode);
	bool UpdateXangle(float angle, Scene_Node* rootNode);
	bool UpdateYangle(float angle, Scene_Node* rootNode);
	bool UpdateZangle(float angle, Scene_Node* rootNode);

	bool MoveForward(float speed, Scene_Node* rootNode);

	void SetModel(Mesh* mesh);
	void AddChildNode(Scene_Node* n);

	bool DetachNode(Scene_Node* n);

	void Execute(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);

	//bool CheckCollision(std::vector<Tile*> tileMap);

	void setCanCollide(bool canCollide);
	void setCanDraw(bool canDraw);

	//bool CheckCollisionRay(xyz* rayPos, xyz* rayDirection, bool checkChildren);

protected:
	Mesh* m_pModel;
	ObjFileModel* m_pObject;
	std::vector <Scene_Node*> m_children;

	float m_x, m_y, m_z;
	float m_xangle, m_yangle, m_zangle;
	float m_scale;
	float m_dx, m_dy, m_dz;

	float m_width, m_height, m_depth;

	float m_world_centre_x;
	float m_world_centre_y;
	float m_world_centre_z;

	float m_world_scale;

	bool m_canCollide = true;
	bool m_canDraw = true;

	XMMATRIX m_local_world_matrix;
};

