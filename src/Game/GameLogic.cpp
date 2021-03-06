/*
Created by PlasmaLotus
Updated May 13, 2017
*/
#include "GameLogic.h"
#include "../States/StateManager.h"
#include "Powerup\FirePowerup.h"
#include "Powerup/RapidFirePowerup.h"

const float GameLogic::PLAYER_MAX_VELOCITY(1.3f);
const float GameLogic::PLAYER_MAX_VELOCITY_DECREASE_RATE(0.06f);
const float GameLogic::PLAYER_MAX_VELOCITY_CAP_DECREASE_RATE(0.15f);
const float GameLogic::PLAYER_FRICTION(0.98f);
const float GameLogic::PLAYER_SHIELD_FRICTION{ 0.91f };
const float GameLogic::PLAYER_VELOCITY_DEAD_ZONE(0.00001f);
const float GameLogic::PLAYER_ACCELERATION_RATE(0.029f);
const float GameLogic::PLAYER_DASH_VELOCITY(1.8f);
const float GameLogic::PLAYER_MINIMUM_DASH_VELOCITY(1.8f);
const float GameLogic::PLAYER_SHIELD_RADIUS(45.0f);

const float GameLogic::PROJECTILE_HITBOX_RADIUS_MINIMUM( 3.f );
const float GameLogic::PROJECTILE_HITBOX_RADIUS_MAXIMUM( 20.f );
const float GameLogic::PROJECTILE_SPEED_MINIMUM( 2.5f );
const float GameLogic::PROJECTILE_SPEED_MAXIMUM( 5.0f );
const float GameLogic::PROJECTILE_BUBBLE_SPEED_MINIMUM(1.2f);
const float GameLogic::PROJECTILE_BUBBLE_SPEED_MAXIMUM(2.5f);
const float GameLogic::PROJECTILE_BUBBLE_HITBOX_RADIUS_MINIMUM(15.f);
const float GameLogic::PROJECTILE_BUBBLE_HITBOX_RADIUS_MAXIMUM(15.f);
const float GameLogic::ENTITY_MINIMUM_RADIUS{ 3.0f };
const float GameLogic::ENTITY_MINIMUM_WIDTH{ 3.0f };
const float GameLogic::ENTITY_MINIMUM_HEIGHT{ 3.0f };

const float GameLogic::ENERGY_MAX_RADIUS{ 15.0f };
const float GameLogic::ENERGY_MINIMUM_RADIUS{ 6.f };
const float GameLogic::POWERUP_RADIUS{ 30.f };
const float GameLogic::GAME_SHIELD_ENERGY_LOSS_MULTIPLYER{ 3.0f };//0.9
/*
const float GameLogic::GAME_SHOT_ENERGY_LOSS_MULTIPLYER{ 0.90f };
const float GameLogic::GAME_DASH_ENERGY_LOSS_MULTIPLYER{ 0.90f };
*/

GameLogic::GameLogic() :
	GameLogic(2){
}

GameLogic::GameLogic(int nbPlayers) :
	_nbPlayers(nbPlayers),
	gameState(GameCurrentState::COUNTDOWN),
	_init(false) {
	init();
}

GameLogic::~GameLogic(){
	_playerIDs.clear();
	_emptyAllEntities();
	_entities.clear();
	_spawnPoints.clear();
}

