//#include <igl/read_triangle_mesh.h>
#include <igl/readSTL.h>
#include <igl/readMESH.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuizmoWidget.h>
#include <igl/project.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <math.h>
#include <iostream>
#include <iostream> //To change the formatting of std::cerr.
#include <signal.h> //For floating point exceptions.
#if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/resource.h> //For setpriority.
#endif
#include "Application.h"
#include "utils/logoutput.h"
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<scene.cpp>
#include<FffPolygonGenerator.cpp>
using namespace std;


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

//Copyright (c) 2018 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

namespace cura
{

    //Signal handler for a "floating point exception", which can also be integer division by zero errors.
    void signal_FPE(int n)
    {
        (void)n;
        logError("Arithmetic exception.\n");
        exit(1);
    }

}//namespace cura


//int main()
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
//int argc = 16;
//char* argv[20] = { "amlab_v1","slice","-v","-j","D:/Clib/AMLab_v1/slice/fdmprinter.def.json","-v","-j","D:/Clib/AMLab_v1/slice/fdmextruder.def.json","-e1","-s","infill_line_distance=0","-o","D:/Clib/AMLab_v1/slice/test.gcode","-l","D:/Clib/AMLab_v1/slice/test.stl","--next" };
//    cura::Application::getInstance().run(argc, argv);
//
//    return 0;
//}

void CuraEngine(int argc, char** argv)
{
#if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    //Lower the process priority on linux and mac. On windows this is done on process creation from the GUI.
    setpriority(PRIO_PROCESS, 0, 10);
#endif

#ifndef DEBUG
    //Register the exception handling for arithmetic exceptions, this prevents the "something went wrong" dialog on windows to pop up on a division by zero.
    signal(SIGFPE, cura::signal_FPE);
#endif
    std::cerr << std::boolalpha;

    cura::Application::getInstance().run(argc, argv);

   // return 0;
}


void Density(int den)
{
    int a = 0;
    char ch[1000];
    char fct[] = "\"infill_sparse_density\"";
    char fct1[] = "\"default_value\":";
    char* pch;
    char* pch1;
    rename("slice\\fdmprinter.def.json", "slice\\fdmprinter.txt");
    FILE* F = fopen("slice\\fdmprinter.txt", "r");;
    FILE* f = fopen("slice\\temp.txt", "w");
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
    remove("slice\\fdmprinter.txt");
    rename("slice\\temp.txt", "slice\\fdmprinter.def.json");
}

void Layer_height(double FH)
{
    int a = 0;
    char ch1[1000];
    char fct[] = "\"layer_height\":";
    char fct1[] = "\"default_value\":";
    char* pch;
    char* pch1;
    rename("slice\\fdmprinter.def.json", "slice\\fdmprinter.txt");
    FILE* F = fopen("slice\\fdmprinter.txt", "r");;
    FILE* f = fopen("slice\\temp.txt", "w");
    while (fgets(ch1, 1000, F) != NULL) {
        pch = strstr(ch1, fct);
        if (pch != NULL) { a = 1; }
        pch1 = strstr(ch1, fct1);
        if (pch1 != NULL && a == 1) {
            fprintf(f, "                    \"default_value\": %.3f,\n", FH);
            a = 0;
        }
        else
            fputs(ch1, f);
    }
    fclose(F);
    fclose(f);
    remove("slice\\fdmprinter.txt");
    rename("slice\\temp.txt", "slice\\fdmprinter.def.json");
}

struct vec3f
{
    float x, y, z;
    int index;
    inline vec3f operator =(const vec3f& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->index = v.index;
        return *this;
    }
    inline bool operator==(vec3f& v)
    {
        if (this->x == v.x && this->y == v.y && this->z == v.z)
            return true;
        else
            return false;
    }
    inline bool operator!=(vec3f& v)
    {
        return !(*this == v);
    }
};

struct face
{
    vec3f verts[3];
    face operator=(const face& f)
    {
        for (int n = 0; n < 3; n++)
        {
            this->verts[n] = f.verts[n];
        }
        return *this;
    }
};


