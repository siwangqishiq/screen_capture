
#include <iostream>
#include <thread>
#include <memory>
#include "GLFW/glfw3.h"
#include "bridge.h"

class Application{
private:
    GLFWwindow *window;
    std::shared_ptr<IScreenApi> mScreenApi;

public:
    void appInit(){
        mScreenApi = std::make_shared<ScreenApi>();
    }

    void execute(){
        glfwInit();

        appInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DECORATED , GLFW_FALSE);

        //窗口透明
        // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER , GLFW_TRUE);
        
        int screenWidth = 0;
        int screenHeight = 0;
        mScreenApi->findScreenSize(screenWidth , screenHeight);

        window = glfwCreateWindow(screenWidth, screenHeight, "screen capture", nullptr, nullptr);
        
        if (window == nullptr) {
            glfwTerminate();
            return;
        }

        glfwSetWindowPos(window , 0 , 0);
        glfwSetWindowAttrib(window , GLFW_DECORATED , GLFW_FALSE);

        glfwSetKeyCallback(window , [](GLFWwindow* windows_,int key,int scancode,int action,int mods){
            if(glfwGetKey(windows_, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                glfwSetWindowShouldClose(windows_, true);
            }
        });

        while(!glfwWindowShouldClose(window)) {
            glfwSwapBuffers(window);
            glfwPollEvents();
            glfwSwapInterval(1);//锁定固定帧率
        }//end while

        glfwTerminate();
    }
};

void subThreadEntry(){
    Application app;
    app.execute();
}

int main(){
    // std::cout << "Hello it works" << std::endl;
    // findScreenSize();

    std::thread th(subThreadEntry);
    th.join();
    return 0;
}//end main