void GameLogic::init() {
	countdownTimer = GAME_COUNTDOWN_TIME;
	_countdownIt = 0;
	_playerIDs.clear();
	_init = false;
	_gameEnd = false;
	_totalDT = 0;
	_spawnPoints.clear();
	_spawnTimerEnergy = 0;
	_spawnTimerPowerup = 0;
	map.generateMap(1);
	
	Player *p1{ new Player(this, map.getSpawnPoint(1)) };
	Player *p2{ new Player(this, map.getSpawnPoint(2)) };
	Player *p3{ new Player(this, map.getSpawnPoint(3)) };
	Player *p4{ new Player(this, map.getSpawnPoint(4)) };

	p3->color = RGBA(128, 0, 128, 255);
	p2->color = RGBA(240, 30, 30, 255);
	p1->color = RGBA(10, 250, 250, 255);
	p4->color = RGBA(255, 255, 0, 255);

	_playerIDs.push_back(p1->getID());
	__playerIDs[0] = p1->getID();
	_playerIDs.push_back(p2->getID());
	__playerIDs[1] = p2->getID();
	_playerIDs.push_back(p3->getID());
	__playerIDs[2] = p3->getID();
	_playerIDs.push_back(p4->getID());
	__playerIDs[3] = p4->getID();

	switch (_nbPlayers) {
	case 4:
		_entities.push_back(p4);
	case 3:
		_entities.push_back(p3);
	case 2:
		_entities.push_back(p2);
	case 1:
		_entities.push_back(p1);
	default: break;

	}

	/*
	_entities.push_back(p1);
	_entities.push_back(p2);
	//_entities.push_back(p3);
	//_entities.push_back(p4);
	*/
	
	frame = 0;
	StateManager::getInstance().eventManager.queueEvent(Event(EventType::GameStart));
	//	StateManager::getInstance().eventManager.queueEvent(Event(EventType::Countdown3));

	_init = true;
}

void GameLogic::reset() {
	_emptyAllEntities();
	_entities.clear();
	init();
	//countdownTimer 
	gameState = GameCurrentState::COUNTDOWN;
}

void GameLogic::tick(int dt) {
	if (_init) {
		switch (gameState)
		{
		case::GameCurrentState::COUNTDOWN: {
			countdownTimer -= dt;
			int it = 3;
			if (countdownTimer > GAME_COUNTDOWN_TIME / 3 * 2) {
				it = 2;
			}
			if (countdownTimer > GAME_COUNTDOWN_TIME / 3) {
				it = 1;
			}
			if (it != _countdownIt) {
				if (it == 3) {
					StateManager::getInstance().eventManager.queueEvent(Event(EventType::Countdown3));
				}
				if (it == 2) {
					StateManager::getInstance().eventManager.queueEvent(Event(EventType::Countdown2));
				}
				if (it == 1) {
					StateManager::getInstance().eventManager.queueEvent(Event(EventType::Countdown1));
				}
			}
			_countdownIt = it;

			if (countdownTimer <= 0) {
				gameState = GameCurrentState::RUNNING;
				countdownTimer = 0;
				StateManager::getInstance().eventManager.queueEvent(Event(EventType::CountdownStart));
			}
			break;
		}
		case GameCurrentState::RUNNING: {
			//printf("GameTick:  DT - %ld\n", dt);
			frame++;
			_totalDT += dt;
			_spawnTimerEnergy += dt;
			_spawnTimerPowerup += dt;
			if (_spawnTimerPowerup >= GAME_POWERUP_SPAWN_TIMER) {
				_spawnTimerPowerup -= GAME_POWERUP_SPAWN_TIMER;
				_spawnPowerUps();
			}
			if (_spawnTimerEnergy >= GAME_ENERGY_SPAWN_TIMER) {
				_spawnTimerEnergy -= GAME_ENERGY_SPAWN_TIMER;
				_spawnEnergy();
			}
			printf("MaxVelocity:%3.5f - Game Frame: %d\n", PLAYER_MAX_VELOCITY, frame);
			_handleEntitiesUpdate(dt);
			_handleEntitiesCollisions(dt);
			_handleMapCollision(dt);
			_handleEntitesEnd();
			_handleGameEnd();
			break;
		}
		case::GameCurrentState::ENDED: {
			countdownTimer -= dt;
			int it = 3;
			if (countdownTimer > GAME_COUNTDOWN_TIME / 3 * 2) {
				it = 2;
			}
			if (countdownTimer > GAME_COUNTDOWN_TIME / 3) {
				it = 1;
			}
			if (it != _countdownIt) {
				if (it == 2) {
					StateManager::getInstance().eventManager.queueEvent(Event(EventType::Countdown2));
				}
				if (it == 1) {
					StateManager::getInstance().eventManager.queueEvent(Event(EventType::Countdown1));
				}
			}
			_countdownIt = it;

			/*
			if (countdownTimer <= 0) {
			gameState = GameCurrentState::RUNNING;
			countdownTimer = 0;
			StateManager::getInstance().eventManager.queueEvent(Event(EventType::CountdownStart));
			}
			*/
			break;
		}

		default:
			break;
		}
	}
	

}

