/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : 2D_Physics.cpp
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

	if (_props.movable == false)
	{
		// Static Body
		b2BodyDef staticBodyDef;
		staticBodyDef.position.Set(convertPos.x, convertPos.y);
		staticBodyDef.angle = _props.angle;
		body = m_pWorld->CreateBody(&staticBodyDef);

		b2PolygonShape staticShape;
		staticShape.SetAsBox((convertScale.x / 2.0f), (convertScale.y / 2.0f));

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &staticShape;
		fixtureDef.density = 0;
		fixtureDef.friction = _props.friction;
		fixtureDef.restitution = _props.restitution;

		body->CreateFixture(&fixtureDef);
	}
	else
	{
		// Dynamic Body
		b2BodyDef dynamicBodyDef;
		dynamicBodyDef.type = b2_dynamicBody;
		dynamicBodyDef.position.Set(convertPos.x, convertPos.y);
		dynamicBodyDef.angle = _props.angle;
		body = m_pWorld->CreateBody(&dynamicBodyDef);

		b2PolygonShape dynamicShape;
		dynamicShape.SetAsBox((convertScale.x / 2.0f), (convertScale.y / 2.0f));

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicShape;
		fixtureDef.density = _props.density;
		fixtureDef.friction = _props.friction;
		fixtureDef.restitution = _props.restitution;

		body->CreateFixture(&fixtureDef);
	}

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(body, convertScale, m_pixelsPerMeter);
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

	if (_props.movable == false)
	{
		// Static Body
		b2BodyDef staticBodyDef;
		staticBodyDef.position.Set(convertPos.x, convertPos.y);
		staticBodyDef.angle = _props.angle;
		body = m_pWorld->CreateBody(&staticBodyDef);

		b2PolygonShape staticShape;
		staticShape.Set(b2Pts, 3);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &staticShape;
		fixtureDef.density = 0;
		fixtureDef.friction = _props.friction;
		fixtureDef.restitution = _props.restitution;

		body->CreateFixture(&fixtureDef);
	}
	else
	{
		// Dynamic Body
		b2BodyDef dynamicBodyDef;
		dynamicBodyDef.type = b2_dynamicBody;
		dynamicBodyDef.position.Set(convertPos.x, convertPos.y);
		dynamicBodyDef.angle = _props.angle;
		body = m_pWorld->CreateBody(&dynamicBodyDef);

		b2PolygonShape dynamicShape;
		dynamicShape.Set(b2Pts, 3);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicShape;
		fixtureDef.density = _props.density;
		fixtureDef.friction = _props.friction;
		fixtureDef.restitution = _props.restitution;

		body->CreateFixture(&fixtureDef);
	}

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(body, convertScale, m_pixelsPerMeter);
	return pPhysicsBody;
}

Physics_Body_2D* Physics_2D::CreatePhysicsObject_Circle(TPhysicsProperties _props)
{
	b2Body* body;
	v2float convertPos = _props.pos / m_pixelsPerMeter;
	v2float convertScale = _props.scale / m_pixelsPerMeter;

	if (_props.movable == false)
	{
		// Static Body
		b2BodyDef staticBodyDef;
		staticBodyDef.position.Set(convertPos.x, convertPos.y);
		staticBodyDef.angle = _props.angle;
		body = m_pWorld->CreateBody(&staticBodyDef);

		b2CircleShape staticCircle;
		staticCircle.m_p.Set(convertPos.x, convertPos.y);
		staticCircle.m_radius = convertScale.x / 2.0f;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &staticCircle;
		fixtureDef.density = 0;
		fixtureDef.friction = _props.friction;
		fixtureDef.restitution = _props.restitution;

		body->CreateFixture(&fixtureDef);
	}
	else
	{
		// Dynamic Body
		b2BodyDef dynamicBodyDef;
		dynamicBodyDef.type = b2_dynamicBody;
		dynamicBodyDef.position.Set(convertPos.x, convertPos.y);
		dynamicBodyDef.angle = _props.angle;
		body = m_pWorld->CreateBody(&dynamicBodyDef);

		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = convertScale.x / 2.0f;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicCircle;
		fixtureDef.density = _props.density;
		fixtureDef.friction = _props.friction;
		fixtureDef.restitution = _props.restitution;

		body->CreateFixture(&fixtureDef);
	}

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(body, convertScale, m_pixelsPerMeter);
	return pPhysicsBody;
}

void Physics_2D::CreateResoluteJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _jointPos)
{
	// Convert the input position into a usable b2Vec2
	b2Vec2 b2jointPos = { _jointPos.x / m_pixelsPerMeter, _jointPos.y / m_pixelsPerMeter };

	// Initialise the Joint
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(_bodyA->GetBody(), _bodyB->GetBody(), b2jointPos);

	// Create the Joint
	m_pWorld->CreateJoint(&jointDef);
}

void Physics_2D::CreateRopeJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _anchorPosA, v2float _anchorPosB)
{
	b2DistanceJointDef jd;
	b2Vec2 p1, p2, d;

	jd.frequencyHz = 0.0f;
	jd.dampingRatio = 0.0f;

	jd.bodyA = _bodyA->GetBody();
	jd.bodyB = _bodyB->GetBody();
	jd.localAnchorA.Set(500, 0);
	jd.localAnchorB.Set(-100, 0);
	p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
	p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
	d = p2 - p1;
	jd.length = d.Length();
	m_pWorld->CreateJoint(&jd);
}