﻿/*
Created by PlasmaLotus
Updated May 13, 2017
*/
#include "GameRenderer.h"
#include "../States/StateManager.h"
#include "GameDrawable\AOEProjectileDrawable.h"
#include "../Game/Projectiles/AOEProjectile.h"
#include "../Game/Powerup/BubblePowerup.h"
#include "../Game/Powerup/FirePowerup.h"
#include "../Game/Powerup/NormalPowerup.h"
#include "../Game/Powerup/RapidFirePowerup.h"
#include "../Game/Powerup/Powerup.h"

GameRenderer::GameRenderer(){
}

GameRenderer::GameRenderer(sf::RenderWindow *w, GameLogic *g):
	window(w),
	game(g){
	initRenderer();
}

GameRenderer::~GameRenderer(){
}

void GameRenderer::render(){
	update();
	draw();
	display();
}

bool GameRenderer::initRenderer() {

	bool success = true;
	shape.setFillColor(sf::Color::Blue);
	shape.setPosition(100, 100);
	shape.setRadius(size);
	mapDrawable = new MapDrawable(&game->map);

	_frame = new sf::RenderTexture();
	
	_displayFPS = StateManager::getInstance().getDisplayFPS();
	_displayPositions = StateManager::getInstance().getDisplayPlayerPositions();
	_displayHitboxes = StateManager::getInstance().getDisplayHitboxes();
	
	/*
	_displayFPS = true;
	_displayPositions = true;
	_displayHitboxes = true;
	*/
	font = new sf::Font();
	//if (!font->loadFromFile("Assets/Fonts/Minecraft.ttf")) {
	if (!font->loadFromFile("Assets/Fonts/arial.ttf")) {
		printf("Unable to load Font\n");
		success = false;
	}
	
	playerTexture = new sf::Texture();
	if (!playerTexture->loadFromFile("./Assets/Images/player2.png")) {
		printf("Unable to load Player PNG\n");
	}

	projectileGeneralTexture = new sf::Texture();
	if (!projectileGeneralTexture->loadFromFile("./Assets/Images/Player_Shot.png")) {
		printf("Unable to load Player PNG\n");
	}

	iconBubbleTexture = new sf::Texture();
	if (!iconBubbleTexture->loadFromFile("./Assets/Images/Kirby/bubble.png")) {
		printf("Unable to load Icon PNG\n");
	}
	
	iconFireTexture = new sf::Texture();
	if (!iconFireTexture->loadFromFile("./Assets/Images/Kirby/fire.png")) {
		printf("Unable to load Icon PNG\n");
	}
	
	iconDefaultTexture = new sf::Texture();
	if (!iconDefaultTexture->loadFromFile("./Assets/Images/Kirby/mana.png")) {
		printf("Unable to load Icon PNG\n");
	}

	iconRapidTexture = new sf::Texture();
	if (!iconRapidTexture->loadFromFile("./Assets/Images/Kirby/rapid.png")) {
		printf("Unable to load Icon PNG\n");
	}
	//iconRapidTexture

	playerDrawable1 = new PlayerDrawable(game->getPlayer(1),this, *playerTexture, *font);
	playerDrawable2 = new PlayerDrawable(game->getPlayer(2),this, *playerTexture, *font);
	playerDrawable3 = new PlayerDrawable(game->getPlayer(3), this, *playerTexture, *font);
	playerDrawable4 = new PlayerDrawable(game->getPlayer(4), this, *playerTexture, *font);

	playerDrawable1->setDisplayHitboxes(_displayHitboxes);
	playerDrawable2->setDisplayHitboxes(_displayHitboxes);
	playerDrawable3->setDisplayHitboxes(_displayHitboxes);
	playerDrawable4->setDisplayHitboxes(_displayHitboxes);
	//playerDrawable2->playerColor = sf::Color::Magenta;
	
	/*
	// player 1 (left side of the screen)
	p1View.setViewport(sf::FloatRect(0, 0, 0.5f, 1));

	// player 2 (right side of the screen)
	p2View.setViewport(sf::FloatRect(0.5f, 0, 0.5f, 1));
	*/

	/*
	p1View.zoom(2.0f);
	p2View.zoom(2.0f);
	*/

	// mini-map (upper-right corner)
	//minimapView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
	/*
	window->setView(p1View);
	window->setView(p2View);
	*/
	//window->view
	mainView = sf::View(sf::Vector2f(1080 / 2, 720 / 2), sf::Vector2f(1080, 720));
	window->setView(mainView);
	//window->setView(minimapView);
	playerUIDrawable = new PlayerUIDrawable(game, game->getPlayer(1), game->getPlayer(2), game->getPlayer(3), game->getPlayer(4), *playerTexture, *font, &mainView);
	return success;
}

