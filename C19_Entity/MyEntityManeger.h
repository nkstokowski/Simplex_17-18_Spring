#pragma once

#include "MyEntity.h"
using namespace std;

namespace Simplex
{

	class MyEntityManeger
	{
	private:
		static MyEntityManeger* m_pInstance; // Singleton pointer
											 //Rule of 3
											 /*
											 USAGE: Constructor
											 ARGUMENTS: ---
											 OUTPUT:
											 */
		MyEntityManeger(void);
		/*
		USAGE: Copy Constructor
		ARGUMENTS:
		OUTPUT:
		*/
		MyEntityManeger(MyEntityManeger const& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS:
		OUTPUT:
		*/
		MyEntityManeger& operator=(MyEntityManeger const& other);
		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~MyEntityManeger(void);
		//Other methods
		/*
		USAGE: Initializes the objects fields
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Init(void);
		/*
		USAGE: Releases the objects memory
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Release(void);

	public:
		vector<MyEntity*> entities;


		//Singleton Methods
		/*
		USAGE: Gets/Constructs the singleton pointer
		ARGUMENTS:
		OUTPUT:
		*/
		static MyEntityManeger* GetInstance();
		/*
		USAGE: Destroys the singleton
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		static void ReleaseInstance(void);

		// Add entity
		void AddEntity(String a_sFileName, String a_sUniqueID = "NA");

		// Set entity
		void SetEntity(String a_sFileName, String a_sUniqueID);

		// Get entity
		MyEntity* GetEntity(String a_sUniqueID);

		// Remove entity
		void RemoveEntity(String a_sUniqueID);

		// Check specific collision
		bool CheckCollision(String a_sFirstID, String a_sSecondID);

		Model * GetModel(String a_sUniqueID);

		RigidBody * GetRigidBody(String a_sUniqueID);

		matrix4 GetModelMatrix(String a_sUniqueID);

		void SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID);

		void SetAxisVisibility(bool a_bVisibility, String a_sUniqueID);



		

	};

}