void GameLogic::pause() {
}

void GameLogic::_handleEntitiesUpdate(int32_t dt){
	printf("Volume of Entities: %d,\n", _entities.size());
	for (int i = _entities.size() - 1; i >= 0; i--) {
		Entity *e{ _entities.at(i) };
		if (e != nullptr) {
			e->update(dt);

			//Player Cast
			for (int pOffset = 0; pOffset < _playerIDs.size(); pOffset++) {
				if (e->getID() == _playerIDs.at(pOffset)) {
					try{
						Player *p = dynamic_cast<Player *>(e);
						printf("Player id-%d - HP:%d -  x:%3.2f - y:%3.2f | Ammo: %d/%d, %d/%d,\n", p->getID(), p->HP, p->posX, p->posY, p->ammo, PLAYER_MAX_AMMO, p->_ammoRechargeProgress, PLAYER_AMMO_RECHARGE_COOLDOWN);
						printf(" -- OriX:%3.2f - OriY:%3.2f | velocityX:%3.2f, velocityY:%3.2f\n", p->orientationX, p->orientationY, p->velocityX, p->velocityY);
					}
					catch (const std::bad_cast& cast){
					}
				}
			}
		
			if (!e->isAlive()) {
				addEntityIDToDelete(e->getID());
			}
		}
		else {
			_entities.erase(_entities.begin() + i);
		}
	}
}

void GameLogic::_handleEntitiesCollisions(int32_t dt){
	for (int i = _entities.size() - 1; i >= 0; i--) {
		Entity *e1 = _entities.at(i);
		if (e1 != nullptr) {
			int e1_x = e1->posX / 100;
			int e1_y = e1->posY / 100;
		
			for (int j = _entities.size() - 1; j >= 0; j--) {
				Entity *e2 = _entities.at(j);
				if (e2 != nullptr) {
					if (e1 != e2) {
						int e2_x = e2->posX / 100;
						int e2_y = e2->posY / 100;
						//Test if the 2 entities are even close to each other
						if ( std::abs(e1_x - e2_x) < 2 && std::abs(e1_y - e2_y) < 2) {
							if (e1->testCollision(e2)) {
								e1->handleCollision(e2);
								//printf("Collision Detected, ID:%d, ID:%d -  Distance: %3.3f!!!\n", e1->getID(), e2->getID(), e1->_distanceBetween(*e2));
							}
						}
					}
				}
			
			}
		}
	}
}

void GameLogic::_handleMapCollision(int32_t dt) {
	for (int i = _entities.size() - 1; i >= 0; i--) {
		Entity *e = _entities.at(i);
		if (map.testCollision(e)) {
			/*COllision handle inside TestCollision*/
			//Map *m{ &map };
			//e->handleCollision(m);
			printf("COLLISION WITH MAP at id: %d\n", e->getID());
		}

	}
}

void GameLogic::_handleEntitesEnd(){
	int id;
	while (!_idsToDestroy.empty()) {
		id = _idsToDestroy.top();
		
		for (int i = _entities.size() - 1; i >= 0; i--) {
			if (_entities.at(i)->getID() == id) {
				delete _entities.at(i);
				_entities.erase(_entities.begin() + i);
			}
		}
		_idsToDestroy.pop();
	}
}

