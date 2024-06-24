#include "nya/windows/intro.h" 
#include "imgui.h" 

namespace nya {
    namespace window { 
        void bar() { 
            if (nya::show_menu) { 
                ImGui::BeginMainMenuBar(); 
                    if (ImGui::BeginMenu("menu bar")) {
                        ImGui::Checkbox("show the welcome window", &nya::show_intro); 
                        ImGui::EndMenu();
                    } //nyaloader 
                ImGui::EndMainMenuBar();

            } //show menu 

        } //bar 

    } //window 

} //nya 