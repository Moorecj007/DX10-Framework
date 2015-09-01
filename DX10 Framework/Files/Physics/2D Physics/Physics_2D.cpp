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
#include "Physics_2D.h"

Physics_2D::Physics_2D()
{
}

Physics_2D::~Physics_2D()
{
	ReleasePtr(m_pWorld);
}

bool Physics_2D::Initialise(float _pixelsPerMeter)
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

	return true;
}

void Physics_2D::Process()
{
	m_pWorld->Step(m_timeStep, m_velocityIterations, m_posIterations);
}

Physics_Body_2D* Physics_2D::CreatePhysicsObject_Quad(TPhysicsProperties _props)
{
	b2Body* body;
	v2float convertPos = _props.pos / m_pixelsPerMeter;
	v2float convertScale = _props.scale / m_pixelsPerMeter;

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

	shape.SetAsBox((convertScale.x / 2.0f), (convertScale.y / 2.0f));

	fixtureDef.shape = &shape;
	fixtureDef.friction = _props.friction;
	fixtureDef.restitution = _props.restitution;
	fixtureDef.filter.categoryBits = _props.collisionType;
	fixtureDef.filter.maskBits = _props.collideWith;
	body->CreateFixture(&fixtureDef);

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(body, convertScale, m_pixelsPerMeter);
	return pPhysicsBody;
}

Physics_Body_2D* Physics_2D::CreatePhysicsPolygon(TPhysicsProperties _props)
{
	b2Body* body;

	b2Vec2* pPts = new b2Vec2[_props.size];
	v2float convertPos = _props.pos / m_pixelsPerMeter;

	for (UINT i = 0; i < _props.size; i++)
	{
		(pPts[i]) = { _props.pPoints[i].x / m_pixelsPerMeter, _props.pPoints[i].y / m_pixelsPerMeter };
		//_props.pPoints[i] = { _props.pPoints[i].x + _props.pos.x, _props.pPoints[i].y + _props.pos.y };
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

	ReleasePtr(pPts);
	return pPhysicsBody;
}

Physics_Body_2D* Physics_2D::CreatePhysicsObject_Tri(TPhysicsProperties _props)
{
	b2Body* body;

	b2Vec2 b2Pts[3];
	v2float convertPos = _props.pos / m_pixelsPerMeter;
	v2float convertScale = _props.scale / m_pixelsPerMeter;

	// Bottom Left point
	b2Pts[0].x = (convertPos.x - (convertScale.x / 2.0f)) - convertPos.x;
	b2Pts[0].y = (convertPos.y + (convertScale.y / 2.0f)) - convertPos.y;
							
	// Top Middle point		
	b2Pts[1].x = 0.0f;
	b2Pts[1].y = (convertPos.y - (convertScale.y / 2.0f)) - convertPos.y;
							
	// Bottom Right point	
	b2Pts[2].x = (convertPos.x + (convertScale.x / 2.0f)) - convertPos.x;
	b2Pts[2].y = (convertPos.y + (convertScale.y / 2.0f)) - convertPos.y;

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

	shape.Set(b2Pts, 3);

	fixtureDef.shape = &shape;
	fixtureDef.friction = _props.friction;
	fixtureDef.restitution = _props.restitution;
	fixtureDef.filter.categoryBits = _props.collisionType;
	fixtureDef.filter.maskBits = _props.collideWith;
	body->CreateFixture(&fixtureDef);

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(body, convertScale, m_pixelsPerMeter);
	return pPhysicsBody;
}

Physics_Body_2D* Physics_2D::CreatePhysicsObject_Circle(TPhysicsProperties _props)
{
	b2Body* body;
	v2float convertPos = _props.pos / m_pixelsPerMeter;
	v2float convertScale = _props.scale / m_pixelsPerMeter;

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
	shape.m_radius = convertScale.x / 2.0f;

	fixtureDef.shape = &shape;
	fixtureDef.friction = _props.friction;
	fixtureDef.filter.categoryBits = _props.collisionType;
	fixtureDef.filter.maskBits = _props.collideWith;
	fixtureDef.restitution = _props.restitution;
	body->CreateFixture(&fixtureDef);

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(body, convertScale, m_pixelsPerMeter);
	return pPhysicsBody;
}

void Physics_2D::CreateRevoluteJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _jointPos, bool _collide)
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

Physics_Joint_2D* Physics_2D::CreateRopeJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _relativeAnchorA, v2float _relativeAnchorB, bool _collide)
{
	b2RopeJointDef distJoint;
	b2Vec2 p1, p2, d;

	distJoint.collideConnected = _collide;

	distJoint.bodyA = _bodyA->GetBody();
	distJoint.bodyB = _bodyB->GetBody();
	distJoint.localAnchorA.Set(_relativeAnchorA.x / m_pixelsPerMeter, _relativeAnchorA.y / m_pixelsPerMeter);
	distJoint.localAnchorB.Set(_relativeAnchorB.x / m_pixelsPerMeter, _relativeAnchorB.y / m_pixelsPerMeter);
	p1 = distJoint.bodyA->GetWorldPoint(distJoint.localAnchorA);
	p2 = distJoint.bodyB->GetWorldPoint(distJoint.localAnchorB);
	d = (p2 - p1);
	distJoint.maxLength = d.Length();
	b2Joint* pJoint = m_pWorld->CreateJoint(&distJoint);

	Physics_Joint_2D* pPhysicsJoint = new Physics_Joint_2D(pJoint, m_pixelsPerMeter);
	return pPhysicsJoint;
}