void GameRenderer::addPlayerAlert(Player * p, std::string text){
	if (playerDrawable1->player == p) {
		playerDrawable1->addAlert(text);
	}
	else if (playerDrawable2->player == p) {
		playerDrawable1->addAlert(text);
	}
	else{}
}

void GameRenderer::addPlayerAlert(int playerID, std::string text){
	Player&p = game->findPlayer(playerID);
	addPlayerAlert(&p, text);
}

void GameRenderer::clear() {
	window->clear();
}

void GameRenderer::draw(){
	
	drawMap(window);
	drawParticleSystems(window);

	drawProjectiles(window);
	drawItems(window);
	drawPlayers(window);
	
	handleViews(window);
	drawUI(window);
	__showFPS();
	__showPlayerPositions();
	//window->draw();
}

sf::RenderTexture* GameRenderer::getLastFrame() {
	drawMap(_frame);
	drawParticleSystems(_frame);

	drawProjectiles(_frame);
	drawItems(_frame);
	drawPlayers(_frame);

	handleViews(_frame);
	drawUI(_frame);
	return _frame;
}

/*
void GameRenderer::_draw(sf::RenderTarget* target) {
	
	drawMap();
	drawParticleSystems();

	drawProjectiles();
	drawItems();
	drawPlayers();

	handleViews();
	__showFPS();
	__showPlayerPositions();
	//window->draw();
}
*/

void GameRenderer::display() {
	window->display();
}

void GameRenderer::update(){

	/*Update players*/

	playerDrawable1->setViewSettings(mainView.getCenter(), mainView.getSize());
	playerDrawable2->setViewSettings(mainView.getCenter(), mainView.getSize());
	playerDrawable3->setViewSettings(mainView.getCenter(), mainView.getSize());
	playerDrawable4->setViewSettings(mainView.getCenter(), mainView.getSize());

	playerDrawable1->update();
	playerDrawable2->update();
	playerDrawable3->update();
	playerDrawable4->update();

	/*
	playerDrawable1->setDisplayHitboxes(_displayHitboxes);
	playerDrawable2->setDisplayHitboxes(_displayHitboxes);
	playerDrawable3->setDisplayHitboxes(_displayHitboxes);
	playerDrawable4->setDisplayHitboxes(_displayHitboxes);
	*/

	/*Update Map*/
	mapDrawable->setViewSettings(mainView.getCenter(), mainView.getSize());
	mapDrawable->update();

	/*Update Particles*/
	updateParticleSystems(StateManager::getInstance().getElapsedTime());

}

void GameRenderer::reset() {
	playerDrawable1 = new PlayerDrawable(game->getPlayer(1),this, *playerTexture, *font);
	playerDrawable2 = new PlayerDrawable(game->getPlayer(2),this, *playerTexture, *font);
	playerDrawable3 = new PlayerDrawable(game->getPlayer(3), this, *playerTexture, *font);
	playerDrawable4 = new PlayerDrawable(game->getPlayer(4), this, *playerTexture, *font);

	playerDrawable1->setDisplayHitboxes(_displayHitboxes);
	playerDrawable2->setDisplayHitboxes(_displayHitboxes);
	playerDrawable3->setDisplayHitboxes(_displayHitboxes);
	playerDrawable4->setDisplayHitboxes(_displayHitboxes);

	if (playerUIDrawable != nullptr) {
		delete playerUIDrawable;
	}
	playerUIDrawable = new PlayerUIDrawable(game, game->getPlayer(1), game->getPlayer(2), game->getPlayer(3), game->getPlayer(4), *playerTexture, *font, &mainView);

}


