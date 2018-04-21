/*----------------------------------------------
Header
Programmer: Alberto Bobadilla (labigm@gmail.com)

Source
Programmer: Nick Stokowski (nks4601@rit.edu)

Date: 2017/07
----------------------------------------------*/
#ifndef __MYOCTREE_H_
#define __MYOCTREE_H_

#include "MyEntityManager.h"

namespace Simplex
{

//System Class
class MyOctree
{
	static uint m_uOctantCount; //will store the number of octants instantiated
	static uint m_uMaxLevel;//will store the maximum level an octant can go to
	static uint m_uIdealEntityCount; //will tell how many ideal Entities this object will contain

	uint m_uID = 0; //Will store the current ID for this octant
	uint m_uLevel = 0; //Will store the current level of the octant
	uint m_uChildren = 0;// Number of children on the octant (either 0 or 8)

	float m_fSize = 0.0f; //Size of the octant

	MeshManager* m_pMeshMngr = nullptr;//Mesh Manager singleton
	MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton

	vector3 m_v3Center = vector3(0.0f); //Will store the center point of the octant
	vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the octant
	vector3 m_v3Max = vector3(0.0f); //Will store the maximum vector of the octant
	
	MyOctree* m_pParent = nullptr;// Will store the parent of current octant
	MyOctree* m_pChild[8];//Will store the children of the current octant

	std::vector<uint> m_EntityList; //List of Entities under this octant (Index in Entity Manager)

	MyOctree* m_pRoot = nullptr;//Root octant
	std::vector<MyOctree*> m_lChild; //list of nodes that contain objects (this will be applied to root only)
	
public:
	/*
	USAGE: Constructor, will create an octant containing all MagnaEntities Instances in the Mesh 
	manager	currently contains
	ARGUMENTS:
	- uint a_nMaxLevel = 2 -> Sets the maximum level of subdivision
	- uint nIdealEntityCount = 5 -> Sets the ideal level of objects per octant
	OUTPUT: class object
	*/
	MyOctree(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);
	/*
	USAGE: Constructor
	ARGUMENTS:
	- vector3 a_v3Center -> Center of the octant in global space
	- float a_fSize -> size of each side of the octant volume
	OUTPUT: class object
	*/
	MyOctree(vector3 a_v3Center, float a_fSize);
	/*
	USAGE: Destructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~MyOctree(void);
	/*
	USAGE: Gets this octant's size
	ARGUMENTS: ---
	OUTPUT: size of octant
	*/
	float GetSize(void);
	/*
	USAGE: Gets the center of the octant in global scape
	ARGUMENTS: ---
	OUTPUT: Center of the octant in global space
	*/
	vector3 GetCenterGlobal(void);
	/*
	USAGE: Gets the min corner of the octant in global space
	ARGUMENTS: ---
	OUTPUT: Minimum in global space
	*/
	vector3 GetMinGlobal(void);
	/*
	USAGE: Gets the max corner of the octant in global space
	ARGUMENTS: ---
	OUTPUT: Maximum in global space
	*/
	vector3 GetMaxGlobal(void);
	/*
	USAGE: Asks if there is a collision with the Entity specified by index from
	the Bounding Object Manager
	ARGUMENTS:
	- int a_uRBIndex -> Index of the Entity in the Entity Manager
	OUTPUT: check of the collision
	*/
	bool IsColliding(uint a_uRBIndex);
	/*
	USAGE: Displays the MyOctree volume specified by index including the objects underneath
	ARGUMENTS:
	- uint a_nIndex -> MyOctree to be displayed.
	- vector3 a_v3Color = REYELLOW -> Color of the volume to display.
	OUTPUT: ---
	*/
	void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);
	/*
	USAGE: Displays the MyOctree volume in the color specified
	ARGUMENTS:
	- vector3 a_v3Color = REYELLOW -> Color of the volume to display.
	OUTPUT: ---
	*/
	void Display(vector3 a_v3Color = C_YELLOW);
	/*
	USAGE: Displays the non empty leafs in the octree
	ARGUMENTS:
	- vector3 a_v3Color = REYELLOW -> Color of the volume to display.
	OUTPUT: ---
	*/
	void DisplayLeafs(vector3 a_v3Color = C_YELLOW);
	/*
	USAGE: Clears the Entity list for each node
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ClearEntityList(void);
	/*
	USAGE: allocates 8 smaller octants in the child pointers
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Subdivide(void);
	/*
	USAGE: returns the child specified in the index
	ARGUMENTS: uint a_nChild -> index of the child (from 0 to 7)
	OUTPUT: MyOctree object (child in index)
	*/
	MyOctree* GetChild(uint a_nChild);
	/*
	USAGE: returns the parent of the octant
	ARGUMENTS: ---
	OUTPUT: MyOctree object (parent)
	*/
	MyOctree* GetParent(void);
	/*
	USAGE: Asks the MyOctree if it does not contain any children (its a leaf)
	ARGUMENTS: ---
	OUTPUT: It contains no children
	*/
	bool IsLeaf(void);
	/*
	USAGE: Asks the MyOctree if it contains more than this many Bounding Objects
	ARGUMENTS:
	- uint a_nEntities -> Number of Entities to query
	OUTPUT: It contains at least this many Entities
	*/
	bool ContainsMoreThan(uint a_nEntities);
	/*
	USAGE: Deletes all children and the children of their children (almost sounds apocalyptic)
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void KillBranches(void);
	/*
	USAGE: Creates a tree using subdivisions, the max number of objects and levels
	ARGUMENTS: 
	- uint a_nMaxLevel = 3 -> Sets the maximum level of the tree while constructing it
	OUTPUT: ---
	*/
	void ConstructTree(uint a_nMaxLevel = 3);

	// Construct a child octant during subdivision
	MyOctree* ConstructChildOct(vector3 center, float size, uint level);

	/*
	USAGE: Traverse the tree up to the leafs and sets the objects in them to the index
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void AssignIDtoEntity(void);

	/*
	USAGE: Gets the total number of octants in the world
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	uint GetOctantCount(void);

private:
	/*
	USAGE: Deallocates member fields
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Release(void);
	/*
	USAGE: Allocates member fields
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Init(void);
	/*
	USAGE: creates the list of all leafs that contains objects.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ConstructList(void);
};//class

} //namespace Simplex

#endif //__OCTREE_H_