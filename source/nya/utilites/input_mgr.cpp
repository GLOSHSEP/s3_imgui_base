#include "input_mgr.h"
#include "hid.h"
#include "hook/trampoline.hpp"
#include "nya.h"

namespace nya::hid { 
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
    nn::hid::NpadBaseState prevControllerState{};
    nn::hid::NpadBaseState curControllerState{}; 
    //chad hand held controller  
    nn::hid::NpadBaseState prevhControllerState{};
    nn::hid::NpadBaseState curhControllerState{};

    nn::hid::KeyboardState curKeyboardState{};
    nn::hid::KeyboardState prevKeyboardState{};

    nn::hid::MouseState curMouseState{};
    nn::hid::MouseState prevMouseState{};

    ulong selectedPort = -1;
    bool isReadInput = true;
    bool toggleInput = false;

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

    void initKBM() {
        nn::hid::InitializeKeyboard();
        nn::hid::InitializeMouse();
    }

    void setPort(ulong port) { selectedPort = port; } 

    void updatePadState() {
        if (isMouseHold(nn::hid::MouseButton::Left)) { 
            //nya::nya_log("meow\n");

            prevControllerState = curControllerState;
            tryGetContState(&curControllerState, selectedPort); 

            prevhControllerState = curhControllerState;
            tryGetContState(&curhControllerState, 0x20); 
        }

        prevKeyboardState = curKeyboardState;
        nn::hid::GetKeyboardState(&curKeyboardState); 

        prevMouseState = curMouseState;
        nn::hid::GetMouseState(&curMouseState);
    }