void GameRenderer::drawPlayers(sf::RenderTarget* target){
	/*
	window->draw(*playerDrawable1);
	window->draw(*playerDrawable2);
	*/
	target->draw(*playerDrawable1);
	target->draw(*playerDrawable2);
	target->draw(*playerDrawable3);
	target->draw(*playerDrawable4);
}

void GameRenderer::drawProjectiles(sf::RenderTarget* target){
	std::vector<Entity*> vec = game->_entities;
	for (int i = 0; i < vec.size(); i++) {
		try {
			Projectile * p = dynamic_cast<Projectile *> (vec.at(i));
			if (p != nullptr) {

				ProjectileDrawable pro(p, *projectileGeneralTexture);
				pro.setDisplayHitboxes(_displayHitboxes);
				pro.setViewSettings(mainView.getCenter(), mainView.getSize());
				pro.update();
				target->draw(pro);
			}
		}
		catch (const std::bad_cast& cast){
		}
		try {
			AOEProjectile * p = dynamic_cast<AOEProjectile *> (vec.at(i));
			if (p != nullptr) {
				
				AOEProjectileDrawable pro(p, *projectileGeneralTexture);
				pro.setDisplayHitboxes(_displayHitboxes);
				pro.setViewSettings(mainView.getCenter(), mainView.getSize());
				pro.update();
				target->draw(pro);
			}
		}
		catch (const std::bad_cast& cast) {
		}
	}
}

void GameRenderer::drawMap(sf::RenderTarget* target){
	target->draw(*mapDrawable);
	//window->draw(*mapDrawable);
}

void GameRenderer::drawItems(sf::RenderTarget* target) {
	std::vector<Entity*> vec = game->_entities;
	for (int i = 0; i < vec.size(); i++) {
		
		try {
			PowerUpItem * p = dynamic_cast<PowerUpItem *> (vec.at(i));
			if (p != nullptr) {
				FirePowerUp * fp = dynamic_cast<FirePowerUp *> (p->powerUp);
				BubblePowerUp * bp = dynamic_cast<BubblePowerUp *> (p->powerUp);
				RapidFirePowerUp * rp = dynamic_cast<RapidFirePowerUp *> (p->powerUp);
				//NormalPowerUp * np = dynamic_cast<NormalPowerUp *> (p->powerUp);
				if (fp != nullptr) {
					GameItemDrawable id(p, *iconFireTexture);
					id.setDisplayHitboxes(_displayHitboxes);
					id.setViewSettings(mainView.getCenter(), mainView.getSize());
					id.update();
					target->draw(id);
				}
				else if (bp != nullptr) {
					GameItemDrawable id(p, *iconBubbleTexture);
					id.setDisplayHitboxes(_displayHitboxes);
					id.setViewSettings(mainView.getCenter(), mainView.getSize());
					id.update();
					target->draw(id);
				}
				else if (rp != nullptr) {
					GameItemDrawable id(p, *iconRapidTexture);
					id.setDisplayHitboxes(_displayHitboxes);
					id.setViewSettings(mainView.getCenter(), mainView.getSize());
					id.update();
					target->draw(id);
				}
				
				
			}
		}
		catch (const std::bad_cast& cast) {
		}

		try {
			Energy * p = dynamic_cast<Energy *> (vec.at(i));
			if (p != nullptr) {
				GameItemDrawable id(p, *iconDefaultTexture);
				id.setDisplayHitboxes(_displayHitboxes);
				id.setViewSettings(mainView.getCenter(), mainView.getSize());
				id.update();
				target->draw(id);
				/*
				sf::CircleShape circle;
				if (p->state == ItemState::ItemCooldown) {
					circle.setFillColor(sf::Color(100, 100, 100, 150));
				}
				else {
					circle.setFillColor(sf::Color(120, 120, 120, 255));
				}
				circle.setPosition(p->posX - p->width / 2, p->posY - p->width / 2);
				circle.setRadius(p->width);
				target->draw(circle);
				*/
			}
		}
		catch (const std::bad_cast& cast) {
		}
	}
}

