#include "cocos2d.h"
USING_NS_CC;

class steeringOutput {
public:
	Vec3 linear = Vec3(0,0,0);
	double angular = 0;
	bool null = false;
	bool detener = false;
	steeringOutput() {};
};

class gameObject {
	public: 
		cocos2d::Sprite* sprite;
		Vec3 velocidad;
		Vec3 posicion;
		double orientacion, rotacion;
		double maxAccel, maxSpeed;
		double maxAngular, maxRotation;

		gameObject() {};

		void set(const std::string s, Vec3 pos, double rot, Vec2 anchor, double maxAcc, double maxSpd, double maxAng, double maxRot){
			if (s == "") {
				orientacion = rot;
				posicion = pos;
				rotacion = rot;
				return;
			}
			sprite = Sprite::create(s);
			sprite->setAnchorPoint(anchor);
			sprite->setPosition3D(pos);
			sprite->setRotation(rot);
			sprite->setScale(1);
			orientacion = rot;
			posicion = pos;
			rotacion = 0;
			maxAccel = maxAcc;
			maxSpeed = maxSpd;
			maxAngular = maxAng;
			maxRotation = maxRot;
		}

		void copy(gameObject & objeto) {
			sprite = objeto.sprite;
			velocidad = objeto.velocidad;
			posicion = objeto.posicion;
			orientacion = objeto.orientacion;
			rotacion = objeto.rotacion;
			maxAccel = objeto.maxAccel;
			maxSpeed = objeto.maxSpeed;
			maxAngular = objeto.maxAngular;
			maxRotation = objeto.maxRotation;
		}

		void moverArriba(float delta) {
			posicion.y += maxSpeed*delta;
			sprite->setPosition3D(posicion);
		}

		void moverAbajo(float delta) {
			posicion.y -= maxSpeed*delta;
			sprite->setPosition3D(posicion);
		}

		void moverIzq(float delta) {
			posicion.x -= maxSpeed*delta;
			sprite->setPosition3D(posicion);
		}

		void moverDer(float delta) {
			posicion.x += maxSpeed*delta;
			sprite->setPosition3D(posicion);
		}

		void rotarAntiHorario(float delta) {
			orientacion -= 1;
			sprite->setRotation(orientacion);
		}

		void alinearSprite() {
			posicion = sprite->getPosition3D();
			orientacion = sprite->getRotation();
		}

		void revisarBounds() {
			if (sprite->getPosition3D().x < 0 - (sprite->getBoundingBox().size.width / 2)) {
				sprite->setPosition3D(Vec3(2048 + sprite->getBoundingBox().size.width / 2, sprite->getPosition3D().y, sprite->getPosition3D().z));
			}
			if (sprite->getPosition3D().y < 0 - (sprite->getBoundingBox().size.height / 2)) {
				sprite->setPosition3D(Vec3(sprite->getPosition3D().x, 1536 + sprite->getBoundingBox().size.height / 2, sprite->getPosition3D().z));
			}
			if (sprite->getPosition3D().x > 2048 + (sprite->getBoundingBox().size.width / 2)) {
				sprite->setPosition3D(Vec3(0 + sprite->getBoundingBox().size.width / 2, sprite->getPosition3D().y, sprite->getPosition3D().z));
			}
			if (sprite->getPosition3D().y > 1536 + (sprite->getBoundingBox().size.height / 2)) {
				sprite->setPosition3D(Vec3(sprite->getPosition3D().x, 0 + sprite->getBoundingBox().size.height / 2, sprite->getPosition3D().z));
			}
			posicion = sprite->getPosition3D();
			//if (target.sprite->getPosition().x  < 0 - (target.sprite->getBoundingBox().size.width / 2))
			//	target.sprite->setPosition(Vec2(this->getBoundingBox().getMaxX() + target.sprite->getBoundingBox().size.width / 2, 
			//		target.sprite->getPosition().y));
		}

