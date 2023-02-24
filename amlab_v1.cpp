#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuizmoWidget.h>
#include <igl/project.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>

//int main(int argc, char* argv[])
//{
//    // Load a mesh from file
//    Eigen::MatrixXd V;
//    Eigen::MatrixXi F;
//    igl::read_triangle_mesh(argc > 1 ? argv[1] : "DATA\\cow.off", V, F);
//    // Set up viewer
//    igl::opengl::glfw::Viewer vr;
//    vr.data().set_mesh(V, F);
//
//    igl::opengl::glfw::imgui::ImGuiPlugin imgui_plugin;
//    vr.plugins.push_back(&imgui_plugin);
//
//    // Add a 3D gizmo plugin
//    igl::opengl::glfw::imgui::ImGuizmoWidget gizmo;
//    imgui_plugin.widgets.push_back(&gizmo);
//    // Initialize ImGuizmo at mesh centroid
//    gizmo.T.block(0, 3, 3, 1) =
//        0.5 * (V.colwise().maxCoeff() + V.colwise().minCoeff()).transpose().cast<float>();
//    // Update can be applied relative to this remembered initial transform
//    const Eigen::Matrix4f T0 = gizmo.T;
//    // Attach callback to apply imguizmo's transform to mesh
//    gizmo.callback = [&](const Eigen::Matrix4f& T)
//    {
//        const Eigen::Matrix4d TT = (T * T0.inverse()).cast<double>().transpose();
//        vr.data().set_vertices(
//            (V.rowwise().homogeneous() * TT).rowwise().hnormalized());
//        vr.data().compute_normals();
//    };
//    // Maya-style keyboard shortcuts for operation
//    vr.callback_key_pressed = [&](decltype(vr)&, unsigned int key, int mod)
//    {
//        switch (key)
//        {
//        case ' ': gizmo.visible = !gizmo.visible; return true;
//        case 'W': case 'w': gizmo.operation = ImGuizmo::TRANSLATE; return true;
//        case 'E': case 'e': gizmo.operation = ImGuizmo::ROTATE;    return true;
//        case 'R': case 'r': gizmo.operation = ImGuizmo::SCALE;     return true;
//        }
//        return false;
//    };
//
//    igl::opengl::glfw::imgui::ImGuiMenu menu;
//    imgui_plugin.widgets.push_back(&menu);
//
//    std::cout << R"(
//W,w  Switch to translate operation
//E,e  Switch to rotate operation
//R,r  Switch to scale operation
//)";
//    vr.launch();
//}

void Density(int den)
{
    int a = 0;
    char ch[1000];
    char fct[] = "\"infill_sparse_density\"";
    char fct1[] = "\"default_value\":";
    char* pch;
    char* pch1;
    rename("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.json", "D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.txt");
    FILE* F = fopen("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.txt", "r");;
    FILE* f = fopen("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\temp.txt", "w");
    while (fgets(ch, 1000, F) != NULL) {
        pch = strstr(ch, fct);
        if (pch != NULL) { a = 1; }
        pch1 = strstr(ch, fct1);
        if (pch1 != NULL && a == 1) {
            fprintf(f, "                    \"default_value\": %d,\n", den);
            a = 0;
        }
        else
            fputs(ch, f);
    }
    fclose(F);
    fclose(f);
    remove("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.txt");
    rename("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\temp.txt", "D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.json");
}

void Layer_height(double FH)
{
    int a = 0;
    char ch[1000];
    char fct[] = "\"layer_height\":";
    char fct1[] = "\"default_value\":";
    char* pch;
    char* pch1;
    rename("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.json", "D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.txt");
    FILE* F = fopen("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.txt", "r");;
    FILE* f = fopen("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\temp.txt", "w");
    while (fgets(ch, 1000, F) != NULL) {
        pch = strstr(ch, fct);
        if (pch != NULL) { a = 1; }
        pch1 = strstr(ch, fct1);
        if (pch1 != NULL && a == 1) {
            fprintf(f, "                    \"default_value\": %.3f,\n", FH);
            a = 0;
        }
        else
            fputs(ch, f);
    }
    fclose(F);
    fclose(f);
    remove("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.txt");
    rename("D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\temp.txt", "D:\\AMLab_v1\\API\\lib\\curaengine\\Debug\\fdmprinter.json");
}

