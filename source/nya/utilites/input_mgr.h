#pragma once

#include "nn/hid.h" 

namespace nya::hid { 
    extern nn::hid::NpadBaseState curControllerState;
    extern nn::hid::NpadBaseState prevControllerState; 

    extern nn::hid::NpadBaseState curhControllerState;
    extern nn::hid::NpadBaseState prevhControllerState;

    extern nn::hid::KeyboardState curKeyboardState;
    extern nn::hid::KeyboardState prevKeyboardState;

    extern nn::hid::MouseState curMouseState;
    extern nn::hid::MouseState prevMouseState; 

    extern ulong selectedPort;

    extern bool isReadInput;
    extern bool toggleInput;

    bool tryGetContState(nn::hid::NpadBaseState *state, ulong port);

    char getKeyValue(nn::hid::KeyboardKey key, bool isUpper, bool isModifier);

    void updatePadState();

    void setPort(ulong port); 

    void initKBM();

    // controller inputs
    bool isButtonHold(nn::hid::NpadButton button);

    bool isButtonPress(nn::hid::NpadButton button);

    bool isButtonRelease(nn::hid::NpadButton button);

    // input disabling

    // keyboard key presses

    bool isKeyHold(nn::hid::KeyboardKey key);

    bool isKeyPress(nn::hid::KeyboardKey key);

    bool isKeyRelease(nn::hid::KeyboardKey key);

    bool isModifierActive(nn::hid::KeyboardModifier modifier);

    // mouse inputs

    bool isMouseHold(nn::hid::MouseButton button);

    bool isMousePress(nn::hid::MouseButton button);

    bool isMouseRelease(nn::hid::MouseButton button);

    bool isMouseConnected();

    // mouse coordinate getters
    void getMouseCoords(float *x, float *y);

    void getScrollDelta(float *x, float *y);

    // specific button funcs
    bool isHoldA(); 
    bool isPressA();
    bool isReleaseA();
    bool isHoldB();
    bool isPressB();
    bool isReleaseB();
    bool isHoldX();
    bool isPressX();
    bool isReleaseX();
    bool isHoldY();
    bool isPressY();
    bool isReleaseY();
    bool isHoldL();
    bool isPressL();
    bool isReleaseL();
    bool isHoldR();
    bool isPressR();
    bool isReleaseR();
    bool isHoldZL();
    bool isPressZL();
    bool isReleaseZL();
    bool isHoldZR();
    bool isPressZR();
    bool isReleaseZR();
    bool isHoldPadLeft();
    bool isPressPadLeft();
    bool isReleasePadLeft();
    bool isHoldPadRight();
    bool isPressPadRight();
    bool isReleasePadRight();
    bool isHoldPadUp();
    bool isPressPadUp();
    bool isReleasePadUp();
    bool isHoldPadDown();
    bool isPressPadDown();
    bool isReleasePadDown();
    bool isHoldStickL();
    bool isPressStickL();
    bool isReleaseStickL();
    bool isHoldStickR();
    bool isPressStickR();
    bool isReleaseStickR();
    bool isPressMinus(); 
    bool isReleaseMinus(); 
    void disableButtons(nn::hid::NpadBaseState* stateded); 
    void install_hooks(); 
}