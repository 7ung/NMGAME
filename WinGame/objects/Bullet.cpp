#include "Bullet.h"

Bullet::Bullet(GVector2 startPosition, eDirection dir) : BaseObject(eID::BULLET)
{
	_startPosition = startPosition;
	_direction = dir;
}

Bullet::~Bullet()
{
}

void Bullet::init()
{
	_damage = 1;

	_sprite = SpriteManager::getInstance()->getSprite(eID::BULLET);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "normal_bullet"));

	this->setPosition(_startPosition);

	GVector2 veloc;

	if ((_direction & eDirection::LEFT) == eDirection::LEFT)
	{
		veloc.x = - NORMAL_BULLET_SPEED;
	}
	else if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
	{
		veloc.x = NORMAL_BULLET_SPEED;
	}
	else
	{
		veloc.x = 0;
	}

	if ((_direction & eDirection::TOP) == eDirection::TOP)
	{
		veloc.y = NORMAL_BULLET_SPEED;
	}
	else if ((_direction & eDirection::BOTTOM) == eDirection::BOTTOM)
	{
		veloc.y = -NORMAL_BULLET_SPEED;
	}
	else
	{
		veloc.y = 0;
	}

	auto movement = new Movement(GVector2(0, 0), veloc, _sprite);
	_componentList["Movement"] = movement;

	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin);
}

void Bullet::update(float deltatime)
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void Bullet::draw(LPD3DXSPRITE spriteHandle, Viewport *viewport)
{
	_sprite->render(spriteHandle, viewport);
}

void Bullet::release()
{
}

GVector2 Bullet::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

void Bullet::onCollisionBegin(CollisionEventArg* collision_arg)
{
	if(collision_arg->_otherObject->getId() == eID::BOX)
		OutputDebugString(L"hit...\n");
}

float Bullet::checkCollision(BaseObject * object, float dt)
{
	auto body = (CollisionBody*)_componentList["CollisionBody"];
	body->checkCollision(object, dt);

	return 0.0f;
}
