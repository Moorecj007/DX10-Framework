/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Break_Listener.cpp
* Description : Contact listener that detects objects that need to break
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Break_Listener.h"

Physics_Break_Listener::Physics_Break_Listener()
{
}

Physics_Break_Listener::~Physics_Break_Listener()
{
}

void Physics_Break_Listener::BeginContact(b2Contact* _contact)
{
	// TO DO - COMMENT THE SHIT OUT OF THIS

	if (_contact->GetFixtureA()->GetFilterData().categoryBits == CT_BREAKABLE
		|| _contact->GetFixtureB()->GetFilterData().categoryBits == CT_BREAKABLE)
	{
		int numPoints = _contact->GetManifold()->pointCount;
		b2Vec2* collisionPoint;

		b2WorldManifold worldmanifold;
		_contact->GetWorldManifold(&worldmanifold);
		collisionPoint = worldmanifold.points;

		v2float* pPoints = new v2float[numPoints];
		for (int i = 0; i < numPoints; i++)
		{
			pPoints[i] = { collisionPoint[i].x, collisionPoint[i].y };
		}

		b2Vec2 velocityA = _contact->GetFixtureA()->GetBody()->GetLinearVelocityFromWorldPoint(collisionPoint[0]);
		b2Vec2 velocityB = _contact->GetFixtureB()->GetBody()->GetLinearVelocityFromWorldPoint(collisionPoint[0]);

		Physics_Body_2D* pPhysicsBody = 0;
		v2float impactVelocity;


		if (_contact->GetFixtureA()->GetFilterData().categoryBits == CT_BREAKABLE)
		{
			b2Vec2 b2ImpactVelocity = velocityA - velocityB;
			impactVelocity = { b2ImpactVelocity.x, b2ImpactVelocity.y };

			pPhysicsBody = (Physics_Body_2D*)(_contact->GetFixtureA()->GetBody()->GetUserData());
		}

		if (_contact->GetFixtureB()->GetFilterData().categoryBits == CT_BREAKABLE)
		{
			b2Vec2 b2ImpactVelocity = velocityB - velocityA;
			impactVelocity = { b2ImpactVelocity.x, b2ImpactVelocity.y };

			pPhysicsBody = (Physics_Body_2D*)(_contact->GetFixtureB()->GetBody()->GetUserData());
		}
		TBreakProperties breakProps;
		breakProps.broken = true;
		breakProps.pCollisionWorldPoints = pPoints;
		breakProps.impactVelocity = impactVelocity;
		pPhysicsBody->SetBreakProperties(breakProps);
	}
}

void Physics_Break_Listener::EndContact(b2Contact* _contact)
{
	
}

void Physics_Break_Listener::PreSolve(b2Contact* _contact, const b2Manifold* _oldManifold)
{

}

void Physics_Break_Listener::PostSolve(b2Contact* _contact, const b2ContactImpulse* _impulse)
{

}
