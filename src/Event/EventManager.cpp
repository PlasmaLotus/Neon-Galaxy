#include "EventManager.h"
//#include "../States/StateManager.h"
//#include "../Game/GameLogic.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
	_events.clear();
}
/*
EventManager& EventManager::getInstance() {
	static EventManager instance;
	return instance;
}
*/

void EventManager::queueEvent(Event e) {
	_events.push_back(e);
}

void EventManager::handleEvents(int dt)
{
	//int dt = StateManager::getInstance().getElapsedTime();
	/*
	for (std::vector<Event>::iterator i = _events.begin(); i != _events.end(); i++) {
		i->lifetime += dt;
		if (!i->handled) {
			__handleEvent(i->type, i->_id);
			i->handled = true;
		}
		if (i->lifetime >= DEFAULT_LIFETIME_DEATH) {
			_events.erase(i);
		}
	}
	*/
	//if 
	for (int i = _events.size() - 1; i >= 0; i--) {
		Event &event = _events.at(i);
		event.lifetime += dt;
		if (!event.handled) {
			if (__handleEvent(event)) {
				event.handled = true;
			}
		}
		if (event.lifetime >= DEFAULT_LIFETIME_DEATH) {
			_events.erase(_events.begin() + i);
		}
	}
}

void EventManager::setGame(GameLogic * g)
{
	game = g;
}

void EventManager::setRenderer(Renderer * r)
{
	renderer = r;
}

bool EventManager::__handleEvent(Event event){
	EventType e = event.type;
	//int id = even
	switch (e) {
	case EventType::ProjectileSpawn: {
		//Entity *e = game->findEntity(id);

		Projectile* p = dynamic_cast<Projectile*>(event.entity);
		__handleEventProjectileSpawn(p);
		printf("Event, PROJECTILE SPAWN -- PROJECTILE SPAWN -- PROJECTILE SPAWN!!!\n : ID: %d", event._id);
		break;
		}
	case EventType::Countdown1:
	case EventType::Countdown2:
	case EventType::Countdown3: {
		__handleEventCoundownTick();
		break;
	}
	case EventType::CountdownStart: {
		__handleEventCoundownStart();
		break;
	}
	case EventType::NoEvent:
	default: {	
		printf("Event, NOEVENT!!!\n : ID: %d", event._id);
		break;
		}
	}
	return true;
}

void EventManager::__handleEventProjectileSpawn(Projectile* projectile) {
	
	//bool loaded = sbuffer.loadFromFile("Assets/Sounds/Pew_Pew-DKnight556-1379997159.wav");
	//sound.setBuffer(sbuffer);
	//sound.setVolume(100.f);
	//sound.play();
	//sound.getSta
	audio->queueAudioEvent(AudioEvent::DefaultEventAudio, false);
}

void EventManager::__handleEventCoundownStart()
{
	audio->queueAudioEvent(AudioEvent::AudioCountdownTick, false);
}

void EventManager::__handleEventCoundownTick()
{
	audio->queueAudioEvent(AudioEvent::AudioCountdownStart, false);
}


void EventManager::setAudioEventManager(AudioEventManager *aem) {
	audio = aem;
}