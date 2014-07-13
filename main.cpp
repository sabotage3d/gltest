#include "SDL.h"
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

struct Vertex {
    float Position[2];
    float Color[4];
};

const Vertex Vertices[] = {
    {{-0.5, -0.866}, {1, 0, 0.0f, 0}},
    {{0.5, -0.866},  {1, 0, 0.0f, 0}},
    {{0, 1},         {1, 0, 0.0f, 0}},
    {{-0.5, -0.866}, {0.5f, 0.5f, 0.5f}},
    {{0.5, -0.866},  {0.5f, 0.5f, 0.5f}},
    {{0, -0.4f},     {0.5f, 0.5f, 0.5f}},
};

GLuint m_framebuffer;
GLuint m_renderbuffer;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Could not initialize SDL\n");
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
   
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);

    SDL_Window* window = SDL_CreateWindow("Orientation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayMode.h, displayMode.w, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    SDL_GLContext gl = SDL_GL_CreateContext(window);
    

    glGenRenderbuffersOES(1, &m_renderbuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffer);
    
    

    glGenFramebuffersOES(1, &m_framebuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffer);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                 GL_COLOR_ATTACHMENT0_OES,
                                 GL_RENDERBUFFER_OES,
                                 m_renderbuffer);
    
    glViewport(0, 0, displayMode.h, displayMode.w);
    
    glMatrixMode(GL_PROJECTION);
    
    // Initialize the projection matrix.
    const float maxX = 2;
    const float maxY = 3;
    glOrthof(-maxX, +maxX, -maxY, +maxY, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);


    SDL_Event event;
    
    bool done = false;
    
    while (!done)
    {
        
        
        //Render
        glClearColor(0.5f, 0.5f, 0.5f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        
        glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &Vertices[0].Position[0]);
        glColorPointer(4, GL_FLOAT, sizeof(Vertex), &Vertices[0].Color[0]);
        
        GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glPopMatrix();

        SDL_GL_SwapWindow(window);
        
   
        SDL_PumpEvents();
        while (SDL_PollEvent(&event))
        {
            
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                SDL_Log("Window %d resized to %dx%d",
                        event.window.windowID, event.window.data1,
                        event.window.data2);
            }
            
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;
            }
        }
    }
    
    SDL_GL_DeleteContext(gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

