/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_2D.cpp
* Description : Simulates all 2D physics for the application
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "PhysicsWorld_2D.h"

PhysicsWorld_2D::PhysicsWorld_2D()
{
}

PhysicsWorld_2D::~PhysicsWorld_2D()
{
	ReleasePtr(m_pWorld);
}

bool PhysicsWorld_2D::Initialise(float _pixelsPerMeter)
{
	m_pixelsPerMeter = _pixelsPerMeter;

	b2Vec2 gravity(0.0f, 9.81f);
	m_allowSleep = true;

	m_timeStep = 1.0f / 60.0f ;
	m_velocityIterations = 6;
	m_posIterations = 2;
	
	// Create a new World
	m_pWorld = new b2World(gravity);

	if (m_pWorld == 0)
	{
		return false;
	}
	m_pWorld->SetAllowSleeping(m_allowSleep);

	b2ContactListener* pBreakListener = new Physics_Break_Listener();
	m_pWorld->SetContactListener(pBreakListener);

	return true;
}

void PhysicsWorld_2D::Process()
{
	m_pWorld->Step(m_timeStep, m_velocityIterations, m_posIterations);
}

Physics_Body_2D* PhysicsWorld_2D::CreatePhysicsObject(TPhysicsProperties _props)
{
	if (_props.circleObject == true)
	{
		return (CreatePhysicsCircle(_props));
	}
	else
	{
		return (CreatePhysicsPolygon(_props));
	}
}

Physics_Body_2D* PhysicsWorld_2D::CreatePhysicsPolygon(TPhysicsProperties _props)
{
	b2Body* body;
	b2Vec2* pPts = new b2Vec2[_props.size];
	v2float convertPos = _props.pos / m_pixelsPerMeter;

	for (UINT i = 0; i < _props.size; i++)
	{
		(pPts[i]) = { _props.pPoints[i].x / m_pixelsPerMeter, _props.pPoints[i].y / m_pixelsPerMeter };
	}

	b2FixtureDef fixtureDef;
	b2BodyDef bodyDef;
	b2PolygonShape shape;

	if (_props.collisionType == CT_BACKGROUND || _props.collisionType == CT_STATIC)
	{
		// Static Body
		bodyDef.type = b2_staticBody;
		fixtureDef.density = 0;
	}
	else
	{
		// Dynamic Body
		bodyDef.type = b2_dynamicBody;
		fixtureDef.density = _props.density;
	}
	bodyDef.position.Set(convertPos.x, convertPos.y);
	bodyDef.angle = _props.angle;
	body = m_pWorld->CreateBody(&bodyDef);

	shape.Set(pPts, _props.size);

	fixtureDef.shape = &shape;
	fixtureDef.friction = _props.friction;
	fixtureDef.restitution = _props.restitution;
	fixtureDef.filter.categoryBits = _props.collisionType;
	fixtureDef.filter.maskBits = _props.collideWith;
	body->CreateFixture(&fixtureDef);

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(body, _props.pPoints, _props.size, m_pixelsPerMeter);
	body->SetUserData(pPhysicsBody);

	ReleasePtr(pPts);
	return pPhysicsBody;
}

Physics_Body_2D* PhysicsWorld_2D::CreatePhysicsCircle(TPhysicsProperties _props)
{
	b2Body* body;
	v2float convertPos = _props.pos / m_pixelsPerMeter;
	float convertRadius = _props.radius / m_pixelsPerMeter;

	b2BodyDef bodyDef;
	b2CircleShape shape;
	b2FixtureDef fixtureDef;

	if (_props.collisionType == CT_BACKGROUND || _props.collisionType == CT_STATIC)
	{
		// Static Body
		bodyDef.type = b2_staticBody;
		fixtureDef.density = 0;
	}
	else
	{
		// Dynamic Body
		bodyDef.type = b2_dynamicBody;
		fixtureDef.density = _props.density;
	}
	bodyDef.position.Set(convertPos.x, convertPos.y);
	bodyDef.angle = _props.angle;
	body = m_pWorld->CreateBody(&bodyDef);

	shape.m_p.Set(0, 0);
	shape.m_radius = convertRadius;

	fixtureDef.shape = &shape;
	fixtureDef.friction = _props.friction;
	fixtureDef.filter.categoryBits = _props.collisionType;
	fixtureDef.filter.maskBits = _props.collideWith;
	fixtureDef.restitution = _props.restitution;
	body->CreateFixture(&fixtureDef);

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(body, convertRadius, m_pixelsPerMeter);
	body->SetUserData(pPhysicsBody);
	return pPhysicsBody;
}

