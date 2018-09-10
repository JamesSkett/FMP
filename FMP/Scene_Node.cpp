#include "Scene_Node.h"
#include "GameSystem.h"
#include "Mesh.h"
#include "objfilemodel.h"
#include "Tile.h"

Scene_Node::Scene_Node()
{
	m_pModel = NULL;

	m_x = 0.0f; m_y = 0.0f; m_z = 0.0f;
	m_xangle = 0.0f; m_yangle = 0.0f; m_zangle = 0.0f;
	m_scale = 1.0f;

	m_dx = sinf(XMConvertToRadians(m_yangle));
	//m_dy = atan(XMConvertToRadians(m_xangle));
	m_dz = cosf(XMConvertToRadians(m_yangle));
}


Scene_Node::~Scene_Node()
{
}

//Getters
float Scene_Node::GetXPos() const
{
	return m_x;
}
float Scene_Node::GetYPos() const
{
	return m_y;
}
float Scene_Node::GetZPos() const
{
	return m_z;
}
float Scene_Node::GetXAngle() const
{
	return m_xangle;
}
float Scene_Node::GetYAngle() const
{
	return m_yangle;
}
float Scene_Node::GetZAngle() const
{
	return m_zangle;
}

float Scene_Node::GetScale() const
{
	return m_scale;
}

float Scene_Node::GetXDir() const
{
	return m_dx;
}

float Scene_Node::GetYDir() const
{
	return m_dy;
}

float Scene_Node::GetZDir() const
{
	return m_dz;
}

float Scene_Node::GetWidth() const
{
	return m_width;
}

float Scene_Node::GetHeight() const
{
	return m_height;
}

float Scene_Node::GetDepth() const
{
	return m_depth;
}

//Setters
void Scene_Node::SetXPos(float x)
{
	m_x = x;
}
void Scene_Node::SetYPos(float y)
{
	m_y = y;
}
void Scene_Node::SetZPos(float z)
{
	m_z = z;
}
void Scene_Node::SetXAngle(float xAngle)
{
	m_xangle = xAngle;
}
void Scene_Node::SetYAngle(float yAngle)
{
	m_yangle = yAngle;
}
void Scene_Node::SetZAngle(float zAngle)
{
	m_zangle = zAngle;
}

void Scene_Node::SetScale(float scale)
{
	m_scale = scale;
}

bool Scene_Node::UpdateXPos(float distance, Scene_Node* rootNode)
{
	float old_x = m_x;	// save current state 
	m_x += distance;		// update state

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	//rootNode->UpdateCollisionTree(&identity, 1.0f);

	// check for collision of this node (and children) against all other nodes
	//if (CheckCollision(rootNode) == true)
	//{
	//	// if collision restore state
	//	m_x = old_x;

	//	return true;
	//}

	return false;

}

bool Scene_Node::UpdateYPos(float distance, Scene_Node* rootNode)
{
	float old_y = m_y;	// save current state 
	m_y += distance;		// update state

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	//rootNode->UpdateCollisionTree(&identity, 1.0f);

	// check for collision of this node (and children) against all other nodes
	//if (CheckCollision(rootNode) == true)
	//{
	//	// if collision restore state
	//	m_y = old_y;

	//	return true;
	//}

	return false;
}

bool Scene_Node::UpdateZPos(float distance, Scene_Node* rootNode)
{
	float old_z = m_z;	// save current state 
	m_z += distance;		// update state

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	//rootNode->UpdateCollisionTree(&identity, 1.0f);

	// check for collision of this node (and children) against all other nodes
	//if (CheckCollision(rootNode) == true)
	//{
	//	// if collision restore state
	//	m_z = old_z;

	//	return true;
	//}

	return false;
}

bool Scene_Node::UpdateXangle(float angle, Scene_Node* rootNode)
{

	if (m_xangle > 60.0f)
	{
		m_xangle = 60.0f;
	}
	else if (m_xangle < -60.0f)
	{
		m_xangle = -60.0f;
	}

	float old_xangle = m_xangle;
	m_xangle += angle;

	m_dx = sinf(XMConvertToRadians(m_yangle));
	m_dy = atan(XMConvertToRadians(m_xangle));
	m_dz = cosf(XMConvertToRadians(m_yangle));

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	//rootNode->UpdateCollisionTree(&identity, 1.0f);

	// check for collision of this node (and children) against all other nodes
	//if (CheckCollision(rootNode) == true)
	//{
	//	// if collision restore state
	//	m_xangle = old_xangle;

	//	return true;
	//}

	return false;
}

bool Scene_Node::UpdateYangle(float angle, Scene_Node* rootNode)
{
	float old_yangle = m_yangle;
	m_yangle += angle;

	m_dx = sinf(XMConvertToRadians(m_yangle));
	m_dy = atan(XMConvertToRadians(m_xangle));
	m_dz = cosf(XMConvertToRadians(m_yangle));

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	//rootNode->UpdateCollisionTree(&identity, 1.0f);

	// check for collision of this node (and children) against all other nodes
	//if (CheckCollision(rootNode) == true)
	//{
	//	// if collision restore state
	//	m_yangle = old_yangle;

	//	return true;
	//}

	return false;
}

