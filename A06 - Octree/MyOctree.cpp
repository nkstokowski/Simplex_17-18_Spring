#include "MyOctree.h"

using namespace Simplex;

uint MyOctree::m_uOctantCount = 0;
uint MyOctree::m_uMaxLevel = 0;
uint MyOctree::m_uIdealEntityCount = 0;

// Getters
float MyOctree::GetSize(void){ return m_fSize; }
vector3 MyOctree::GetMinGlobal(void){ return m_v3Min; }
vector3 MyOctree::GetMaxGlobal(void){ return m_v3Max; }
MyOctree * MyOctree::GetParent(void) { return m_pParent; }
vector3 MyOctree::GetCenterGlobal(void) { return m_v3Center; }
uint MyOctree::GetOctantCount(void) { return m_uOctantCount; }
MyOctree * MyOctree::GetChild(uint a_nChild)
{
	if (a_nChild >= 0 || a_nChild <= 7 || m_uChildren != 0)
	{
		return m_pChild[a_nChild];
	}

	return nullptr;
}


// Construction / Destruction
MyOctree::MyOctree(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();

	// Set args
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	// Actions for root
	m_uID = m_uOctantCount;
	m_pRoot = this;
	m_lChild.clear();
	m_uOctantCount = 1;

	// Use a rigidbody to get root octant bounds
	MyRigidBody* rootOctantRB;
	MyRigidBody* entityRB;
	std::vector<vector3> rbPointsList;

	// Build list of all min and max points
	for (uint i=0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		entityRB = m_pEntityMngr->GetEntity(i)->GetRigidBody();
		rbPointsList.push_back(entityRB->GetMaxGlobal());
		rbPointsList.push_back(entityRB->GetMinGlobal());
	}

	// Build rigidbody from list
	rootOctantRB = new MyRigidBody(rbPointsList);

	// Set boundries
	m_v3Center = rootOctantRB->GetCenterLocal();

	vector3 halfWidth = rootOctantRB->GetHalfWidth();
	m_v3Max = m_v3Center + halfWidth;
	m_v3Min = m_v3Center - halfWidth;
	m_fSize = 2.0f * std::max(halfWidth.x, std::max(halfWidth.y, halfWidth.z));

	delete rootOctantRB;
	ConstructTree(m_uMaxLevel);
}
MyOctree::MyOctree(vector3 a_v3Center, float a_fSize)
{
	Init();
	
	// Set args
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	// Set bounding region
	vector3 offset = vector3(m_fSize / 2.0f);
	m_v3Min = m_v3Center - offset;
	m_v3Max = m_v3Center + offset;

	m_uOctantCount++;
}
MyOctree::~MyOctree(void)
{
	Release();
}


// Checks
bool MyOctree::IsColliding(uint a_uRBIndex)
{
	if (a_uRBIndex >= m_pEntityMngr->GetEntityCount()) {
		return false;
	}

	// Try to get entity
	MyEntity* entitiy = m_pEntityMngr->GetEntity(a_uRBIndex);
	if (!entitiy) {
		return false;  // Failed to find entity
	}

	MyRigidBody* rb = entitiy->GetRigidBody();
	if (!rb) {
		return false; // Failed to find rigidbody
	}

	vector3 otherMin = rb->GetMinGlobal();
	vector3 otherMax = rb->GetMaxGlobal();

	if (m_v3Max.x < otherMin.x || m_v3Min.x > otherMax.x)
	{
		return false;
	}
	if (m_v3Max.y < otherMin.y || m_v3Min.y > otherMax.y)
	{
		return false;
	}
	if (m_v3Max.z < otherMin.z || m_v3Min.z > otherMax.z)
	{
		return false;
	}

	return true;
}
bool MyOctree::IsLeaf(void)
{
	return (m_uChildren == 0);
}
bool MyOctree::ContainsMoreThan(uint a_nEntities)
{
	int count = 0;
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++) {
		count += IsColliding(i) ? 1 : 0;
	}

	return count > a_nEntities;
}


