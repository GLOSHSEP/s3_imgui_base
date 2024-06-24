#include "input_mgr.h"

static const char *styleNames[] = {
        "Pro Controller",
        "Joy-Con controller in handheld mode",
        "Joy-Con controller in dual mode",
        "Joy-Con left controller in single mode",
        "Joy-Con right controller in single mode",
        "GameCube controller",
        "Poké Ball Plus controller",
        "NES/Famicom controller",
        "NES/Famicom controller in handheld mode",
        "SNES controller",
        "N64 controller",
        "Sega Genesis controller",
        "generic external controller",
        "generic controller",
}; 

//virgin controllers 
nn::hid::NpadBaseState nya::hid::prevControllerState{};
nn::hid::NpadBaseState nya::hid::curControllerState{}; 
//chad hand held controller  
nn::hid::NpadBaseState nya::hid::prevhControllerState{};
nn::hid::NpadBaseState nya::hid::curhControllerState{};

nn::hid::KeyboardState nya::hid::curKeyboardState{};
nn::hid::KeyboardState nya::hid::prevKeyboardState{};

nn::hid::MouseState nya::hid::curMouseState{};
nn::hid::MouseState nya::hid::prevMouseState{};

ulong nya::hid::selectedPort = -1;
bool nya::hid::isReadInput = true;
bool nya::hid::toggleInput = false;

const char *getStyleName(nn::hid::NpadStyleSet style) {

    s32 index = -1;

    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleFullKey)) { index = 0; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleHandheld)) { index = 1; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleJoyDual)) { index = 2; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleJoyLeft)) { index = 3; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleJoyRight)) { index = 4; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleGc)) { index = 5; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStylePalma)) { index = 6; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleLark)) { index = 7; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleHandheldLark)) { index = 8; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleLucia)) { index = 9; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleLagon)) { index = 10; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleLager)) { index = 11; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleSystemExt)) { index = 12; }
    if (style.Test((int) nn::hid::NpadStyleTag::NpadStyleSystem)) { index = 13; }

    if (index != -1) {
        return styleNames[index];
    } else {
        return "Unknown";
    }
}

void nya::hid::initKBM() {
    nn::hid::InitializeKeyboard();
    nn::hid::InitializeMouse();
}

void nya::hid::updatePadState() {
    prevControllerState = curControllerState;
    tryGetContState(&curControllerState, selectedPort); 

    prevhControllerState = curhControllerState;
    tryGetContState(&curhControllerState, 0x20); 

    prevKeyboardState = curKeyboardState;
    nn::hid::GetKeyboardState(&curKeyboardState); 

    prevMouseState = curMouseState;
    nn::hid::GetMouseState(&curMouseState);
}

bool nya::hid::tryGetContState(nn::hid::NpadBaseState *state, ulong port) {

    nn::hid::NpadStyleSet styleSet = nn::hid::GetNpadStyleSet(port);
    isReadInput = true;
    bool result = true; 

    if (isReadInput) { 
        //cringe controlls 
        if (port != 0x20) { 
            if (styleSet.Test((int) nn::hid::NpadStyleTag::NpadStyleFullKey)) {
                nn::hid::GetNpadState((nn::hid::NpadFullKeyState *) state, port);
            } else if (styleSet.Test((int) nn::hid::NpadStyleTag::NpadStyleJoyDual)) {
                nn::hid::GetNpadState((nn::hid::NpadJoyDualState *) state, port);
            } else if (styleSet.Test((int) nn::hid::NpadStyleTag::NpadStyleJoyLeft)) {
                nn::hid::GetNpadState((nn::hid::NpadJoyLeftState *) state, port);
            } else if (styleSet.Test((int) nn::hid::NpadStyleTag::NpadStyleJoyRight)) {
                nn::hid::GetNpadState((nn::hid::NpadJoyRightState *) state, port);
            } else {
                result = false;
            } 
        //based hand held mode 
        } else { 
            nn::hid::GetNpadState((nn::hid::NpadHandheldState *) state, 0x20);
        } 
    } 

    isReadInput = false;

    return result;
}

bool nya::hid::isButtonHold(nn::hid::NpadButton button) {
    if (
        curControllerState.mButtons.Test((int) button) or 
        curhControllerState.mButtons.Test((int) button)
    ) { 
        return true; 
    } else { 
        return false;
    }
}

bool nya::hid::isButtonPress(nn::hid::NpadButton button) {
    if (
        curControllerState.mButtons.Test((int) button) && !prevControllerState.mButtons.Test((int) button) or 
        curhControllerState.mButtons.Test((int) button) && !prevhControllerState.mButtons.Test((int) button) 
    ) { 
        return true; 
    } else { 
        return false;
    }
}

bool nya::hid::isButtonRelease(nn::hid::NpadButton button) {
    if ( 
        !curControllerState.mButtons.Test((int) button) && prevControllerState.mButtons.Test((int) button) or 
        !curhControllerState.mButtons.Test((int) button) && prevhControllerState.mButtons.Test((int) button) 
    ) {
        return true;
    } else { 
        return false;
    }
}

bool nya::hid::isKeyHold(nn::hid::KeyboardKey key) {
    return curKeyboardState.keys.Test((int) key);
}

bool nya::hid::isKeyPress(nn::hid::KeyboardKey key) {
    return curKeyboardState.keys.Test((int) key) && !prevKeyboardState.keys.Test((int) key);
}

bool nya::hid::isKeyRelease(nn::hid::KeyboardKey key) {
    return !curKeyboardState.keys.Test((int) key) && prevKeyboardState.keys.Test((int) key);
}

bool nya::hid::isModifierActive(nn::hid::KeyboardModifier modifier) {
    return curKeyboardState.modifiers.Test((int) modifier);
}

bool nya::hid::isMouseHold(nn::hid::MouseButton button) {
    return curMouseState.buttons.Test((int) button);
}

bool nya::hid::isMousePress(nn::hid::MouseButton button) {
    return curMouseState.buttons.Test((int) button) && !prevMouseState.buttons.Test((int) button);
}

bool nya::hid::isMouseRelease(nn::hid::MouseButton button) {
    return !curMouseState.buttons.Test((int) button) && prevMouseState.buttons.Test((int) button);
}

bool nya::hid::isMouseConnected() {
    return curMouseState.attributes.Test((int) nn::hid::MouseAttribute::IsConnected);
}

void nya::hid::getMouseCoords(float *x, float *y) {
    *x = curMouseState.x;
    *y = curMouseState.y;
}

void nya::hid::getScrollDelta(float *x, float *y) {
    *x = curMouseState.wheelDeltaX;
    *y = curMouseState.wheelDeltaY;
} 