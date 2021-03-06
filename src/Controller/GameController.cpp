/*
Created by PlasmaLotus
Updated May 13, 2017
*/
#include "GameController.h"
#include "../States/StateManager.h"
#include "../States/GameState.h"

/*The controller recieves input from a Keyboard or Joystick and outputs Commands to execute on the Board and Game*/
GameController::GameController(ControllerConfig* c, Player* e) :
	GameController(nullptr, c, e){
}

GameController::GameController(GameState *s, ControllerConfig * c, Player * e):
	Controller(c),
	player(e),
	state(s){
	mode = ControlMode::Joystick;
}

GameController::~GameController(){
}

/*Main input check call -- Checks if any input is pressed and acts accordingly*/
bool GameController::handleInput() {
	pAngleX = 0.f;
	pAngleY = 0.f;
	cAngleX = 0.f;
	cAngleY = 0.f;
	bool input = Controller::handleInput();
	player->setPlayerOrienation(pAngleX, pAngleY);
	/*
	if ((pAngleX >= config->joystickDeadZone || pAngleX <= -config->joystickDeadZone) &&
		(pAngleY >= config->joystickDeadZone || pAngleY <= -config->joystickDeadZone)) {
		player->commandMove(pAngleX, pAngleY);
	}
	*/
	if ((std::abs(pAngleX) > config->joystickDeadZone) || (std::abs(pAngleY) > config->joystickDeadZone)) {
		player->commandMove(pAngleX, pAngleY);
	}
	player->setCursorOrientation(cAngleX, cAngleY);
	player->setDashOrientation(pAngleX, pAngleY);
	return true;

}

/*When an input is detected, a command is raised to this function*/
void GameController::handleCommand(ControllerCommand command){
    /*Apply action on the menu dependant on the command*/
	Controller::handleCommand(command);
	if (player != NULL) {
		switch (command)
		{
		case ControllerCommand::Up:
		{
			pAngleY = -70.f;
			break;
		}
		case ControllerCommand::Down:
		{
			pAngleY = 70.f;
			break;
		}
		case ControllerCommand::Left:
		{
			pAngleX = -70.f; 
			break;
		}
		case ControllerCommand::Right:
		{
			pAngleX = 70.f;
			break;
		}
		case ControllerCommand::CursorUp:
		{
			cAngleY = -70.f;
			break;
		}
		case ControllerCommand::CursorDown:
		{
			cAngleY = 70.f;
			break;
		}
		case ControllerCommand::CursorLeft:
		{
			cAngleX = -70.f;
			break;
		}
		case ControllerCommand::CursorRight:
		{
			cAngleX = 70.f;
			break;
		}
		case ControllerCommand::Shoot:
		{
			player->commandShootAlt();
			break;
		}
		case ControllerCommand::ShootAlt:
		{
			player->commandShootAlt();
			break;
		}
		case ControllerCommand::Shield:
		{
			player->commandShield();
			break;
		}
		case ControllerCommand::Dash:
		{
			player->commandDash();
			break;
		}
		case ControllerCommand::Pause:
		{
			//menu->inputSelect();
			if (state != nullptr) {
				state->pause();
			}
			break;
		}
		case ControllerCommand::Cheat:
		{
			//board->Cheat();
			break;
		}
		case ControllerCommand::NoInput:
		default: break;
		}
	}
}

void GameController::handleJoystickAxis(sf::Joystick::Axis axis) {
	Controller::handleJoystickAxis(axis);
	AxisHandler handler = config->getAxisHandlerFromAxis(axis);
	float value = sf::Joystick::getAxisPosition(config->getJoystickNumber(), axis);
	//printf("Joystick%d\n", axis);
	//arbitrary dead zone of 7
	
	//Z AXIS, LEFT AND RIGHT TRIGGERS

		//printf("Joystick%d - in switch\n", axis);
		ControllerCommand command;
		if (value > 0) {
			command = handler.positif;
		}
		else {
			command = handler.negatif;
		}

		if (axis == sf::Joystick::Axis::Z) {

		}
		

		switch (command) {
			case ControllerCommand::Up:
			case ControllerCommand::Down:
			case ControllerCommand::PVertical:
			{
				if (value >= config->joystickDeadZone || value <= -config->joystickDeadZone) {
					pAngleY = value;
				}
				break;
			}
			case ControllerCommand::Left:
			case ControllerCommand::Right:
			case ControllerCommand::PHorizontal:
			{
				if (value >= config->joystickDeadZone || value <= -config->joystickDeadZone) {
					pAngleX = value;
				}
				break;
			}

			case ControllerCommand::CursorUp:
			case ControllerCommand::CursorDown:
			case ControllerCommand::CVertical:
			{
				//cAngleY = value;
				if (axis == sf::Joystick::Axis::PovY) {
					cAngleY = -value;
				}
				else {
					cAngleY = value;
				}
				//if (value >= config->joystickDeadZone || value <= -config->joystickDeadZone) {
				/*
				if (value >= 20.f || value <= -20.f) {
					cAngleY = value;
				}
				 */
				break;
			}

			case ControllerCommand::CursorLeft:
			case ControllerCommand::CursorRight:
			case ControllerCommand::CHorizontal:
			{
				cAngleX = value;
				/*
				if (value >= config->joystickDeadZone || value <= -config->joystickDeadZone) {
					cAngleX = value;
				}
				*/
				break;
			}
			case ControllerCommand::Shoot:
			case ControllerCommand::ShootAlt: {
				if (value >= config->joystickDeadZone || value <= -config->joystickDeadZone) {
					player->commandShootAlt();
				}
				break;
			}
			case ControllerCommand::Dash: {
				if (value >= config->joystickDeadZone || value <= -config->joystickDeadZone) {
					player->commandDash();
				}
				break;
			}
			case ControllerCommand::Shield: {
				if (value >= config->joystickDeadZone || value <= -config->joystickDeadZone) {
					player->commandShield();
				}
				break;
			}
			default:
				break;
		}
}

void GameController::handleMouseAxis() {
	sf::Vector2i mousePos = sf::Mouse::getPosition(*StateManager::getInstance().getWindow());
	//printf("HandleMouseAxis - MousePos: %d, %d\n", mousePos.x, mousePos.y);
	if (mode == ControlMode::Keyboard) {
		//printf("ControllerModeKeyboardMouse\n");
		//player->setCursorOrientationFromMouse(mousePos.x, mousePos.y);
		//sf::Mouse::setPosition(sf::Vector2i(100, 100));
	}
}
void GameController::setPlayer(Player* e){
        player = e;
}

void GameController::setState(GameState * s){
	state = s;
}

