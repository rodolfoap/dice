#ifndef VIDEO_WGL_H
#define VIDEO_WGL_H

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include <phoenix.hpp>
#include "video.h"

using namespace phoenix;

#define glGetProcAddress(name) wglGetProcAddress(name)

static bool wglExtensionSupportedEXT(const char *extension_name)
{
    // pointer to function which returns a string with a list of all wgl extensions
    PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT;
    wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) wglGetProcAddress("wglGetExtensionsStringEXT");

    if (wglGetExtensionsStringEXT == NULL || strstr(wglGetExtensionsStringEXT(), extension_name) == NULL)
        return false;

    return true;
}

static bool wglExtensionSupportedARB(const char *extension_name, HDC dc)
{
    // pointer to function which returns a string with a list of all wgl extensions
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");

    if (wglGetExtensionsStringARB == NULL || strstr(wglGetExtensionsStringARB(dc), extension_name) == NULL)
        return false;

    return true;
}

class VideoWgl : public Video
{
private:
    VerticalLayout& layout;
    Viewport*& viewport;

    HDC display;
    HWND window;
    HGLRC wglcontext;

    unsigned multisample;
    int multisampleFormat;

    bool initMultisampling();

public:
    VideoWgl(VerticalLayout& l, Viewport*& v) : Video(), layout(l), viewport(v), 
        window((HWND)v->handle()), wglcontext(0), multisample(0) { }

    ~VideoWgl()
    {
        if(wglcontext)
        {
            wglMakeCurrent(display, 0);
            wglDeleteContext(wglcontext);
            wglcontext = 0;
        }
    }

    void video_init(int width, int height, const Settings::Video& settings)
    {
        GLuint pixel_format;
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
    
        display = GetDC(window);

        if(wglcontext == 0)
        {
            pixel_format = ChoosePixelFormat(display, &pfd);
            SetPixelFormat(display, pixel_format, &pfd);
            wglcontext = wglCreateContext(display);
            wglMakeCurrent(display, wglcontext);
        }

        int prev_multisample = multisample;
        multisample = settings.multisampling;
        if(multisample && !initMultisampling()) multisample = 0;

        wglMakeCurrent(display, 0);
        wglDeleteContext(wglcontext);
        wglcontext = 0;

        if(multisample != prev_multisample)
        {
            // Must destroy window and recreate it in order to change pixel format
            ReleaseDC(window, display);
            
            layout.remove(*viewport);
            delete viewport;
            viewport = new Viewport();
            layout.append(*viewport, {~0, ~0});

            window = (HWND)viewport->handle();
            display = GetDC(window);
        }
        
        if(multisample) pixel_format = multisampleFormat;
        else pixel_format = ChoosePixelFormat(display, &pfd);

        SetPixelFormat(display, pixel_format, &pfd);
    
        wglcontext = wglCreateContext(display);
        wglMakeCurrent(display, wglcontext);
    
        if (wglExtensionSupportedEXT("WGL_EXT_swap_control"))
        {
            // Extension is supported, init pointer
            PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress ("wglSwapIntervalEXT");

            if(wglSwapIntervalEXT) wglSwapIntervalEXT(settings.vsync);
        }

        Video::video_init(width, height, settings);
    }
    
    void swap_buffers()
    {
        SwapBuffers(display);
    }

    void show_cursor(bool show)
    {
        ShowCursor(show);
    }
};

bool VideoWgl::initMultisampling()
{
    if (!wglExtensionSupportedARB("WGL_ARB_multisample", display)) return false;
 
    // Get Our Pixel Format
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

    if(!wglChoosePixelFormatARB) return false;

    UINT numFormats;
    float fAttributes[] = { 0.0, 0.0 }; 
    int iAttributes[] = { 
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 24,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        WGL_SAMPLES_ARB, 1 << multisample,
        0, 0
    };
    
    bool valid = wglChoosePixelFormatARB(display, iAttributes, fAttributes, 1, &multisampleFormat, &numFormats);
    
    return valid && numFormats >= 1;
}

#endif
