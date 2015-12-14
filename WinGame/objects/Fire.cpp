#include "Fire.h"


Fire::Fire(GVector2 leftposition, GVector2  rightposition,GVector2 pos,int veloc) :BaseObject(eID::FIRE)
{
	_leftPosition = leftposition;
	_rightPosition = rightposition;
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::FIRE);
	this->_sprite->setScale(SCALE_FACTOR);
	this->_sprite->setPosition(pos);
	_veloc = veloc;
}

void Fire::init()
{
	
	this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FIRE, "normal"));

	Movement* movement = new Movement(VECTOR2ZERO, HORIZONTAL_VELOC, _sprite);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_listComponent["Movement"] = movement;
	this->_listComponent["CollisionBody"] = collisionBody;
	this->setStatus(NORMAL);
	this->setPhysicsBodySide(eDirection::TOP);
}
void Fire::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	auto pos = this->getPosition();
	auto move = (Movement*) this->_listComponent["Movement"];
	if (_veloc == 1)
	{
		
		move->setVelocity(HORIZONTAL_VELOC);
	}
	else
	{
		move->setVelocity(HORIZONTAL_VELOC_PRE);
	}
	if (pos.x <= _leftPosition.x)
	{
	/*	auto move = (Movement*) this->_listComponent["Movement"];*/
		move->setVelocity(HORIZONTAL_VELOC);
		_veloc = 1;
	}
	if (pos.x >= _rightPosition.x)
	{
	/*	auto move = (Movement*) this->_listComponent["Movement"];*/
		move->setVelocity(HORIZONTAL_VELOC_PRE);
		_veloc = -1;
	}
	for (auto component : _listComponent)
	{
		component.second->update(deltatime);
	}
}
void Fire::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	this->_sprite->render(spriteHandle, viewport);
}
void Fire::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	_listComponent.clear();

	SAFE_DELETE(this->_sprite);
}
GVector2 Fire::getVelocity()
{
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}
IComponent* Fire::getComponent(string componentName)
{
	auto it = _listComponent.find(componentName);
	if (it == _listComponent.end())
		throw new exception("key not match");
	return it->second;
}
Fire::~Fire()
{
}