// Display 
void MyOctree::Display(uint a_nIndex, vector3 a_v3Color)
{
	if (a_nIndex >= m_pRoot->GetOctantCount()) {
		Display(a_v3Color);
	}
	else if (m_uID == a_nIndex)
	{
		matrix4 octCube = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
		m_pMeshMngr->AddWireCubeToRenderList(octCube, a_v3Color, RENDER_WIRE);
		//DisplayLeafs(a_v3Color);
	}
	else {
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->Display(a_nIndex, a_v3Color);
		}
	}
}
void MyOctree::Display(vector3 a_v3Color)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color);
	}

	matrix4 octCube = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
	m_pMeshMngr->AddWireCubeToRenderList(octCube, a_v3Color, RENDER_WIRE);
}
void MyOctree::DisplayLeafs(vector3 a_v3Color)
{
	for (uint i = 0; i < m_lChild.size(); i++)
	{
		m_lChild[i]->DisplayLeafs(a_v3Color);
	}
	matrix4 octCube = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
	m_pMeshMngr->AddWireCubeToRenderList(octCube, a_v3Color, RENDER_WIRE);
	DisplayLeafs(a_v3Color);
}


// Actions
void MyOctree::Subdivide(void)
{

	float subOctOffset, subOctSize;
	uint nextLevel = m_uLevel + 1;
	vector3 subOctCenter;
		
	subOctOffset = m_fSize / 4.0f;
	subOctSize = m_fSize / 2.0f;
	subOctCenter = m_v3Center + vector3(subOctOffset);

	m_uChildren = 8;

	// Top Four
	m_pChild[0] = ConstructChildOct(subOctCenter, subOctSize, nextLevel);
	subOctCenter.z -= subOctSize;
	m_pChild[1] = ConstructChildOct(subOctCenter, subOctSize, nextLevel);
	subOctCenter.x -= subOctSize;
	m_pChild[2] = ConstructChildOct(subOctCenter, subOctSize, nextLevel);
	subOctCenter.z += subOctSize;
	m_pChild[3] = ConstructChildOct(subOctCenter, subOctSize, nextLevel);


	// Bottom Four
	subOctCenter.y -= subOctSize;

	m_pChild[4] = ConstructChildOct(subOctCenter, subOctSize, nextLevel);
	subOctCenter.x += subOctSize;
	m_pChild[5] = ConstructChildOct(subOctCenter, subOctSize, nextLevel);
	subOctCenter.z -= subOctSize;
	m_pChild[6] = ConstructChildOct(subOctCenter, subOctSize, nextLevel);
	subOctCenter.x -= subOctSize;
	m_pChild[7] = ConstructChildOct(subOctCenter, subOctSize, nextLevel);

}
void MyOctree::AssignIDtoEntity(void)
{
	// Add all intersecting entities
	if (m_uChildren == 0) {
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			if (IsColliding(i))
			{
				m_pEntityMngr->AddDimension(i, m_uID);
				m_EntityList.push_back(i);
			}
		}
	}
	else {
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->AssignIDtoEntity();
		}
	}
}
void MyOctree::ConstructList(void)
{
	// If non empty, add to the root master list
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}

	// Recurse over children
	for (uint i=0; i < m_uChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}
}
void MyOctree::ConstructTree(uint a_nMaxLevel)
{

	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;

	if (ContainsMoreThan(m_uIdealEntityCount) && m_uLevel < m_uMaxLevel)
	{
		Subdivide();
	}

	AssignIDtoEntity();
	ConstructList();
}
MyOctree* MyOctree::ConstructChildOct(vector3 center, float size, uint level)
{
	MyOctree* result = new MyOctree(center, size);
	result->m_pParent = this;
	result->m_pRoot = m_pRoot;
	result->m_uLevel = level;

	// Subdivide if needed / able
	if (result->ContainsMoreThan(m_uIdealEntityCount) && level < m_uMaxLevel)
	{
		result->Subdivide();
	}

	return result;
}


// Initalization / Cleanup 
void MyOctree::ClearEntityList(void)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_lChild[i]->ClearEntityList();
	}

	m_EntityList.clear();
}
void MyOctree::Init(void)
{
	// Get singletons
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	// Set id
	m_uID = m_uOctantCount;

	// prepare child array
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}
void MyOctree::Release(void)
{
	// Clear lists
	m_EntityList.clear();
	m_lChild.clear();

	// If root kill branches
	if (m_uLevel == 0)
	{
		KillBranches();
	}
}
void MyOctree::KillBranches(void)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		if (m_pChild) {
			m_pChild[i]->KillBranches();
			SafeDelete(m_pChild[i]);
		}
	}
	m_uChildren = 0;
}