void PhysicsWorld_2D::CreateRevoluteJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _jointPos, bool _collide)
{
	// Convert the input position into a usable b2Vec2
	b2Vec2 b2jointPos = { _jointPos.x / m_pixelsPerMeter, _jointPos.y / m_pixelsPerMeter };

	// Initialise the Joint
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(_bodyA->GetBody(), _bodyB->GetBody(), b2jointPos);
	jointDef.collideConnected = _collide;

	// Create the Joint
	m_pWorld->CreateJoint(&jointDef);
}

//Physics_Joint_2D* PhysicsWorld_2D::CreateRopeJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _relativeAnchorA, v2float _relativeAnchorB, bool _collide)
//{
//	b2RopeJointDef distJoint;
//	b2Vec2 p1, p2, d;
//
//	distJoint.collideConnected = _collide;
//
//	distJoint.bodyA = _bodyA->GetBody();
//	distJoint.bodyB = _bodyB->GetBody();
//	distJoint.localAnchorA.Set(_relativeAnchorA.x / m_pixelsPerMeter, _relativeAnchorA.y / m_pixelsPerMeter);
//	distJoint.localAnchorB.Set(_relativeAnchorB.x / m_pixelsPerMeter, _relativeAnchorB.y / m_pixelsPerMeter);
//	p1 = distJoint.bodyA->GetWorldPoint(distJoint.localAnchorA);
//	p2 = distJoint.bodyB->GetWorldPoint(distJoint.localAnchorB);
//	d = (p2 - p1);
//	distJoint.maxLength = d.Length();
//	b2Joint* pJoint = m_pWorld->CreateJoint(&distJoint);
//
//	Physics_Joint_2D* pPhysicsJoint = new Physics_Joint_2D(pJoint, m_pixelsPerMeter);
//	return pPhysicsJoint;
//}

Physics_Rope_2D* PhysicsWorld_2D::CreateRope(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _relativeAnchorA, v2float _relativeAnchorB, COLORREF _color)
{
	std::vector<Physics_Body_2D*>* ropeBodies = new std::vector<Physics_Body_2D*>;

	b2Vec2 relativeAnchorA = { _relativeAnchorA.x, _relativeAnchorA.y };
	b2Vec2 worldAnchorA = _bodyA->GetBody()->GetWorldPoint(relativeAnchorA);
	b2Vec2 relativeAnchorB = { _relativeAnchorB.x, _relativeAnchorB.y };
	b2Vec2 worldAnchorB = _bodyB->GetBody()->GetWorldPoint(relativeAnchorB);

	float distance = (worldAnchorA - worldAnchorB).Length() / m_pixelsPerMeter;
	int ropeLinks = 20;

	v2float position = { worldAnchorA.x / m_pixelsPerMeter, worldAnchorA.y / m_pixelsPerMeter };
	Physics_Body_2D* prevBody = _bodyA;
	for (int i = 0; i < ropeLinks; i++)
	{
		TPhysicsProperties physProps;
		v2float* pPoints = new v2float[4];
		pPoints[0] = { -0.5, -(distance / ropeLinks) / 2.0f };
		pPoints[1] = { 0.5, -(distance / ropeLinks) / 2.0f };
		pPoints[2] = { 0.5, (distance / ropeLinks) / 2.0f };
		pPoints[3] = { -0.5, (distance / ropeLinks) / 2.0f };
		ZeroMemory(&physProps, sizeof(physProps));
		physProps.circleObject = false;
		physProps.pPoints = pPoints;
		physProps.size = 4;
		physProps.pos = position;
		physProps.density = 1.0f;
		physProps.collisionType = CT_DYNAMIC;
		physProps.collideWith = 0;
		Physics_Body_2D* pTempBody = CreatePhysicsObject(physProps);
		ropeBodies->push_back(pTempBody);

		CreateRevoluteJoint(prevBody, pTempBody, position.x + (distance / ropeLinks), false);

		position.x += (distance / ropeLinks);
		prevBody = pTempBody;
	}

	v2float anchorB = { worldAnchorB.x / m_pixelsPerMeter, worldAnchorB.y / m_pixelsPerMeter };
	CreateRevoluteJoint(prevBody, _bodyB, anchorB, false);

	return 0;
}

std::vector<Physics_Body_2D*>* PhysicsWorld_2D::BreakObject(Physics_Body_2D* _body)
{
	v2float* points = _body->GetPoints();
	UINT size = _body->GetSize();

	v2float collisionPoint = _body->GetBreakProperties()->pCollisionWorldPoints[0];
	v2float impactVelocity = _body->GetBreakProperties()->impactVelocity;


	return 0;
}





