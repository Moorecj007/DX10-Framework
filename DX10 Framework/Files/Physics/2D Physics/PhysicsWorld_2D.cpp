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
	m_pWorld = 0;
	m_pBreakListener = 0;
}

PhysicsWorld_2D::~PhysicsWorld_2D()
{
	ReleasePtr(m_pBreakListener);
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

	m_pBreakListener = new Physics_Break_Listener();
	m_pWorld->SetContactListener(m_pBreakListener);

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

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(m_pWorld, body, _props, m_pixelsPerMeter);
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

	Physics_Body_2D* pPhysicsBody = new Physics_Body_2D(m_pWorld, body, convertRadius, m_pixelsPerMeter);
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

Physics_Rope_2D* PhysicsWorld_2D::CreateDistanceJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _relativeAnchorA, v2float _relativeAnchorB, bool _collide)
{
	b2DistanceJointDef distJoint;
	b2Vec2 p1, p2, d;

	distJoint.collideConnected = _collide;

	distJoint.frequencyHz = 0.0f;
	distJoint.dampingRatio = 1.0f;
	distJoint.bodyA = _bodyA->GetBody();
	distJoint.bodyB = _bodyB->GetBody();
	distJoint.localAnchorA.Set(_relativeAnchorA.x / m_pixelsPerMeter, _relativeAnchorA.y / m_pixelsPerMeter);
	distJoint.localAnchorB.Set(_relativeAnchorB.x / m_pixelsPerMeter, _relativeAnchorB.y / m_pixelsPerMeter);
	p1 = distJoint.bodyA->GetWorldPoint(distJoint.localAnchorA);
	p2 = distJoint.bodyB->GetWorldPoint(distJoint.localAnchorB);
	d = (p2 - p1);
	distJoint.length = d.Length();
	b2Joint* pJoint = m_pWorld->CreateJoint(&distJoint);

	Physics_Rope_2D* pPhysicsJoint = new Physics_Rope_2D(pJoint, m_pixelsPerMeter);
	return pPhysicsJoint;
}

Physics_Pulley_2D* PhysicsWorld_2D::CreatePulley(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _relativeAnchorA, v2float _relativeAnchorB, v2float _worldAnchorA, v2float _worldAnchorB)
{
	b2Vec2 localAnchorA = { _bodyA->GetBody()->GetPosition().x + _relativeAnchorA.x / m_pixelsPerMeter, _bodyA->GetBody()->GetPosition().y + _relativeAnchorA.y / m_pixelsPerMeter };
	b2Vec2 localAnchorB = { _bodyB->GetBody()->GetPosition().x + _relativeAnchorB.x / m_pixelsPerMeter, _bodyB->GetBody()->GetPosition().y + _relativeAnchorB.y / m_pixelsPerMeter };

	b2Vec2 groundAnchorA = b2Vec2(_worldAnchorA.x / m_pixelsPerMeter, _worldAnchorA.y / m_pixelsPerMeter);
	b2Vec2 groundAnchorB = b2Vec2(_worldAnchorB.x / m_pixelsPerMeter, _worldAnchorB.y / m_pixelsPerMeter);

	float ratio = 1.0f;

	b2PulleyJointDef pulleyJointDef;
	pulleyJointDef.Initialize(_bodyA->GetBody(), _bodyB->GetBody(), groundAnchorA, groundAnchorB, localAnchorA, localAnchorB, ratio);
	pulleyJointDef.lengthA = (localAnchorA - groundAnchorA).Length();
	pulleyJointDef.lengthB = (localAnchorB - groundAnchorB).Length();

	b2Joint* pPulleyJoint = m_pWorld->CreateJoint(&pulleyJointDef);
	Physics_Pulley_2D* pPhysicsJoint = new Physics_Pulley_2D((b2PulleyJoint*)pPulleyJoint, m_pixelsPerMeter);
	return pPhysicsJoint;	
}