		void update(steeringOutput st, float delta) {
			
			if (!st.null) {
				if ((st.linear != Vec3(0, 0, 0)) || !st.detener) {
					posicion = posicion + (velocidad*delta);
				}
				if ((st.angular != 0) || !st.detener) {
					orientacion = orientacion + rotacion*delta;
				}
				velocidad += st.linear * delta;
				rotacion += st.angular * delta;
				//log("angular : %f", st.angular);
				//log("Lineal_n: %f %f", st.linear.x, st.linear.y);
				if (velocidad.length() > maxSpeed) {
					velocidad.normalize();
					velocidad *= maxSpeed;
				}
				sprite->setPosition3D(posicion);
				sprite->setRotation(orientacion);
			}
			//log("La rotacion es: %lf", rotacion);
			//log("La orientacion es: %lf", orientacion);
			//log("La posicion es: %f %f", posicion.x, posicion.y);
			revisarBounds();
};
		
		double getNewOrientation(double orientActual, Vec2 velocidad) {
			if (velocidad.length() > 0) {
				return atan2(-velocidad.x, velocidad.y);
			}
			else {
				return orientActual;
			}
		};

		Vec3 asVector(double orient) {
			Vec3 retorno(sin(orient), cos(orient), 0);
			
			return retorno;
		}
};

class seek {
	
	public:

		seek() {};

		virtual steeringOutput getSteering(gameObject &character, gameObject &target) {
			steeringOutput steering;

			steering.linear = target.posicion - character.posicion;
			steering.linear.normalize();
			steering.linear *= character.maxAccel;

			steering.angular = 0;
			return steering;
		}

		void update(gameObject &character, gameObject &target, float delta) {
			steeringOutput steering = getSteering(character, target);
			//log("%f %f", steering.linear.x, steering.linear.y);
			character.update(steering, delta);
		}
};

class flee {

public:

	flee() {};

	virtual steeringOutput getSteering(gameObject &character, gameObject &target) {
		steeringOutput steering;

		steering.linear = character.posicion - target.posicion;
		steering.linear.normalize();
		steering.linear *= character.maxAccel;

		steering.angular = 0;
		return steering;
	}

	void update(gameObject &character, gameObject &target, float delta) {
		steeringOutput steering = getSteering(character, target);
		//log("%f %f", steering.linear.x, steering.linear.y);
		character.update(steering, delta);
	}
};

class arrive {
	public:
		
		double targetRadius;
		double slowRadius;
		double timeToTarget = 0.1;


		arrive() {};

		virtual void set(double targetR, double slowR) {
			targetRadius = targetR;
			slowRadius = slowR;
		}

		steeringOutput getSteering(gameObject &character, gameObject &target) {
			steeringOutput steering;
			//log("En el movimiento el pj tiene %f %f ", character.posicion.x, character.posicion.y);

			Vec3 direction = target.posicion - character.posicion;
			double distance = direction.length();

			int targetSpeed;
			Vec3 targetVelocity;

			if (distance < targetRadius) {
				steering.null = true;
				return steering;
			}

			if (distance > slowRadius) {
				targetSpeed = character.maxSpeed;
			}
			else {
				targetSpeed = character.maxSpeed * distance / slowRadius;
			}
			targetVelocity = direction;
			targetVelocity.normalize();
			targetVelocity *= targetSpeed;

			steering.linear = targetVelocity - character.velocidad;
			steering.linear = steering.linear / timeToTarget;

			if (steering.linear.length() > character.maxAccel) {
				steering.linear.normalize();
				steering.linear *= character.maxAccel;	
			}

			steering.angular = 0;
			return steering;
		}

		void update(gameObject &character, gameObject &target, float delta) {
			steeringOutput steering = getSteering(character, target);
			//log("%f %f", steering.linear.x, steering.linear.y);
			character.update(steering, delta);
		}
};

class align {
	public:
		double targetRadius;
		double slowRadius;
		double timeToTarget = 0.1;

		align(){}

		virtual void set(double targetR, double slowR) {
			targetRadius = targetR;
			slowRadius = slowR;
		}

		double mapToPi(double x) {
			if (x == 0) {
				return 0;
			}
			double aux = x*M_PI / 180.0;
			double ret = atan2(sin(aux), cos(aux));
			return ret*180.0 / M_PI;
		}