void GameRenderer::addGame(GameLogic* g) {
	game = g;
}
void GameRenderer::addWindow(sf::RenderWindow* g) {
	window = g;
}
void GameRenderer::handleViews(sf::RenderTarget* target){
	//setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
	Player*p1 = game->getPlayer(1);
	Player*p2 = game->getPlayer(2);

	if (p1 != nullptr && p2 != nullptr) {

		float minX, minY, maxX, maxY, distX, distY;
		maxX = std::max(p1->posX, p2->posX);
		maxY = std::max(p1->posY, p2->posY);
		minX = std::min(p1->posX, p2->posX);
		minY = std::min(p1->posY, p2->posY);
		distX = abs(p1->posX - p2->posX);
		distY = abs(p1->posY - p2->posY);
		/*
		//p1View.move(p->posX - p->prevPosX, p->posY - p->prevPosY);
		p1View.setCenter(playerDrawable1->player->posX, playerDrawable1->player->posY);
		p2View.setCenter(playerDrawable2->player->posX, playerDrawable2->player->posY);
		*/

		//mainView.setViewport(sf::FloatRect(minX, minY, maxX - minX + 100, maxY - minY + 100));
		float some = 1280.f * 9.f / 16.f;
		float extraRoom = 480.f;
		//if (distX > window->getSize().x * 0.8f || distY > window->getSize().y  * 0.8f) {
			float newDistX = distX;
			float newDistY =  distY;
			if (distY * 16.f / 9.f > distX) {
				newDistX = distY * 16.f / 9.f;
				newDistY = newDistX * 9.f / 16.f;
			}
			if (distX * 9.f / 16.f > distY) {
				newDistY = distX * 9.f / 16.f;
				newDistX = newDistY * 16.f / 9.f;
			}
	
			//if (newDistX < 1280 && newDistY < 720) {
			if (newDistX < StateManager::getWindowWidth() && newDistY < StateManager::getWindowHeight()) {
				newDistX = 1.0f * StateManager::getWindowWidth();
				newDistY = 1.0f * StateManager::getWindowHeight();
			}
			sf::Vector2f viewSize(newDistX + extraRoom * 16.f / 9.f, newDistY + extraRoom * 9.f / 16.f);
			mainView.setSize(viewSize);
			//mainView.setSize(sf::Vector2f(newDistX + extraRoom * 16.f / 9.f , newDistY + extraRoom * 9.f / 16.f));
		
			///TODO Limit 
			///Limit camera dependant of  map;
			sf::Vector2f viewCenter(1.0f * distX / 2.f + minX, 1.0f * distY / 2.f + minY);
			if (viewCenter.x < window->getSize().x /2.f || viewCenter.y < window->getSize().y /2.f) {
				if (viewCenter.x < window->getSize().x /2.f) {
					viewCenter.x = window->getSize().x / 2.f;
				}
				if (viewCenter.y < window->getSize().y /2.f) {
					viewCenter.y = window->getSize().y / 2.f;
				}
			}
			
			mainView.setCenter(viewCenter);
			//window->setView(mainView);
			target->setView(mainView);
	}

}
void GameRenderer::updateParticleSystems(int dt)
{
	for (int i = _particleSystems.size() - 1; i >= 0; --i) {
		ParticleSystem *p{ _particleSystems.at(i) };
		p->update(dt);
		if (!p->isAlive()) {
			delete p;
			_particleSystems.erase(_particleSystems.begin() + i);
		}
	}
}

void GameRenderer::drawUI(sf::RenderTarget * target){
	playerUIDrawable->update();
	target->draw(*playerUIDrawable);
}

sf::Sprite GameRenderer::renderFrame()
{
	_frame->clear();
	getLastFrame();
	sf::Sprite sprite(_frame->getTexture());
	
	//_frame->draw(*playerDrawable1);
	return sprite;
}
void GameRenderer::drawParticleSystems(sf::RenderTarget* target)
{
	for (int i = _particleSystems.size() - 1; i >= 0; --i) {
		ParticleSystem *p{ _particleSystems.at(i) };
		window->draw(*p);
	}
}

