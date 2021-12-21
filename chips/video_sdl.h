#ifndef VIDEO_SDL_H
#define VIDEO_SDL_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#define glGetProcAddress(name) SDL_GL_GetProcAddress(name)

#include "video.h"

class VideoSdl : public Video
{
private:
    uintptr_t handle;

public:
    VideoSdl(uintptr_t h) : Video(), handle(h) { }

    void video_init(int width, int height, const Settings::Video& settings)
    {
        static char env[256];
        sprintf(env, "SDL_WINDOWID=%lld", (uint64_t)handle);
        putenv(env);

        if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	    {
	        printf("Unable to init SDL Video:\n%s\n", SDL_GetError());
		    exit(1);
	    }

        //Set OpenGL Parameters
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

		if(settings.multisampling)
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1 << settings.multisampling);
        }
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, settings.vsync);
    
        if(SDL_SetVideoMode(width, height, 32, SDL_OPENGL) == NULL)
		{
			printf("Unable to set video mode:\n%s\n", SDL_GetError());
			exit(1);
		}

        Video::video_init(width, height, settings);
    }
    
    void swap_buffers()
    {
        SDL_GL_SwapBuffers();
    }

    void show_cursor(bool show)
    {
        SDL_ShowCursor(show);
    }
};

#endif
