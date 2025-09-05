#include "StageInputHandler.h"

/// Engine
#include "Engine/System/Input/Input.h"

bool StageInputHandler::UndoInput(){
	for(const auto& key : kUndoKey_){
		if(Input::IsTriggerKey(key)){
			return true;
		}
	}
	for(const auto& button : kUndoButton_){
		if(Input::IsTriggerButton(button)){
			return true;
		}
	}
	return false;
}
bool StageInputHandler::RedoInput(){
	for(const auto& key : kRedoKey_){
		if(Input::IsTriggerKey(key)){
			return true;
		}
	}
	for(const auto& button : kRedoButton_){
		if(Input::IsTriggerButton(button)){
			return true;
		}
	}
	return false;
}
bool StageInputHandler::ResetInput(){
	for(const auto& key : kResetKey_){
		if(Input::IsPressKey(key)){
			return true;
		}
	}
	for(const auto& button : kResetButton_){
		if(Input::IsPressButton(button)){
			return true;
		}
	}
	return false;
}