    bool tryGetContState(nn::hid::NpadBaseState *state, ulong port) {
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

    bool isButtonHold(nn::hid::NpadButton button) {
        if (
            curControllerState.mButtons.Test((int) button) or 
            curhControllerState.mButtons.Test((int) button)
        ) { 
            return true; 
        } else { 
            return false;
        }
    }

    bool isButtonPress(nn::hid::NpadButton button) {
        if (
            (curControllerState.mButtons.Test((int) button) && !prevControllerState.mButtons.Test((int) button)) or 
            (curhControllerState.mButtons.Test((int) button) && !prevhControllerState.mButtons.Test((int) button)) 
        ) { 
            return true; 
        } else { 
            return false;
        }
    }

    bool isButtonRelease(nn::hid::NpadButton button) {
        if ( 
            (!curControllerState.mButtons.Test((int) button) && prevControllerState.mButtons.Test((int) button)) or 
            (!curhControllerState.mButtons.Test((int) button) && prevhControllerState.mButtons.Test((int) button)) 
        ) {
            return true;
        } else { 
            return false;
        }
    }

    bool isKeyHold(nn::hid::KeyboardKey key) {
        return curKeyboardState.keys.Test((int) key);
    }

    bool isKeyPress(nn::hid::KeyboardKey key) {
        return curKeyboardState.keys.Test((int) key) && !prevKeyboardState.keys.Test((int) key);
    }

    bool isKeyRelease(nn::hid::KeyboardKey key) {
        return !curKeyboardState.keys.Test((int) key) && prevKeyboardState.keys.Test((int) key);
    }

    bool isModifierActive(nn::hid::KeyboardModifier modifier) {
        return curKeyboardState.modifiers.Test((int) modifier);
    }

    bool isMouseHold(nn::hid::MouseButton button) {
        return curMouseState.buttons.Test((int) button);
    }

    bool isMousePress(nn::hid::MouseButton button) {
        return curMouseState.buttons.Test((int) button) && !prevMouseState.buttons.Test((int) button);
    }

    bool isMouseRelease(nn::hid::MouseButton button) {
        return !curMouseState.buttons.Test((int) button) && prevMouseState.buttons.Test((int) button);
    }

    bool isMouseConnected() {
        return curMouseState.attributes.Test((int) nn::hid::MouseAttribute::IsConnected);
    }

    void getMouseCoords(float *x, float *y) {
        *x = curMouseState.x;
        *y = curMouseState.y;
    }

    void getScrollDelta(float *x, float *y) {
        *x = curMouseState.wheelDeltaX;
        *y = curMouseState.wheelDeltaY;
    } 

    void disableButtons(nn::hid::NpadBaseState* stateded) {
        stateded->mButtons = nn::hid::NpadButtonSet();
        stateded->mAnalogStickL = nn::hid::AnalogStickState();
        stateded->mAnalogStickR = nn::hid::AnalogStickState();
    } 

    void disable_L(nn::hid::NpadBaseState* stateded) { 
        stateded->mButtons.Set(int(nn::hid::NpadButton::L), false);
    } 

    HOOK_DEFINE_TRAMPOLINE(GetNpadStatesFullKeyHook) {
        static void Callback(nn::hid::NpadBaseState* state, int a, const u32& b) {
            Orig(state, a, b); 
            if (toggleInput) disableButtons(state); 
        }
    };

    HOOK_DEFINE_TRAMPOLINE(GetNpadStatesHandheldHook) {
        static void Callback(nn::hid::NpadBaseState* state, int a, const u32& b) {
            Orig(state, a, b);
            if (toggleInput) disableButtons(state); 
        }
    };

    HOOK_DEFINE_TRAMPOLINE(GetNpadStatesJoyDualHook) {
        static void Callback(nn::hid::NpadBaseState* state, int a, const u32& b) {
            Orig(state, a, b);
            if (toggleInput) disableButtons(state);
        }
    };

    void install_hooks() { 
        GetNpadStatesFullKeyHook::InstallAtSymbol("_ZN2nn3hid13GetNpadStatesEPNS0_16NpadFullKeyStateEiRKj");
        GetNpadStatesHandheldHook::InstallAtSymbol("_ZN2nn3hid13GetNpadStatesEPNS0_17NpadHandheldStateEiRKj");
        GetNpadStatesJoyDualHook::InstallAtSymbol("_ZN2nn3hid13GetNpadStatesEPNS0_16NpadJoyDualStateEiRKj");
    }

    bool isHoldA() { return isButtonHold(nn::hid::NpadButton::A); }
    bool isPressA() { return isButtonPress(nn::hid::NpadButton::A); }
    bool isReleaseA() { return isButtonRelease(nn::hid::NpadButton::A); }
    bool isHoldB() { return isButtonHold(nn::hid::NpadButton::B); }
    bool isPressB() { return isButtonPress(nn::hid::NpadButton::B); }
    bool isReleaseB() { return isButtonRelease(nn::hid::NpadButton::B); }
    bool isHoldX() { return isButtonHold(nn::hid::NpadButton::X); }
    bool isPressX() { return isButtonPress(nn::hid::NpadButton::X); }
    bool isReleaseX() { return isButtonRelease(nn::hid::NpadButton::X); }
    bool isHoldY() { return isButtonHold(nn::hid::NpadButton::Y); }
    bool isPressY() { return isButtonPress(nn::hid::NpadButton::Y); }
    bool isReleaseY() { return isButtonRelease(nn::hid::NpadButton::Y); }
    bool isHoldL() { return isButtonHold(nn::hid::NpadButton::L); }
    bool isPressL() { return isButtonPress(nn::hid::NpadButton::L); }
    bool isReleaseL() { return isButtonRelease(nn::hid::NpadButton::L); }
    bool isHoldR() { return isButtonHold(nn::hid::NpadButton::R); }
    bool isPressR() { return isButtonPress(nn::hid::NpadButton::R); }
    bool isReleaseR() { return isButtonRelease(nn::hid::NpadButton::R); }
    bool isHoldZL() { return isButtonHold(nn::hid::NpadButton::ZL); }
    bool isPressZL() { return isButtonPress(nn::hid::NpadButton::ZL); }
    bool isReleaseZL() { return isButtonRelease(nn::hid::NpadButton::ZL); }
    bool isHoldZR() { return isButtonHold(nn::hid::NpadButton::ZR); }
    bool isPressZR() { return isButtonPress(nn::hid::NpadButton::ZR); }
    bool isReleaseZR() { return isButtonRelease(nn::hid::NpadButton::ZR); }
    bool isHoldPadLeft() { return isButtonHold(nn::hid::NpadButton::Left); }
    bool isPressPadLeft() { return isButtonPress(nn::hid::NpadButton::Left); }
    bool isReleasePadLeft() { return isButtonRelease(nn::hid::NpadButton::Left); }
    bool isHoldPadRight() { return isButtonHold(nn::hid::NpadButton::Right); }
    bool isPressPadRight() { return isButtonPress(nn::hid::NpadButton::Right); }
    bool isReleasePadRight() { return isButtonRelease(nn::hid::NpadButton::Right); }
    bool isHoldPadUp() { return isButtonHold(nn::hid::NpadButton::Up); }
    bool isPressPadUp() { return isButtonPress(nn::hid::NpadButton::Up); }
    bool isReleasePadUp() { return isButtonRelease(nn::hid::NpadButton::Up); }
    bool isHoldPadDown() { return isButtonHold(nn::hid::NpadButton::Down); }
    bool isPressPadDown() { return isButtonPress(nn::hid::NpadButton::Down); }
    bool isReleasePadDown() { return isButtonRelease(nn::hid::NpadButton::Down); }
    bool isHoldStickL() { return isButtonHold(nn::hid::NpadButton::StickL); }
    bool isPressStickL() { return isButtonPress(nn::hid::NpadButton::StickL); }
    bool isReleaseStickL() { return isButtonRelease(nn::hid::NpadButton::StickL); }
    bool isHoldStickR() { return isButtonHold(nn::hid::NpadButton::StickR); }
    bool isPressStickR() { return isButtonPress(nn::hid::NpadButton::StickR); }
    bool isReleaseStickR() { return isButtonRelease(nn::hid::NpadButton::StickR); }
    bool isPressMinus() { return isButtonPress(nn::hid::NpadButton::Minus); }
    bool isReleaseMinus() { return isButtonRelease(nn::hid::NpadButton::Minus); }
} 