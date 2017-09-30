#pragma once

#include "cocos2d.h"
#include "Object.cpp"
#include <map>
USING_NS_CC;

class minionScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init() override;
	void update(float) override;
	void actualizarFormacion();
	void chequearTeclas(gameObject &target, double delta);
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
	std::map<cocos2d::EventKeyboard::KeyCode,int> keys;

	CREATE_FUNC(minionScene);

private:
	gameObject personaje;
	gameObject target;
	gameObject pj2, pj3, pj4, pj5;
	gameObject f1, f2, f3, f4;
	//DrawNode drawNode;
	//arrive comportamiento;
	//wander comportamiento2;
	separation comportamiento3;
	wander comportamiento4;
	seek comportamiento5;
	face comportamiento6;
	arrive comportamiento7;
	pursue comportamiento8;
	align comportamiento9;
	collisionAvoidance comportamiento10;
	collisionAvoidance evadirColision;
	formation flock;
	Vec2 p_form_1;
	Vec2 p_form_2;
	Vec2 p_form_3;
	Vec2 p_form_4;
	Vec2 aux;

	int delta_x_1;
	int delta_x_2;
	int delta_y_1;
	int delta_y_2;
	//lookWhereYoureGoing comportamiento5;
	//collisionAvoidance comportamiento4;
	std::vector<gameObject*> targets1, targets2, targets3, targets4;
};