bool Scene_Node::UpdateZangle(float angle, Scene_Node* rootNode)
{
	float old_zangle = m_zangle;
	m_zangle += angle;

	m_dx = sinf(XMConvertToRadians(m_yangle));
	m_dy = atan(XMConvertToRadians(m_xangle));
	m_dz = cosf(XMConvertToRadians(m_yangle));

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	//rootNode->UpdateCollisionTree(&identity, 1.0f);

	// check for collision of this node (and children) against all other nodes
	//if (CheckCollision(rootNode) == true)
	//{
	//	// if collision restore state
	//	m_zangle = old_zangle;

	//	return true;
	//}

	return false;
}

bool Scene_Node::MoveForward(float speed, Scene_Node* rootNode)
{
	float old_z = m_z;	// save current state 
	float old_x = m_x;

	m_x += m_dx * speed;
	m_y += -m_dy * speed;
	m_z += m_dz * speed;

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	//rootNode->UpdateCollisionTree(&identity, 1.0f);

	// check for collision of this node (and children) against all other nodes
	//if (CheckCollision(rootNode) == true)
	//{
	//	// if collision restore state
	//	m_z = old_z;
	//	m_x = old_x;

	//	return true;
	//}

	return false;
}

void Scene_Node::SetModel(Mesh * mesh)
{
	m_pModel = mesh;
}

void Scene_Node::AddChildNode(Scene_Node * n)
{
	m_children.push_back(n);
}

bool Scene_Node::DetachNode(Scene_Node * n)
{
	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		if (n == m_children[i])
		{
			m_children.erase(m_children.begin() + i);
			return true;
		}

		if (m_children[i]->DetachNode(n) == true) return true;
	}
	return false;
}

void Scene_Node::Execute(XMMATRIX * world, XMMATRIX * view, XMMATRIX * projection)
{
	//m_dx = sinf(XMConvertToRadians(m_yangle));
	//m_dy = atan(XMConvertToRadians(m_xangle));
	//m_dz = cosf(XMConvertToRadians(m_yangle));
	
	//XMMATRIX local_world = DirectX::XMMatrixIdentity();

	m_local_world_matrix = DirectX::XMMatrixRotationX(XMConvertToRadians(m_xangle));
	m_local_world_matrix *= DirectX::XMMatrixRotationY(XMConvertToRadians(m_yangle));
	m_local_world_matrix *= DirectX::XMMatrixRotationZ(XMConvertToRadians(m_zangle));

	m_local_world_matrix *= DirectX::XMMatrixScaling(m_scale, m_scale, m_scale);

	m_local_world_matrix *= DirectX::XMMatrixTranslation(m_x + m_dx, m_y + m_dy, m_z + m_dy);

	//passed in world matrix contains concatenated transformations of all 
	//parent nodes so that this nodes transformations are relative to those
	m_local_world_matrix *= *world;

	if (m_canDraw == true)
	{
		// only draw if there is a model attached
		if (m_pModel) m_pModel->Draw(&m_local_world_matrix, view, projection);
	}
	

	// traverse all child nodes, passing in the concatenated world matrix
	for (unsigned int i = 0; i< m_children.size(); i++)
	{
		m_children[i]->Execute(&m_local_world_matrix, view, projection);
	}

	//UpdateCollisionTree(world, m_scale);

}

//bool Scene_Node::CheckCollision(std::vector<Tile*> tileMap)
//{
//	//check to see if root of tree being compared is same as root node of     
//	//object tree being checked
//	// i.e. stop object node and children being checked against each other
//	//if (objectTreeRoot == compareTree) return false;
//
//	for (unsigned int i = 0; i < tileMap.size(); i++)
//	{
//		if (tileMap[i]->m_canCollide == false)
//		{
//			return false;
//		}
//
//		if (m_pModel && tileMap[i]->m_pModel)
//		{
//
//			XMFLOAT3 box1Pos = XMFLOAT3(m_x - (m_width / 2), m_y - (m_height / 2), m_z - (m_depth / 2));
//			XMFLOAT3 box1Param = XMFLOAT3(this->GetWidth(), this->GetHeight(), this->GetDepth());
//			XMFLOAT3 box2Pos = XMFLOAT3(tileMap[i]->GetXPos(), tileMap[i]->GetYPos(), tileMap[i]->GetZPos());
//			XMFLOAT3 box2Param = XMFLOAT3(tileMap[i]->GetWidth(), tileMap[i]->GetHeight(), tileMap[i]->GetDepth());
//
//			box2Pos.x = box2Pos.x - (box2Param.x / 2);
//			box2Pos.y = box2Pos.y - (box2Param.y / 2);
//			box2Pos.z = box2Pos.z - (box2Param.z / 2);
//
//			if ((box1Pos.x < box2Pos.x + box2Param.x) && (box1Pos.x + box1Param.x > box2Pos.x) && (box1Pos.y < box2Pos.y + box2Param.y) && (box1Param.y + box1Pos.y > box2Pos.y) && (box1Pos.z < box2Pos.z + box2Param.z) && (box1Param.z + box1Pos.z > box2Pos.z))
//			{
//				return true;
//			}
//		}
//	}
//
//
//	return false;
//}

void Scene_Node::setCanCollide(bool canCollide)
{
	m_canCollide = canCollide;
}

void Scene_Node::setCanDraw(bool canDraw)
{
	m_canDraw = canDraw;
}