Physics_Spring_2D* PhysicsWorld_2D::CreateSpring(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _relativeAnchorA, v2float _relativeAnchorB, float _maxForce)
{
	b2MotorJointDef motorJointDef;
	motorJointDef.Initialize(_bodyB->GetBody(), _bodyA->GetBody());
	motorJointDef.maxForce = _maxForce;
	motorJointDef.maxTorque = 10000.0f;
	b2MotorJoint* pMotorJoint = (b2MotorJoint*)m_pWorld->CreateJoint(&motorJointDef);
	b2Vec2 vec = pMotorJoint->GetLinearOffset();

	b2PrismaticJointDef prismaticJointDef;
	prismaticJointDef.bodyA = _bodyA->GetBody();
	prismaticJointDef.bodyB = _bodyB->GetBody();
	prismaticJointDef.localAnchorA = { _relativeAnchorA.x / m_pixelsPerMeter, _relativeAnchorA.y / m_pixelsPerMeter };
	prismaticJointDef.localAnchorB = { _relativeAnchorB.x / m_pixelsPerMeter, _relativeAnchorB.y / m_pixelsPerMeter };
	
	b2Vec2 worldAnchorA = _bodyA->GetBody()->GetWorldPoint({ _relativeAnchorA.x / m_pixelsPerMeter, _relativeAnchorA.y / m_pixelsPerMeter });
	b2Vec2 worldAnchorB = _bodyB->GetBody()->GetWorldPoint({ _relativeAnchorB.x / m_pixelsPerMeter, _relativeAnchorB.y / m_pixelsPerMeter });

	prismaticJointDef.localAxisA = { worldAnchorA.x - worldAnchorB.x, worldAnchorA.y - worldAnchorB.y };
	b2PrismaticJoint* pPrismaticJoint = (b2PrismaticJoint*)m_pWorld->CreateJoint(&prismaticJointDef);

	Physics_Spring_2D* pSpring = new Physics_Spring_2D(pMotorJoint, m_pixelsPerMeter);
	return pSpring;
}




std::vector<Physics_Body_2D*>* PhysicsWorld_2D::CreateRope(Physics_Body_2D* _physBodyA, Physics_Body_2D* _physBodyB, v2float _relativeAnchorA, v2float _relativeAnchorB, COLORREF _color)
{
	std::vector<Physics_Body_2D*>* ropeBodies = new std::vector < Physics_Body_2D* > ;

	b2Vec2 b2RelativeAnchorA = { _relativeAnchorA.x / m_pixelsPerMeter, _relativeAnchorA.y / m_pixelsPerMeter };
	b2Vec2 b2RelativeAnchorB = { _relativeAnchorB.x / m_pixelsPerMeter, _relativeAnchorB.y / m_pixelsPerMeter };
	b2Vec2 b2WorldAnchorA = _physBodyA->GetBody()->GetWorldPoint(b2RelativeAnchorA);
	b2Vec2 b2WorldAnchorB = _physBodyB->GetBody()->GetWorldPoint(b2RelativeAnchorB);

	float ropeLength = (b2WorldAnchorA - b2WorldAnchorB).Length() * m_pixelsPerMeter;
	int ropeLinkCount = 20;

	v2float linkPosition = { (b2WorldAnchorA.x * m_pixelsPerMeter), (b2WorldAnchorA.y * m_pixelsPerMeter) };
	float linkHeight = ropeLength / ropeLinkCount;
	float linkWidth = 1.0f;
	Physics_Body_2D* prevBody = _physBodyA;

	for (int i = 0; i < ropeLinkCount; i++)
	{
		TPhysicsProperties physProps;
		v2float* pPoints = new v2float[4];
		pPoints[0] = { -linkWidth, (-linkHeight / 2.0f) - 1 };
		pPoints[1] = { linkWidth, (-linkHeight / 2.0f) - 1 };
		pPoints[2] = { linkWidth, (linkHeight / 2.0f) + 1 };
		pPoints[3] = { -linkWidth, (linkHeight / 2.0f) + 1 };
		ZeroMemory(&physProps, sizeof(physProps));
		physProps.circleObject = false;
		physProps.pPoints = pPoints;
		physProps.size = 4;
		physProps.pos = linkPosition;
		physProps.density = 200.0f;
		physProps.collisionType = CT_DYNAMIC;
		physProps.collideWith = 0;
		Physics_Body_2D* pTempBody = CreatePhysicsObject(physProps);
		ropeBodies->push_back(pTempBody);

		v2float jointLocation = { linkPosition.x, linkPosition.y + linkHeight / 2.0f };
		CreateRevoluteJoint(prevBody, pTempBody, jointLocation, false);

		linkPosition.y += linkHeight;
		prevBody = pTempBody;
	}

	b2Vec2 storeOriginalPos = { _physBodyB->GetPosition().x / m_pixelsPerMeter, _physBodyB->GetPosition().y / m_pixelsPerMeter};
	float storeOriginalAngle = _physBodyB->GetAngle();
	b2Vec2 endofRopePos = { prevBody->GetPosition().x, (prevBody->GetPosition().y + linkHeight / 2.0f) };

	b2Vec2 physicsPosition = endofRopePos - b2RelativeAnchorB;
	physicsPosition.x = physicsPosition.x / m_pixelsPerMeter;
	physicsPosition.y = physicsPosition.y / m_pixelsPerMeter;
	_physBodyB->GetBody()->SetTransform(physicsPosition, storeOriginalAngle);
	
	CreateRevoluteJoint(prevBody, _physBodyB, {endofRopePos.x, endofRopePos.y}, false);

	_physBodyB->GetBody()->SetTransform(storeOriginalPos, storeOriginalAngle);

	return ropeBodies;
}

