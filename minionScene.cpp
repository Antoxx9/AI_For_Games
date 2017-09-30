#include "minionScene.h"

USING_NS_CC;

Scene* minionScene::createScene()
{
	auto scene = Scene::create();
	auto layer = minionScene::create();
	scene->addChild(layer);
	return scene;
}

bool minionScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto drawNode = DrawNode::create();
	//drawNode->drawSegment(Vec2(100, 400), Vec2(500, 400), 10, Color4F::ORANGE);
	//drawNode->drawSegment(Vec2(100, 400), Vec2(100, 1000), 10, Color4F::ORANGE);
	//drawNode->drawSegment(Vec2(100, 1000), Vec2(500, 400), 10, Color4F::ORANGE);
	//this->addChild(drawNode, 5);
	//auto drawNode = DrawNode::create();
	personaje.set("veyron.png", Vec3(1000, 1000, -300), -90.0, Vec2(0.5, 0.5), 50.0, 75.0, 25.0, 50.0);
	target.set("veyron.png", Vec3(1500, 1000, 0), 0.0, Vec2(0.5, 0.5), 150.0, 200.0, 25.0, 50.0);
	pj2.set("veyron.png", Vec3(200, 1000, 0), -90.0, Vec2(0.5, 0.5), 150.0, 200.0, 25.0, 50.0);
	pj3.set("veyron.png", Vec3(900, 1000, 0), 45.0, Vec2(0.5, 0.5), 150.0, 400.0, 25.0, 50.0);
	pj4.set("veyron.png", Vec3(500, 1000, 200), -90.0, Vec2(0.5, 0.5), 150.0, 200.0, 25.0, 50.0);
	pj5.set("veyron.png", Vec3(500, 500, -100), -90.0, Vec2(0.5, 0.5), 150.0, 200.0, 25.0, 50.0);

	this->addChild(personaje.sprite, 0);
	this->addChild(target.sprite, 1);
	this->addChild(pj2.sprite, 2);
	this->addChild(pj3.sprite, 3);
	this->addChild(pj4.sprite, 4);
	this->addChild(pj5.sprite, 5);

	delta_x_1 = 100;
	delta_x_2 = 2 * delta_x_1;
	delta_y_1 = 100;
	delta_y_2 = 2 * delta_y_1;
	p_form_1 = Vec2(personaje.posicion.x + delta_x_1, personaje.posicion.y + delta_y_1);
	p_form_2 = Vec2(personaje.posicion.x + delta_x_1, personaje.posicion.y - delta_y_1);
	p_form_3 = Vec2(personaje.posicion.x + delta_x_2, personaje.posicion.y + delta_y_2);
	p_form_4 = Vec2(personaje.posicion.x + delta_x_2, personaje.posicion.y - delta_y_2);
	aux = Vec2(personaje.posicion.x, personaje.posicion.y);
	p_form_1.rotate(aux, CC_DEGREES_TO_RADIANS(-personaje.orientacion));
	p_form_2.rotate(aux, CC_DEGREES_TO_RADIANS(-personaje.orientacion));
	p_form_3.rotate(aux, CC_DEGREES_TO_RADIANS(-personaje.orientacion));
	p_form_4.rotate(aux, CC_DEGREES_TO_RADIANS(-personaje.orientacion));
	//drawNode->drawDot(p_form_1, 10, Color4F::RED);
	//drawNode->drawDot(p_form_2, 10, Color4F::ORANGE);
	//drawNode->drawDot(p_form_3, 10, Color4F::YELLOW);
	//drawNode->drawDot(p_form_4, 10, Color4F::BLUE);

	f1.set("", Vec3(p_form_1.x, p_form_1.y, personaje.posicion.z), 0, Vec2(0.5, 0.5), 0, 0, 0, 0);
	f2.set("", Vec3(p_form_2.x, p_form_2.y, personaje.posicion.z), 0, Vec2(0.5, 0.5), 0, 0, 0, 0);
	f3.set("", Vec3(p_form_3.x, p_form_3.y, personaje.posicion.z), 0, Vec2(0.5, 0.5), 0, 0, 0, 0);
	f4.set("", Vec3(p_form_4.x, p_form_4.y, personaje.posicion.z), 0, Vec2(0.5, 0.5), 0, 0, 0, 0);

	this->addChild(drawNode, 100);
	//this->addChild(target.sprite, 1);
	//this->addChild(pj2.sprite, 2);

	//comportamiento.setear(100.0, 45.0, 1.0, 5.0);
	//comportamiento2.setear(5.0, 1.0, 5.0, 45.0, 45.0, 0.75, 30.0, 250.0, 25.0);
	//comportamiento2.setear(20.0, 20.0, 0.42, 4.0);
	comportamiento3.set(500.0, 1000000);
	comportamiento4.set(5.0, 30.0, 150.0, 45.0, 0.25, 25.0);
	comportamiento6.set(1.0, 5.0);
	comportamiento7.set(5.0, 100.0);
	comportamiento8.set(20.0);
	comportamiento9.set(1.0, 25.0);
	comportamiento10.set(20.0);
	evadirColision.set(20.0);
	//targets.push_back(pj2);
	//targets.push_back(pj3);
	//targets.push_back(pj4);
	//targets1.push_back(&target);
	targets1.push_back(&pj2);
	//targets1.push_back(&pj3);
	//targets1.push_back(&pj4);
	//targets1.push_back(&pj5);

	//targets2.push_back(&target);
	//targets2.push_back(&personaje);
	//targets2.push_back(&pj2);
	targets2.push_back(&pj3);
	//targets2.push_back(&pj5);

	//targets3.push_back(&target);
	targets3.push_back(&personaje);
	targets3.push_back(&pj2);
	targets3.push_back(&pj3);
	targets3.push_back(&pj5);

	//targets4.push_back(&target);
	targets4.push_back(&personaje);
	targets4.push_back(&pj2);
	targets4.push_back(&pj3);
	targets4.push_back(&pj4);
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(minionScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(minionScene::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleUpdate();
	return true;
}

void minionScene::actualizarFormacion() {
	delta_x_1 = 100;
	delta_x_2 = 2 * delta_x_1;
	delta_y_1 = 100;
	delta_y_2 = 2 * delta_y_1;
	p_form_1 = Vec2(personaje.posicion.x + delta_x_1, personaje.posicion.y + delta_y_1);
	p_form_2 = Vec2(personaje.posicion.x + delta_x_1, personaje.posicion.y - delta_y_1);
	p_form_3 = Vec2(personaje.posicion.x + delta_x_2, personaje.posicion.y + delta_y_2);
	p_form_4 = Vec2(personaje.posicion.x + delta_x_2, personaje.posicion.y - delta_y_2);
	aux = Vec2(personaje.posicion.x, personaje.posicion.y);
	p_form_1.rotate(aux, CC_DEGREES_TO_RADIANS(-personaje.orientacion));
	p_form_2.rotate(aux, CC_DEGREES_TO_RADIANS(-personaje.orientacion));
	p_form_3.rotate(aux, CC_DEGREES_TO_RADIANS(-personaje.orientacion));
	p_form_4.rotate(aux, CC_DEGREES_TO_RADIANS(-personaje.orientacion));
	f1.set("", Vec3(p_form_1.x, p_form_1.y, personaje.posicion.z), 0, Vec2(0.5, 0.5), 0, 0, 0, 0);
	f2.set("", Vec3(p_form_2.x, p_form_2.y, personaje.posicion.z), 0, Vec2(0.5, 0.5), 0, 0, 0, 0);
	f3.set("", Vec3(p_form_3.x, p_form_3.y, personaje.posicion.z), 0, Vec2(0.5, 0.5), 0, 0, 0, 0);
	f4.set("", Vec3(p_form_4.x, p_form_4.y, personaje.posicion.z), 0, Vec2(0.5, 0.5), 0, 0, 0, 0);
}

void minionScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event){
	
	if (keys.find(keyCode) == keys.end()) {
		keys[keyCode] = 1;
	}
}

void minionScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event){
	keys.erase(keyCode);
}

bool minionScene::isKeyPressed(EventKeyboard::KeyCode code) {
	// Check if the key is currently pressed by seeing it it's in the std::map keys
	// In retrospect, keys is a terrible name for a key/value paried datatype isnt it?
	if (keys.find(code) != keys.end())
		return true;
	return false;
}

void minionScene::chequearTeclas(gameObject &target, double delta) {
	EventKeyboard::KeyCode keyCode;
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
		target.moverArriba(delta);
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
		target.moverAbajo(delta);
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
		target.moverDer(delta);
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
		target.moverIzq(delta);
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
		target.rotarAntiHorario(delta);
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
		//target.rotarHorario(delta);
	}

	target.revisarBounds();
}

void minionScene::update(float delta) {
	//log("Posicion target: %f %f", target.sprite->getPosition().x, target.sprite->getPosition().y);
	
	//target.sprite->setPosition(Vec2(target.sprite->getPosition().x - 150 * delta, target.sprite->getPosition().y));
	//target.alinearSprite();
	//if (target.sprite->getPosition().x  < 0 - (target.sprite->getBoundingBox().size.width / 2))
	//	target.sprite->setPosition(Vec2(this->getBoundingBox().getMaxX() + target.sprite->getBoundingBox().size.width / 2, 
	//		target.sprite->getPosition().y));
	//log("%f %f", this->getBoundingBox().getMaxX(), this->getBoundingBox().getMaxY());
	//personaje.revisarBounds();
	//personaje.orientacion += 1;
	//personaje.sprite->setRotation(personaje.orientacion);
	//Vec2 resta;
	//resta.rotate(personaje.posicion, CC_DEGREES_TO_RADIANS(-personaje.orientacion));
	//log("LA resta da %f %f", resta.x, resta.y);
	chequearTeclas(personaje, delta);
	actualizarFormacion();
	//comportamiento4.update(pj2, delta);
	//comportamiento4.update(pj3, delta);
	//comportamiento5.update(pj2, target, delta);
	//evadirColision.update(pj3, targets1, delta);
	//evadirColision.update(pj2, targets2, delta);
	//comportamiento10.update(pj2, targets1, delta);
	//comportamiento10.update(pj3, targets2, delta);
	//comportamiento10.update(pj4, targets3, delta);
	//comportamiento10.update(pj5, targets4, delta);
	//log("Orientacion pj2 %f", pj2.orientacion);

	flock.update(pj2, f1, personaje, delta);
	flock.update(pj3, f2, personaje, delta);
	flock.update(pj4, f3, personaje, delta);
	flock.update(pj5, f4, personaje, delta);
	personaje.orientacion += 0.5;
	personaje.sprite->setRotation(personaje.orientacion);
	/*
	comportamiento9.update(pj2, personaje, delta);
	comportamiento9.update(pj3, personaje, delta);
	comportamiento9.update(pj4, personaje, delta);
	comportamiento9.update(pj5, personaje, delta);
	//comportamiento10.update(personaje, delta);
	comportamiento7.update(pj2, f1, delta);
	comportamiento7.update(pj3, f2, delta);
	comportamiento7.update(pj4, f3, delta);
	comportamiento7.update(pj5, f4, delta);
	//comportamiento8.update(personaje, target, delta);
	*/
	//comportamiento4.update(personaje, delta);
	//comportamiento4.update(pj2, delta);
	//comportamiento4.update(pj3, delta);
	//comportamiento4.update(pj4, delta);

	//comportamiento5.update(personaje, target, delta);
	//comportamiento5.update(pj2, target, delta);
	//comportamiento5.update(pj3, target, delta);
	//comportamiento5.update(pj4, target, delta);

	//comportamiento6.update(personaje, target, delta);
	//comportamiento6.update(pj2, target, delta);
	//comportamiento6.update(pj3, target, delta);
	//comportamiento6.update(pj4, target, delta);

	//comportamiento3.update(personaje, targets1, delta);
	//comportamiento3.update(pj2, targets2, delta);
	//comportamiento3.update(pj3, targets3, delta);
	//comportamiento3.update(pj4, targets4, delta);
	//comportamiento3.update(personaje, targets1, delta);
}