void fileformatchange(char* filename)
{
    FILE* filein;
    FILE* fileout;
    float x, y, z;
    char buf[999];
    char str[100];
    vector<vec3f> vec1;
    vector<face> myvector;
    if (!(filein = fopen(filename, "r")))exit(1);
    while (fscanf(filein, "%s", buf) != EOF)
    {
        switch (buf[0])
        {
        case's'://solid CATIA STL
        case'o'://outer loop
        case'e'://endloop or endfacet or endsolid C...
            fgets(buf, sizeof(buf), filein);
            break;
        case'f'://facet normal ...!
        {
            face temp;
            fscanf(filein, "%s", str);
            fscanf(filein, "%f %f %f", &x, &y, &z);
            fgets(buf, sizeof(buf), filein);//...might be '\0'
            fgets(buf, sizeof(buf), filein);//skip "outer loop"
            for (int k = 0; k < 3; k++)
            {
                float xx, yy, zz;
                fscanf(filein, "%s", str);//skip "vertex"
                fscanf(filein, "%f %f %f", &xx, &yy, &zz);
                temp.verts[k].x = xx;
                temp.verts[k].y = yy;
                temp.verts[k].z = zz;
                int w = 0;
                if (vec1.size() == 0)
                {
                    temp.verts[k].index = w + 1;
                }
                else
                {
                    for (w = 0; w < vec1.size(); w++)
                    {
                        if (temp.verts[k] == vec1[w])
                        {
                            temp.verts[k].index = w + 1;
                            break;
                        }
                        else if (temp.verts[k] != vec1[w])
                            continue;

                        else
                            ;
                    }
                }

                if (w == vec1.size())
                {
                    temp.verts[k].index = w + 1;
                    vec1.push_back(temp.verts[k]);
                }
            }
            myvector.push_back(temp);
            break;
        }
        default:
            fgets(buf, sizeof(buf), filein);
            break;
        }
    }
    fclose(filein);
    if (!(fileout = fopen("D:\\Clib\\AMLab_v1\\DATA\\chao.txt", "w")))exit(1);
    fprintf(fileout, "# %d vertex %d face\n", (int)vec1.size(), (int)myvector.size());
    for (int j = 0; j < vec1.size(); j++)
        fprintf(fileout, "v %f %f %f\n", vec1[j].x, vec1[j].y, vec1[j].z);
    for (int j = 0; j < myvector.size(); j++)
        fprintf(fileout, "f %d %d %d\n", myvector[j].verts[0].index, myvector[j].verts[1].index, myvector[j].verts[2].index);
    fclose(fileout);

}


int main(int argc, char* argv[])
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    Eigen::MatrixXd N;

    //fileformatchange("D:\\Clib\\AMLab_v1\\DATA\\Exercise01.stl");
    //rename("D:\\Clib\\AMLab_v1\\DATA\\chao.txt", "D:\\Clib\\AMLab_v1\\DATA\\Exercise01.obj");
    // Load a mesh in OFF format
    //igl::readOBJ( "DATA\\blade11.obj", V, F);
    igl::readOBJ("DATA\\testSubarea.obj", V, F);
    //igl::readSTL( "DATA\\blade11.STL", V, F, N);
    //igl::read_triangle_mesh(argc > 1 ? argv[1] : "DATA\\cow.off", V, F);
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
        int argc=16;
        char* argv[]={"amlab_v1","slice","-v","-j","slice/fdmprinter.def.json","-v","-j","slice/fdmextruder.def.json","-e1","-s","infill_line_distance=0","-o","slice/test.gcode","-l","DATA\\testSubarea.STL","--next"};

        if (ImGui::Button(u8"开始切片", ImVec2(-1, 0)))
        {
            //std::cout << "Hello\n";
            CuraEngine(argc,argv);
        }
        ImGui::End();
    };

    // Plot the mesh
    viewer.data().set_mesh(V, F);
    viewer.data().add_label(viewer.data().V.row(0) + viewer.data().V_normals.row(0).normalized() * 0.005, "Hello World!");
    viewer.launch();
}