void GameRenderer::setDisplayHitboxes(bool value) {
	_displayHitboxes = value;

	playerDrawable1->setDisplayHitboxes(_displayHitboxes);
	playerDrawable2->setDisplayHitboxes(_displayHitboxes);
	playerDrawable3->setDisplayHitboxes(_displayHitboxes);
	playerDrawable4->setDisplayHitboxes(_displayHitboxes);
}

void GameRenderer::setDisplayPositions(bool value) {
	_displayPositions = value;
}

void GameRenderer::setDisplayFPS(bool value) {
	_displayFPS = value;
}

void GameRenderer::toggleHitboxes()
{
	if (_displayHitboxes) {
		setDisplayHitboxes(false);
		StateManager::getInstance().m_alertManager.addAlert("HitBox Display turned OFF");
	}
	else {
		setDisplayHitboxes(true);
		StateManager::getInstance().m_alertManager.addAlert("HitBox Display turned ON");
	}
}

void GameRenderer::togglePositions()
{
	if (_displayPositions) {
		setDisplayPositions(false);
		StateManager::getInstance().m_alertManager.addAlert("Player Positions Display turned OFF");
	}
	else {
		setDisplayPositions(true);
		StateManager::getInstance().m_alertManager.addAlert("Player Positions Display turned ON");

	}
}

void GameRenderer::toggleFPS()
{
	if (_displayFPS) {
		setDisplayFPS(false);
		StateManager::getInstance().m_alertManager.addAlert("FPS Display turned OFF");
	}
	else {
		setDisplayFPS(true);
		StateManager::getInstance().m_alertManager.addAlert("FPS Display turned ON");

	}
}

void GameRenderer::__showFPS() {
	if (_displayFPS) {
		int windowY = window->getView().getCenter().y - window->getView().getSize().y / 2;
		int windowX = window->getView().getCenter().x - window->getView().getSize().x / 2;
		int fontSize = window->getView().getSize().x / 100;

		sf::Text fpsText("", playerDrawable1->m_font, fontSize);
		std::string fpsstring = "FPS: ";
		fpsstring += std::to_string(StateManager::getInstance().getFrameRate());
		fpsstring += " / ";
		fpsstring += std::to_string(StateManager::getInstance().FPS);
		fpsText.setString(fpsstring);
		//playerText1.setFont()
		fpsText.setFillColor(sf::Color::Magenta);
		fpsText.setPosition(windowX + 2, windowY + 2);
		window->draw(fpsText);
	}
}

void GameRenderer::__showPlayerPositions() {
	if (_displayPositions) {

		/*
		int windowY = window->getView().getViewport().top;
		int windowX = window->getView().getViewport().left;
		*/

		int windowY = window->getView().getCenter().y - window->getView().getSize().y / 2;
		int windowX = window->getView().getCenter().x - window->getView().getSize().x / 2;
		int fontSize = window->getView().getSize().x / 100;

		sf::Text playerText1("", playerDrawable1->m_font, fontSize);
		//playerText1.setFont()
		playerText1.setFillColor(sf::Color::Magenta);
		playerText1.setString("P1 - X: " + std::to_string((int)playerDrawable1->player->posX) + " Y: " + std::to_string((int)playerDrawable1->player->posY));
		playerText1.setPosition(windowX + 2, windowY + fontSize*2 + 2);
		window->draw(playerText1);


		sf::Text playerText2("", playerDrawable2->m_font, fontSize);
		playerText2.setFillColor(sf::Color::Magenta);
		playerText2.setString("P2 - X: " + std::to_string((int)playerDrawable2->player->posX) + " Y: " + std::to_string((int)playerDrawable2->player->posY));
		playerText2.setPosition(windowX + 2, windowY + fontSize*3 + 2);
		window->draw(playerText2);
	}
}

void GameRenderer::playerHitDisplay(float x, float y)
{
	ParticleSystem * p{ new ParticleSystem(50) };
	p->setEmitter(sf::Vector2f(x, y));
	_particleSystems.push_back(p);
}