int main(int argc, char* argv[])
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

    // Load a mesh in OFF format
    //igl::readOFF(TUTORIAL_SHARED_PATH "/1.STL", V, F);
    igl::read_triangle_mesh(argc > 1 ? argv[1] : "DATA\\cow.off", V, F);
    // Init the viewer
    igl::opengl::glfw::Viewer viewer;

    // Attach a menu plugin
    igl::opengl::glfw::imgui::ImGuiPlugin plugin;
    viewer.plugins.push_back(&plugin);
    igl::opengl::glfw::imgui::ImGuiMenu menu;
    plugin.widgets.push_back(&menu);

    // Customize the menu
    double layer_height = 0.1f;
    double density = 80;
    // Shared between two menus

    // Add content to the default menu window
    menu.callback_draw_viewer_menu = [&]()
    {
        // Draw parent menu content
        menu.draw_viewer_menu();

        // Add new group
    };

    // Draw additional windows
    menu.callback_draw_custom_window = [&]()
    {
        // Define next window position + size
        ImGui::SetNextWindowPos(ImVec2(132.f * menu.menu_scaling(), 0), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_FirstUseEver);
        ImGui::Begin(
            u8"设置", nullptr,
            ImGuiWindowFlags_NoSavedSettings
        );

        // Expose the same variable directly ...

        ImGui::PushItemWidth(-110);
        //static std::string str = "bunny";
        //ImGui::InputText("Name", str);
        ImGui::InputDouble(u8"层高(mm)", &layer_height, 0, 0, "%.3f");
        Layer_height(layer_height);
        // Expose an enumeration type
        enum Orientation { Carbon = 0, Glass };
        static Orientation dir = Carbon;
        ImGui::Combo(u8"耗材类型", (int*)(&dir), u8"碳纤维\0玻璃纤维\0\0");

        enum Orientation1 { Isqrid = 0, Anisogrid, Solid };
        static Orientation1 pattern = Isqrid;
        ImGui::Combo(u8"填充类型", (int*)(&pattern), "Isqrid\0Anisogrid\0Solid\0\0");


        ImGui::InputDouble(u8"填充密度(%)", &density, 0, 0, "%.1f");
        Density(density);
        ImGui::PopItemWidth();
        ImGui::PushItemWidth(-100);
        // We can also use a std::vector<std::string> defined dynamically
        static int num_choices = 3;
        static std::vector<std::string> choices;
        static int idx_choice = 0;
        if (ImGui::InputInt(u8"外围轮廓", &num_choices))
        {
            num_choices = std::max(0, std::min(26, num_choices));
        }
        static int num_choices1 = 3;
        static std::vector<std::string> choices1;
        static int idx_choice1 = 0;
        if (ImGui::InputInt(u8"内部轮廓", &num_choices1))
        {
            num_choices1 = std::max(0, std::min(26, num_choices1));
        }
        static bool brim = true;
        if (ImGui::Checkbox(u8"打印底座", &brim))
        {
            // do something
            std::cout << "boolVariable: " << std::boolalpha << brim << std::endl;
        }
        static bool support = true;
        if (ImGui::Checkbox(u8"完全支撑", &support))
        {
            // do something
            std::cout << "boolVariable: " << std::boolalpha << support << std::endl;
        }

        if (ImGui::Button(u8"开始切片", ImVec2(-1, 0)))
        {
            std::cout << "Hello\n";
        }
        ImGui::End();
    };

    // Plot the mesh
    viewer.data().set_mesh(V, F);
    viewer.data().add_label(viewer.data().V.row(0) + viewer.data().V_normals.row(0).normalized() * 0.005, "Hello World!");
    viewer.launch();
}


//Copyright (c) 2018 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

//#include <iostream> //To change the formatting of std::cerr.
//#include <signal.h> //For floating point exceptions.
//#if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
//#include <sys/resource.h> //For setpriority.
//#endif
//#include "Application.h"
//#include "utils/logoutput.h"
//
//namespace cura
//{
//
//    //Signal handler for a "floating point exception", which can also be integer division by zero errors.
//    void signal_FPE(int n)
//    {
//        (void)n;
//        logError("Arithmetic exception.\n");
//        exit(1);
//    }
//
//}//namespace cura
//
//int main(int argc, char** argv)
//{
//#if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
//    //Lower the process priority on linux and mac. On windows this is done on process creation from the GUI.
//    setpriority(PRIO_PROCESS, 0, 10);
//#endif
//
//#ifndef DEBUG
//    //Register the exception handling for arithmetic exceptions, this prevents the "something went wrong" dialog on windows to pop up on a division by zero.
//    signal(SIGFPE, cura::signal_FPE);
//#endif
//    std::cerr << std::boolalpha;
//
//    cura::Application::getInstance().run(argc, argv);
//
//    return 0;
//}
