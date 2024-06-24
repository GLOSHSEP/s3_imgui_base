#include "nya/windows/intro.h" 
#include "imgui.h" 

namespace nya {
    namespace window { 
        void demo_window() { 
            if (nya::show_demo) { 
                ImGui::Begin("Welcome", &nya::show_intro);
                ImGui::SetWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver); 

                static bool showWindow = false; 

                if (ImGui::Button("Toggle Demo Window")) {
                    showWindow = !showWindow;
                }

                if (showWindow) {
                    ImGui::ShowDemoWindow();
                }

                ImGui::End(); 
            } 
        } 
    }
}