		virtual steeringOutput getSteering(gameObject &character, gameObject &target) {
			steeringOutput steering;
			//log("Orientacion target %f orientacion char %f", target.orientacion, character.orientacion);
			double rotation = target.orientacion - character.orientacion;
			double rotationSize;
			rotation = mapToPi(rotation);
			rotationSize = abs(rotation);
			if (rotationSize < targetRadius) {
				steering.null = true;
				return steering;
			}
			double targetRotation;
			if (rotationSize > slowRadius) {
				targetRotation = character.maxRotation;
			}
			else {
				targetRotation = character.maxRotation * rotationSize / slowRadius;
			}
			//log("targetRotacion de shit %lf", targetRotation);
			if (rotationSize != 0) {
				targetRotation *= rotation / rotationSize;
			}
			steering.angular = targetRotation - character.rotacion;
			steering.angular = steering.angular / timeToTarget;
			double angularAcc = abs(steering.angular);
			if (angularAcc > character.maxAngular) {
				steering.angular = steering.angular / angularAcc;
				steering.angular *= character.maxAngular;
			}
			//log("Angular del align %f", steering.angular);
			steering.linear = Vec3(0,0,0);
			return steering;
		}

		void update(gameObject &character, gameObject &target, float delta) {
			steeringOutput steering = getSteering(character, target);
			//log("Bug: %f %f %lf", steering.linear.x, steering.linear.y, steering.angular);
			character.update(steering, delta);
		}
};

class velocityMatching {
public:

	double timeToTarget = 0.1;

	steeringOutput getSteering(gameObject &character, gameObject &target) {
		steeringOutput steering;

		steering.linear = target.velocidad - character.velocidad;
		steering.linear = steering.linear / timeToTarget;

		if (steering.linear.length() > character.maxAccel) {
			steering.linear.normalize();
			steering.linear *= character.maxAccel;
		}

		steering.angular = 0;
		return steering;
	}

	void update(gameObject &character, gameObject &target, float delta) {
		steeringOutput steering = getSteering(character, target);
		//log("%f %f", steering.linear.x, steering.linear.y);
		character.update(steering, delta);
	}

};

class pursue : public seek {
	public:
		double maxPrediction;

		pursue() {}

		virtual void set(double mP) {
			maxPrediction = mP;
		}

		steeringOutput getSteering(gameObject &character, gameObject &target) {
			Vec3 direction = target.posicion - character.posicion;
			int distance = direction.length();

			int speed = character.velocidad.length();
			double prediction;

			if (speed <= distance / maxPrediction) {
				prediction = maxPrediction;
			}
			else {
				prediction = distance / speed;
			}

			gameObject seekTarget;
			seekTarget.copy(target);
			seekTarget.posicion += target.velocidad * prediction;

			return seek::getSteering(character, seekTarget);
		}

		void update(gameObject &character, gameObject &target, float delta) {
			steeringOutput steering = getSteering(character, target);
			//log("%f %f", steering.linear.x, steering.linear.y);
			character.update(steering, delta);
		}
};

class evade : public seek {
public:
	double maxPrediction;

	evade() {}

	virtual void set(double mP) {
		maxPrediction = mP;
	}

	steeringOutput getSteering(gameObject &character, gameObject &target) {
		Vec3 direction = character.posicion - target.posicion;
		int distance = direction.length();

		int speed = character.velocidad.length();
		double prediction;

		if (speed <= distance / maxPrediction) {
			prediction = maxPrediction;
		}
		else {
			prediction = distance / speed;
		}

		gameObject seekTarget;
		seekTarget.copy(target);
		seekTarget.posicion += target.velocidad * prediction;

		return seek::getSteering(character, seekTarget);
	}

	void update(gameObject &character, gameObject &target, float delta) {
		steeringOutput steering = getSteering(character, target);
		//log("%f %f", steering.linear.x, steering.linear.y);
		character.update(steering, delta);
	}
};

class face : public align {
public:
	face() {}

