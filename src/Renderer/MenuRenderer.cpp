﻿/*
Created by PlasmaLotus
Updated May 13, 2017
*/

#include "MenuRenderer.h"

MenuRenderer::MenuRenderer()
{
}

MenuRenderer::MenuRenderer(sf::RenderWindow *w, Menu *m):
	window(w),
	menu(m)
{
	initRenderer();
}

MenuRenderer::~MenuRenderer()
{
	_menuItems.clear();
}

void MenuRenderer::render()
{
	//Renderer::render();
	
	//clear();
	update();
	draw();
	display();
	
}

bool MenuRenderer::initRenderer() {
	//sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	//debugText.setString("Debug Text");
	/*
	debugText.setFillColor(sf::Color::Magenta);
	debugText.setOutlineColor(sf::Color::White);
	debugText.setPosition(sf::Vector2f(100, 100));
	debugText.setCharacterSize(64);
	*/
	nbMenuItems = menu->getItemsSize();

	//if (!font.loadFromFile("Assets\fonts\Minecraft.ttf")) {
	if (!font.loadFromFile("Assets\fonts\arial.ttf")) {
		fontLoaded = false;
	}

	//if (!font.loadFromFile("Assets\fonts\LemonMilk.LemonMilk.otf")) {
	/*
	if (!font.loadFromFile("Assets/Fonts/LemonMilk/LemonMilk.otf")){
		fontLoaded = false;
		//return EXIT_FAILURE;
		//resources/sansation.ttf
		//fontLoaded = false;
		//sf::Font::
		//font.loadFromMemory("", 367112);
	}

	debugText.setFont(font);
	debugText.setString("Sf Text Test");
	debugText.setCharacterSize(24);
	//debugText.setFillColor(sf::Color::Red);
	debugText.setStyle(sf::Text::Bold | sf::Text::Underlined);
	debugText.setPosition(50, 50);
	*/
	/*
	float x = window->getView().getCenter().x;
	float y = window->getView().getCenter().y;
	//sf::Vector2f targetSize(window->getViewport(window->getView()).width, window->getViewport(window->getView()).height);
	*/
	sf::Vector2f targetSize = window->getView().getSize();
	sf::Vector2f targetCenter = window->getView().getCenter();// ->getView()->getCenter();

	for (int i = 0; i < menu->getItemsSize(); i++) {
		//menuItems[i] = sf::RectangleShape(sf::Vector2f(100, 50));
		//menuItems[i].setPosition(50, i * 100);
		//menuItems[i].setFillColor(sf::Color::Magenta);
		_menuItems.push_back(MenuItemDrawable(&menu->getItems().at(i), targetCenter.x, targetCenter.y - (75 * nbMenuItems) +  i*75));
		//_menuItems[i].setFont(font);
	}
	bool success = false;

	return success;
}


void MenuRenderer::clear() {
	window->clear();
}

void MenuRenderer::update() {
	for (int i = 0; i < menu->getItemsSize(); i++) {
		
		if (menu->getSelection() == i) {
			_menuItems.at(i).isSelected(true);
		}
		else {
			_menuItems.at(i).isSelected(false);
		}
		
		_menuItems.at(i).update();
	}
}

void MenuRenderer::draw()
{
	//window->draw(shape);
	/*
	sf::Font font;
	*/
	int y = 100;


	//window->draw(debugText);
	for (int i = 0; i < menu->getItemsSize(); i++) {
		if (menu->getSelection() == i){
			menuItems[i].setFillColor(sf::Color::Cyan);
		}
		else {
			menuItems[i].setFillColor(sf::Color::Magenta);
		}
		window->draw(_menuItems.at(i));
	}

}

void MenuRenderer::display() {
	window->display();
}
void MenuRenderer::addMenu(Menu * m)
{
	menu = m;
}
void MenuRenderer::addWindow(sf::RenderWindow* g) {
	window = g;
}
sf::Texture MenuRenderer::getLastFrame() {
	return lastFrame;
}

