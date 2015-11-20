﻿#ifndef __BILL_H__
#define __BILL_H__

#include "..\FrameWork\define.h"
#include "..\FrameWork\Animation.h"
#include "..\FrameWork\InputController.h"
#include "..\FrameWork\Managers\SceneManager.h"
#include "..\FrameWork\StopWatch.h"
#include "BaseObject.h"
#include "IComponent.h"
#include "CollisionBody.h"
#include "Bullet.h"

#define BILL_MOVE_SPEED 120
#define BILL_JUMP_VEL 450
#define TEST_LAND 200
#define GRAVITY 800
#define SHOOT_SPEED 200.0f

[event_receiver(native)]
class Bill : public BaseObject, public IControlable
{
public:
	Bill();
	~Bill();

	void init();
	void updateInput(float dt);
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();

	//void setPosition(float x, float y);

	void onKeyPressed(KeyEventArg* key_event);
	void onKeyReleased(KeyEventArg* key_event);

	void onCollisionBegin(CollisionEventArg* collision_arg);
	void onCollisionEnd(CollisionEventArg* collision_arg);

	float checkCollision(BaseObject* object, float dt);

	// Đổi kiểu đạn. 
	void changeBulletType(eAirCraftType );
private:
	map<int, Animation*> _animations;
	map<string, IComponent*> _componentList;
	
	float _movingSpeed;
	StopWatch* _stopWatch;

	void standing();
	void moveLeft();
	void moveRight();
	void jump();
	void layDown();
	void falling();
	void shoot();
	list<bool> _canStand;

	GVector2 getVelocity();
	void updateStatus(float dt);

	eStatus _currentAnimateIndex;
	void updateCurrentAnimateIndex();

	bool _sideCollide;

	list<Bullet* > _listBullets;

	eDirection getAimingDirection();
};

//TEST
[event_receiver(native)]
class MyBox : public BaseObject
{
public:
	MyBox(int index) : BaseObject(eID::BOX)
	{
		_index = index;
	}
	~MyBox()
	{

	}

	void init()
	{
		_sprite = SpriteManager::getInstance()->getSprite(eID::BOX);
		_sprite->setIndex(_index);

		this->setPhysicsBodySide((eDirection)ALL_EDGES);

		//_componentList["CollisionBody"] = new CollisionBody(this);

		//if (_index == 1)
		//{
		//	_componentList["Movement"] = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
		//	_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), (Movement*)_componentList["Movement"]);
		//}

		//__hook(&CollisionBody::onCollisionBegin, (CollisionBody*)_componentList["CollisionBody"], &MyBox::onCollisionBegin);

	}

	void update(float deltatime)
	{
		for (auto it = _componentList.begin(); it != _componentList.end(); it++)
		{
			it->second->update(deltatime);
		}
	}

	void draw(LPD3DXSPRITE sp, Viewport* vp)
	{
		_sprite->render(sp, vp);
	}

	//float checkCollision(BaseObject* object, float dt) 
	//{
	//	((CollisionBody*)_componentList["CollisionBody"])->checkCollision(object, dt);
	//	
	//	return 0.0f;
	//}

	//void onCollisionBegin(CollisionEventArg* e)
	//{
	//	auto x = e->_sideCollisionB;

	//	((Gravity*)_componentList["Gravity"])->setStatus(eGravityStatus::SHALLOWED);
	//	((Movement*)_componentList["Movement"])->setVelocity(GVector2(0, 0));
	//}

	//GVector2 getVelocity() {
	//	return ((Movement*)_componentList["Movement"])->getVelocity();
	//}

	void release() {}

private:
	map<string, IComponent*> _componentList;
	int _index;
	bool _flag;
};
#endif // !__BILL_H__