	virtual steeringOutput getSteering(gameObject &character, gameObject &target) {
		steeringOutput steering;

		Vec3 direction = target.posicion - character.posicion;

		if (direction.length() == 0) {
			return align::getSteering(character, target);
		}

		gameObject alignTarget;
		alignTarget.copy(target);
		alignTarget.orientacion = atan2(direction.y, -direction.x);
		alignTarget.orientacion = (alignTarget.orientacion)*180.0 / M_PI;
		log("La orientacion a la que debo es %f", alignTarget.orientacion);
		return align::getSteering(character, alignTarget);
	}

	void update(gameObject &character, gameObject &target, float delta) {
		steeringOutput steering = getSteering(character, target);
		//log("Bug: %f %f %lf", steering.linear.x, steering.linear.y, steering.angular);
		character.update(steering, delta);
	}
};

class lookWhereYoureGoing : public align {
public:
	lookWhereYoureGoing() {}

	steeringOutput getSteering(gameObject &character) {
		steeringOutput steering;

		if (character.velocidad.length() == 0) {
			steering.null = true;
			return steering;
		}

		gameObject alignTarget;
		alignTarget.copy(character);
		alignTarget.orientacion = atan2(character.velocidad.y, -character.velocidad.x);
		alignTarget.orientacion = (alignTarget.orientacion)*180.0 / M_PI;

		return align::getSteering(character, alignTarget);
	}

	void update(gameObject &character,  float delta) {
		steeringOutput steering = getSteering(character);
		//log("Bug: %f %f %lf", steering.linear.x, steering.linear.y, steering.angular);
		character.update(steering, delta);
	}

};

class wander : public face {
public:

	double wanderOffset;
	double wanderRadius;
	double wanderRate;
	double wanderOrientation;

	wander() {}

	void set(double targetR, double slowR, double wOffset, double wRadius, double wRate, double wOrient) {
		targetRadius = targetR;
		slowRadius = slowR;
		wanderOffset = wOffset;
		wanderRadius = wRadius;
		wanderRate = wRate;
		wanderOrientation = wOrient;
	}

	virtual steeringOutput getSteering(gameObject &character) {
		steeringOutput steering;
		wanderOrientation += randomBinomial() * wanderRate;

		gameObject targetWander;
		targetWander.copy(character);
		targetWander.orientacion = wanderOrientation + character.orientacion;
		targetWander.posicion = character.posicion + wanderOffset*(character.asVector(character.orientacion));
		targetWander.posicion += wanderRadius * targetWander.asVector(targetWander.orientacion);

		steering = face::getSteering(character, targetWander);
		steering.linear = character.maxAccel * character.asVector(character.orientacion);

		return steering;
	}

	double randomBinomial() {	
		return rand_0_1();
	}

	void update(gameObject &character, float delta) {
		steeringOutput steering = getSteering(character);
		//log("Bug: %f %f %lf", steering.linear.x, steering.linear.y, steering.angular);
		character.update(steering, delta);
	}
};

class separation {
public:

	double threshold;
	double decayCoefficient;

	separation() {}

	void set(double tHold, double decCoeff) {
		threshold = tHold;
		decayCoefficient = decCoeff;
	}

	steeringOutput getSteering(gameObject &character, std::vector<gameObject*> &targets) {
		steeringOutput steering;
		Vec3 direccion;
		double distance;
		double strength;
		bool frenar = true;
		for (int i = 0; i < targets.size(); i++) {
			direccion = (*targets[i]).posicion - character.posicion;
			distance = direccion.length();
			log("Distancia con %d y %f", i, distance);
			if (distance < threshold) {
				log("WEBO");
				frenar = false;
				if (distance != 0 && (decayCoefficient / (distance*distance)) < character.maxAccel) {
					strength = -(decayCoefficient / (distance*distance));
				}
				else {
					strength = -character.maxAccel;
				}
				direccion.normalize();
				if (distance != 0) {
					steering.linear += strength * direccion;
				}
				else {
					steering.linear += strength * Vec3(1,1,1);
				}	
			}
		}
		if (frenar) {
			steering.null = true;
		}
		steering.angular = 0;
		return steering;
	}
	void update(gameObject &character, std::vector<gameObject*> &targets, float delta) {
		steeringOutput steering = getSteering(character, targets);
		log("Bug: %f %f %lf", steering.linear.x, steering.linear.y, steering.angular);
		character.update(steering, delta);
	}

};