void GameLogic::_handleGameEnd() {
	int pAlive = 0;
	for (int i = _entities.size() - 1; i >= 0; i--) {
		//for (int j = 0; j < _entities.size(); j++) {
		Entity *e = _entities.at(i);
		try {
			Player *p = dynamic_cast<Player *>(e);
			if (p != nullptr) {
				if (p->isAlive() && p->HP > 0) {
					pAlive++;
				}
			}
		}
		catch (const std::bad_cast& cast) {
		}
	}
	if (pAlive <= 1) {
		StateManager::getInstance().eventManager.queueEvent(Event(EventType::GameEnd));
		StateManager::getInstance().eventManager.queueEvent(Event(EventType::LastManStanding));
		gameState = GameCurrentState::ENDED;
		_gameEnd = true;
	}

	
	if (_totalDT >= GAME_MAXIMUM_GAME_TIME) {
		StateManager::getInstance().eventManager.queueEvent(Event(EventType::GameEnd));
		StateManager::getInstance().eventManager.queueEvent(Event(EventType::TimeLimit));
		gameState = GameCurrentState::ENDED;
		countdownTimer = 3000;
		_gameEnd = true;
	}
	else {

	}
}

void GameLogic::addEntityIDToDelete(int id){
	_idsToDestroy.push(id);
}

void GameLogic::_emptyAllEntities()
{
	for (int i = _entities.size() - 1; i >= 0; i--) {
		Entity* e = _entities.at(i);
		if (e != nullptr) {
			delete e;
		}
	}
}

void GameLogic::addEntity(Entity *e) {
	_entities.push_back(e);
}

Entity* GameLogic::findEntity(int id) {
	for (int i = 0;  i < _entities.size(); i++) {
		if (_entities.at(i)->getID() == id) {
			return _entities.at(i);
		}
	}
	return nullptr;
}

Entity& GameLogic::findEntityCopy(int id) {
	for (int i = 0; i < _entities.size(); i++) {
		if (_entities.at(i)->getID() == id) {
			return *_entities.at(i);
		}
	}
	Entity e;
	return e;
}

Player& GameLogic::findPlayer(int id) {
	for (int i = 0; i < _entities.size(); i++) {
		Entity *e = _entities.at(i);
		if (e->getID() == id) {
			try{
				Player *p = dynamic_cast<Player *>(e);
				return *p;
			}
			catch (const std::bad_cast& cast){
			}
		}
	}
}

Player * GameLogic::getPlayer(int playerNumber){
	int id = _playerIDs.at(playerNumber - 1);
	for (int i = 0; i < _entities.size(); i++) {
		Entity *e = _entities.at(i);
		if (e->getID() == id) {
			try{
				Player *p = dynamic_cast<Player *>(e);
				return p;
			}
			catch (const std::bad_cast& cast){
			}
		}
	}
	return nullptr;
}

Player GameLogic::getPlayerCopy(int playerNumber){
	int id = _playerIDs[playerNumber - 1];
	for (int i = 0; i < _entities.size(); i++) {
		Entity *e = _entities.at(i);
		if (e->getID() == id) {
			try{
				Player *p = dynamic_cast<Player *>(e);
				return *p;
			}
			catch (const std::bad_cast& cast){
			}
		}
	}
}

void GameLogic::_spawnItems(){
}

void GameLogic::_spawnEnergy(){
	int randValue = 1 + rand() % GAME_ENERGY_SPAWN_AMOUNT;
	for (int i = 0; i < randValue; i++) {
		Vector2 v = map.getRandomSpawnPoint(ENERGY_MINIMUM_RADIUS, ENERGY_MINIMUM_RADIUS);
		Energy *e{ new Energy(v, GAME_ENERGY_SPAWN_AURA) };
		addEntity(e);
	}
}

void GameLogic::_spawnPowerUps(){
	int randValue = 1 + rand() % GAME_POWERUP_SPAWN_AMOUNT ;
	for (int i = 0; i < randValue; i++) {
		Vector2 v = map.getRandomSpawnPoint(ENERGY_MINIMUM_RADIUS, ENERGY_MINIMUM_RADIUS);
		int whichPowerup = rand() % 3;
		PowerUpItem* item(nullptr);
		switch (whichPowerup) {
		
		case 0:
		{
			item = new PowerUpItem(this, new BubblePowerUp(this), v.x, v.y);
			break;
		}
		case 1:
		{
			item = new PowerUpItem(this, new FirePowerUp(this), v.x, v.y);

			break;
		}
		case 2:
		default: {
				item = new PowerUpItem(this, new RapidFirePowerUp(this), v.x, v.y);
				break;
			}
		}
		
		addEntity(item);
	}
}
