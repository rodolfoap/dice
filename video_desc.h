#ifndef VIDEODESC_H
#define VIDEODESC_H

#include <vector>

enum VideoOrientation { ROTATE_0 = 0, ROTATE_90, ROTATE_180, ROTATE_270 }; // Degrees clock-wise to rotate monitor
enum VideoScanMode { PROGRESSIVE = 0, INTERLACED };
enum VideoMonitorType { MONOCHRONE = 0, COLOR };

typedef double VIDEO_RESISTOR_MATRIX[9][3];

struct VideoOverlay
{
    double x, y;
    double width, height; // NOTE: Negative values for width or height specify maximum possible value
    double r, g, b;
};

struct VideoDesc
{
    union {
        double r[9];
        const double (*r_color)[3];
    };
    double brightness;
    double contrast;
    VideoOrientation orientation;
    VideoScanMode scan_mode;
    VideoMonitorType monitor_type;
    std::vector<VideoOverlay> overlays;

    VideoDesc() : brightness(0.0), contrast(1.0), orientation(ROTATE_0), scan_mode(PROGRESSIVE), monitor_type(MONOCHRONE)
    {
        for(int i = 0; i < 9; i++) r[i] = 0.0;
    }
    VideoDesc& set_r(unsigned pin, double val) 
    { 
        if(pin > 0 && pin <= 9) r[pin-1] = val;
        return *this;
    }
    VideoDesc& set_brightness(double b) 
    { 
        brightness = b;
        return *this;
    }
    VideoDesc& set_contrast(double c) 
    { 
        contrast = c;
        return *this;
    }
    VideoDesc& set_orientation(VideoOrientation o) 
    { 
        orientation = o;
        return *this;
    }
    VideoDesc& set_scan_mode(VideoScanMode s)
    {
        scan_mode = s;
        return *this;
    }
    VideoDesc& set_monitor_type(VideoMonitorType m)
    {
        monitor_type = m;
        return *this;
    }
    VideoDesc& add_overlay(const VideoOverlay& o)
    {
        overlays.push_back(o);
        return *this;
    }
    VideoDesc& set_r_color(const double (*rc)[3])
    {
        r_color = rc;
        return *this;
    }

    static const VideoDesc DEFAULT;
};

#define VIDEO_DESC( name ) VideoDesc video_##name = VideoDesc()
#define VIDEO_RESISTANCE( p, r ) .set_r(p, r)
#define VIDEO_BRIGHTNESS( b ) .set_brightness(b)
#define VIDEO_CONTRAST( c ) .set_contrast(c)
#define VIDEO_ORIENTATION( o ) .set_orientation(o)
#define VIDEO_SCAN_MODE( s ) .set_scan_mode(s)
#define VIDEO_MONITOR_TYPE( m ) .set_monitor_type(m)
#define VIDEO_OVERLAY( o... ) .add_overlay({o})
#define VIDEO_RESISTANCES_RGB( r... ) .set_r_color(r)
#define VIDEO_DESC_END ;

#endif
