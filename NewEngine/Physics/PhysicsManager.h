#pragma once

/**
 * Handles physical representation of the world and its object
 * Code adapted from YouTube bullet physics tutorial series (https://pastebin.com/Xxv5gBLH)
 */

#include <vector>
#include <iostream>
#include <algorithm>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "PhysicsObject.h"
#include "PhysicsPlayer.h"

using std::vector;
using std::find;
using std::distance;

class PhysicsManager{
	public:
		PhysicsManager();
		~PhysicsManager();

		//Timestep function
		inline void UpdatePhysics(float msec){
			world->stepSimulation(1 / 90.0f);
			world->performDiscreteCollisionDetection();


			//For each contact manifold
			for(int i = 0; i < world->getDispatcher()->getNumManifolds(); i++){
				btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
				btCollisionObject *obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
				btCollisionObject *obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

				//Ignore floor collisions
				if(obA->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE
					|| obB->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE){
					continue;
				}
				
				if((PhysicsObject*)obA->getUserPointer() != nullptr && (PhysicsObject*)obB->getUserPointer() != nullptr){
					((PhysicsObject*)obA->getUserPointer())->HandleHit((PhysicsObject*)obB->getUserPointer());
					((PhysicsObject*)obB->getUserPointer())->HandleHit((PhysicsObject*)obA->getUserPointer());
				}
			}

			//Update physics player
			if(player){
				player->Tick();
			}
		}

		/* Add new physical representation of game object */
		btRigidBody* AddSphere(float radius, float x, float y, float z, float mass);
		btRigidBody* AddPlane(float x, float y, float z, float mass);
		btRigidBody* AddCylinder(float radius, float height, float x, float y, float z, float mass);
		btRigidBody* AddCone(float radius, float height, float x, float y, float z, float mass);
		btRigidBody* AddBox(float width, float height, float depth, float x, float y, float z, float mass);

		//Collision callback function (not working)
		bool CollisionFunc(btManifoldPoint &collisionPoint, const btCollisionObject *obj1, int id1, int index1, 
			const btCollisionObject *obj2, int id2, int index2);

		inline void ClearPhysicsObjects(){
			for(auto obj : physicsObjects){
				world->removeCollisionObject(obj->GetBody());
			}

			physicsObjects.clear();
		}

		//Adds PhysicsObject to list to be rendered each frame
		inline void AddPhysicsObj(PhysicsObject *obj){
			obj->GetBody()->setUserPointer(obj);
			physicsObjects.push_back(obj);
		}

		inline void RemovePhysicsObj(PhysicsObject *obj){
			//std::cout << "Size: " << physicsObjects.size() << std::endl;
			world->removeCollisionObject(obj->GetBody());

			//Remove object from physics list
			auto it = find(physicsObjects.begin(), physicsObjects.end(), obj);
			if(it != physicsObjects.end()){
				auto index = distance(physicsObjects.begin(), it);
				physicsObjects.erase(it);
			}
		}

		//Add PhysicsPlayer to physics system
		inline void AddPhysicsPlayer(PhysicsPlayer *player){
			this->player = player;

			world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
			this->player->SetGravity(world->getGravity());

			world->addCollisionObject(this->player->GetGhostObj(), btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
			world->addAction(this->player->GetCharController());
		}
		inline void RemovePhysicsPlayer(){
			if(player){
				//world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(nullptr);
				world->removeCollisionObject(player->GetGhostObj());
				world->removeAction(player->GetCharController());
			}
		}

		inline btDynamicsWorld* GetWorld(){
			return world;
		}

	private:
		vector<PhysicsObject*> physicsObjects;
		btDynamicsWorld *world;
		btDispatcher *dispatcher;
		btCollisionConfiguration *collisionConfig;
		btBroadphaseInterface *broadphase;
		btConstraintSolver *solver;
		PhysicsPlayer *player;
};