#include "MyEntityManeger.h"

/*
Programmer: Nick Stokowski
Programmer: Thomas Tabacchi
*/

using namespace Simplex;
MyEntityManeger* MyEntityManeger::m_pInstance = nullptr;

MyEntityManeger* MyEntityManeger::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManeger();
	}
	return m_pInstance;
}

void MyEntityManeger::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void Simplex::MyEntityManeger::AddEntity(String a_sFileName, String a_sUniqueID)
{
	MyEntity* newEntity = new MyEntity(a_sFileName, a_sUniqueID);
	entities.push_back(newEntity);
	m_iEntityCount++;
}

void Simplex::MyEntityManeger::SetEntity(String a_sFileName, String a_sUniqueID)
{
	MyEntity* newEntity = new MyEntity(a_sFileName, a_sUniqueID);
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->GetUniqueID() == a_sUniqueID) {
			entities[i] = newEntity;
		}
	}
}

MyEntity* Simplex::MyEntityManeger::GetEntity(String a_sUniqueID)
{
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->GetUniqueID() == a_sUniqueID) {
			return entities[i];
		}
	}

	return nullptr;
}

void Simplex::MyEntityManeger::RemoveEntity(String a_sUniqueID)
{
	for (auto it = entities.begin(); it != entities.end(); it++) {
		if ((*it)->GetUniqueID() == a_sUniqueID) {
			entities.erase(it);
			return;
		}
	}

	m_iEntityCount--;
}

bool Simplex::MyEntityManeger::CheckCollision(String a_sFirstID, String a_sSecondID)
{
	MyEntity* first = GetEntity(a_sFirstID);
	MyEntity* second = GetEntity(a_sSecondID);

	return first->IsColliding(second);
}


void Simplex::MyEntityManeger::CheckAllCollisions()
{
	// Run collision checker for all entities
	for (int i = 0; i < entities.size() - 1; i++)
	{
		for (int j = (i + 1); j < entities.size(); j++)
		{
			entities[i]->IsColliding(entities[j]);
		}
	}
}


// Unique ID Versions
Model* Simplex::MyEntityManeger::GetModel(String a_sUniqueID)
{
		MyEntity* entity = MyEntity::GetEntity(a_sUniqueID);	
		if (entity)
		{
			return entity->GetModel();
		}
		return nullptr;
}

RigidBody* Simplex::MyEntityManeger::GetRigidBody(String a_sUniqueID)
{
	
	MyEntity* entity = MyEntity::GetEntity(a_sUniqueID);

	if (entity)
	{
		return entity->GetRigidBody();
	}
	return nullptr;
}

matrix4 Simplex::MyEntityManeger::GetModelMatrix(String a_sUniqueID)
{
	
	MyEntity* entity = MyEntity::GetEntity(a_sUniqueID);
	if (entity)
	{
		return entity->GetModelMatrix();
	}
	return IDENTITY_M4;
}

void Simplex::MyEntityManeger::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	MyEntity* entity = MyEntity::GetEntity(a_sUniqueID);
	if (entity)
	{
		entity->SetModelMatrix(a_m4ToWorld);
	}
}

void Simplex::MyEntityManeger::SetAxisVisibility(bool a_bVisibility, String a_sUniqueID)
{

	MyEntity* entity = MyEntity::GetEntity(a_sUniqueID);
	if (entity)
	{
		entity->SetAxisVisible(a_bVisibility);
	}
}

void Simplex::MyEntityManeger::renderEntity(String a_sUniqueID, bool drawRigidBody)
{
	MyEntity* entity = GetEntity(a_sUniqueID);
	entity->AddToRenderList(drawRigidBody);
}


// Index versions
Model* Simplex::MyEntityManeger::GetModel(int a_iEntityIndex)
{
	MyEntity* entity = entities[a_iEntityIndex];
	if (entity)
	{
		return entity->GetModel();
	}
	return nullptr;
}

RigidBody* Simplex::MyEntityManeger::GetRigidBody(int a_iEntityIndex)
{

	MyEntity* entity = entities[a_iEntityIndex];

	if (entity)
	{
		return entity->GetRigidBody();
	}
	return nullptr;
}

matrix4 Simplex::MyEntityManeger::GetModelMatrix(int a_iEntityIndex)
{

	MyEntity* entity = entities[a_iEntityIndex];
	if (entity)
	{
		return entity->GetModelMatrix();
	}
	return IDENTITY_M4;
}

void Simplex::MyEntityManeger::SetModelMatrix(matrix4 a_m4ToWorld, int a_iEntityIndex)
{
	MyEntity* entity = entities[a_iEntityIndex];
	if (entity)
	{
		entity->SetModelMatrix(a_m4ToWorld);
	}
}

void Simplex::MyEntityManeger::SetAxisVisibility(bool a_bVisibility, int a_iEntityIndex)
{

	MyEntity* entity = entities[a_iEntityIndex];
	if (entity)
	{
		entity->SetAxisVisible(a_bVisibility);
	}
}

void Simplex::MyEntityManeger::renderEntity(int a_iEntityIndex, bool drawRigidBody)
{
	MyEntity* entity = entities[a_iEntityIndex];
	entity->AddToRenderList(drawRigidBody);
}

//The big 3
MyEntityManeger::MyEntityManeger() { Init(); }
MyEntityManeger::MyEntityManeger(MyEntityManeger const& other) { }
MyEntityManeger& MyEntityManeger::operator=(MyEntityManeger const& other) { return *this; }
MyEntityManeger::~MyEntityManeger() { Release(); }


void Simplex::MyEntityManeger::Init(void)
{
	m_iEntityCount = 0;
	entities.clear();
}
void Simplex::MyEntityManeger::Release(void)
{
	for (int i = 0; i < m_iEntityCount; i++)
	{
		MyEntity* entitiy = entities[i];
		delete entitiy;
	}
	m_iEntityCount = 0;
	entities.clear();
}
;