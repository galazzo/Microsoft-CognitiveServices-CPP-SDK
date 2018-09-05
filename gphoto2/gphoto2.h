#ifndef __GPHOTO2_H
#define __GPHOTO2_H

#include <string>
#include <vector>
#include <gphoto2/gphoto2.h>
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-widget.h>
#include <gphoto2/gphoto2-result.h>
#include <gphoto2/gphoto2-port-log.h>

using namespace std;

namespace Gphoto2
{
    int camera_auto_focus (Camera *list, GPContext *context, int onoff);
    int camera_eosviewfinder (Camera *list, GPContext *context, int onoff);
    int camera_manual_focus (Camera *list, int tgt, GPContext *context);

    class DigitalCamera
    {
    public:
        DigitalCamera(std::string model, std::string port);
        ~DigitalCamera();

        int Open();
        int CanonEnableCapture (int onoff);

        int GetConfigValueString (const char *key, char **str);
        int SetConfigValueString (const char *key, const char *val);

        void CapturePreviewToMemory(const char **ptr, size_t *size);
        void CaptureToFile(const char *fn);
        void CaptureToMemory(const char **ptr, size_t *size);

        std::string Model();
        std::string Port();

        static std::vector<Gphoto2::DigitalCamera> Autodetect();

        static GPContext* Context();

    private:
        std::string model;
        std::string port;

        Camera *camera;

        static GPContext* _context;
    };

    static void ctx_error_func (GPContext *context, const char *str, void *data);
    static void ctx_status_func (GPContext *context, const char *str, void *data);
}

#endif