std::vector<Physics_Body_2D*>* PhysicsWorld_2D::BreakObject(Physics_Body_2D* _body)
{
	v2float* pPoints = _body->GetPoints();
	v2float position = _body->GetPosition();
	UINT size = _body->GetSize();
	v2float* pWorldPoints = new v2float[size];

	v2float collisionPoint = _body->GetBreakProperties()->pCollisionWorldPoints[0] * m_pixelsPerMeter;
	v2float impactVelocity = _body->GetBreakProperties()->impactVelocity;

	//collisionPoint = collisionPoint + (impactVelocity.Normalise() * 50.0f);

	for (UINT i = 0; i < size; i++)
	{
		pWorldPoints[i] = { pPoints[i].x + position.x, pPoints[i].y + position.y };
	}

	std::vector<Physics_Body_2D*>* pPhysBodies = new std::vector<Physics_Body_2D*>;
	for (UINT i = 0; i < size; i++)
	{
		v2float* pTriPoints = new v2float[3];
		pTriPoints[0] = pWorldPoints[i];
		if (i == (size - 1))
		{
			pTriPoints[1] = pWorldPoints[0];
		}
		else
		{
			pTriPoints[1] = pWorldPoints[i + 1];
		}
		pTriPoints[2] = collisionPoint;

		v2float centerPos;
		centerPos.x = pTriPoints[0].x + pTriPoints[1].x + pTriPoints[2].x;
		centerPos.y = pTriPoints[0].y + pTriPoints[1].y + pTriPoints[2].y;
		centerPos = centerPos / 3.0f;

		for (UINT i = 0; i < 3; i++)
		{
			pTriPoints[i] -= centerPos;
		}

		TPhysicsProperties physProps = _body->GetPhysicsProperties();
		physProps.size = 3;
		physProps.pos = centerPos;
		physProps.pPoints = pTriPoints;
		physProps.collisionType = CT_DYNAMIC;

		Physics_Body_2D* pTempBody = CreatePhysicsObject(physProps);
		pPhysBodies->push_back(pTempBody);
	}

	ReleasePtr(_body->GetBreakProperties()->pCollisionWorldPoints);
	ReleasePtr(pWorldPoints)

	return pPhysBodies;
}