class collisionAvoidance {
public:
	
	double colRadius;

	void set(double colR) {
		colRadius = colR;
	}

	steeringOutput getSteering(gameObject &character, std::vector<gameObject*> &targets) {
		double shortestTime = 1000000;
		steeringOutput steering;

		gameObject firstTarget;
		firstTarget.set("", Vec3(-1, -1, -1), 0, Vec2(-1, -1), 10.0, 10.0, 10.0, 10.0);
		double firstMinSeparation = 0;
		double firstDistance = 0;
		Vec3 firstRelativePos = Vec3(0,0,0);
		Vec3 firstRelativeVel = Vec3(0,0,0);

		Vec3 relativePos;
		Vec3 relativeVel;
		double relativeSpeed;
		double timeToCollision;
		double distance;
		double minSeparation;

		for (int i = 0; i < targets.size(); i++) {
			log("Chequeando targets.");
			log("Mi velocidad %f %f %f", character.velocidad.x, character.velocidad.y, character.velocidad.z);
			log("Velocidad target %f %f %f", (*targets[i]).velocidad.x, (*targets[i]).velocidad.y, (*targets[i]).velocidad.z);
			relativePos = (*targets[i]).posicion - character.posicion;
			relativeVel = (*targets[i]).velocidad - character.velocidad;
			relativeSpeed = relativeVel.length();
			log("Posicion relativa %f %f %f", relativePos.x, relativePos.y, relativePos.z);
			log("Velocidad relativa %f %f %f", relativeVel.x, relativeVel.y, relativeVel.z);
			log("Rapidez relativa %f", relativeSpeed);
			log("Producto punto %f", -relativePos.dot(relativeVel));
			if (relativeSpeed == 0) {
				continue;
			}
			timeToCollision = -relativePos.dot(relativeVel) / (relativeSpeed * relativeSpeed);
			distance = relativePos.length();
			minSeparation = distance - relativeSpeed*timeToCollision;

			if (minSeparation > 2 * colRadius) {
				continue;
			}
			log("Tiempo de colision %f", timeToCollision);
			if (timeToCollision > 0 && timeToCollision < shortestTime) {
				log("Va a chocar!");
				shortestTime = timeToCollision;
				firstTarget = *(targets[i]);
				log("El target es: %d", i);
				firstMinSeparation = minSeparation;
				firstDistance = distance;
				firstRelativePos = relativePos;
				firstRelativeVel = relativeVel;
			}
		}

		if (firstTarget.posicion.x == -1 && firstTarget.posicion.y == -1) {
			log("NO HAY A QUIEN ESQUIVAR!!");
			return steering;
		}
		if (firstMinSeparation <= 0 || firstDistance < 2 * colRadius) {
			relativePos = firstTarget.posicion - character.posicion;
		}
		else {
			relativePos = firstRelativePos + firstRelativeVel * timeToCollision;
		}
		relativePos.normalize();
		steering.linear = -relativePos * character.maxAccel;
		steering.angular = 0;
		return steering;
	}

	void update(gameObject &character, std::vector<gameObject*> &targets, float delta) {
		steeringOutput steering = getSteering(character, targets);
		//log("Bug: %f %f %lf", steering.linear.x, steering.linear.y, steering.angular);
		character.update(steering, delta);
	}
};

class formation {

public:
	arrive llegar;
	align alinear;

	steeringOutput getSteering(gameObject &character, gameObject &targetPos, gameObject &targetOrient) {
		steeringOutput steering;
		//log("Orientacion mia %f", character.orientacion);
		llegar.set(5.0, 100.0);
		alinear.set(1.0, 25.0);
		steering.linear = (llegar.getSteering(character, targetPos)).linear;
		steering.angular = (alinear.getSteering(character, targetOrient)).angular;
		//log("Lineal %f %f %f Angular %f", steering.linear.x, steering.linear.y, steering.linear.z, steering.angular);
		return steering;
	}

	void update(gameObject &character, gameObject &targetPos, gameObject &targetOrient, float delta) {
		steeringOutput steering = getSteering(character, targetPos, targetOrient);
		character.update(steering, delta);
	}

};