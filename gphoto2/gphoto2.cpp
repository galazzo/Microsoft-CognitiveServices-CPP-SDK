#include "gphoto2.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

using namespace std;

GPContext* Gphoto2::DigitalCamera::_context = nullptr;

Gphoto2::DigitalCamera::DigitalCamera(std::string model, std::string port)
{
    this->model = model;
    this->port = port;

    //this->context = Gphoto2::CreateContext();
}

Gphoto2::DigitalCamera::~DigitalCamera()
{
    gp_camera_exit(this->camera, Gphoto2::DigitalCamera::Context());
    gp_camera_free(this->camera);
}

/*
 * This function looks up a label or key entry of
 * a configuration widget.
 * The functions descend recursively, so you can just
 * specify the last component.
 */

int _lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child)
{
    int ret;
    ret = gp_widget_get_child_by_name (widget, key, child);
    if (ret < GP_OK)
        ret = gp_widget_get_child_by_label (widget, key, child);
    return ret;
}


GPContext* Gphoto2::DigitalCamera::Context()
{
    if(Gphoto2::DigitalCamera::_context == nullptr)
    {
        // This is the mandatory part
        Gphoto2::DigitalCamera::_context = gp_context_new();

        // All the parts below are optional!
        gp_context_set_error_func (Gphoto2::DigitalCamera::_context, ctx_error_func, NULL);
        gp_context_set_status_func (Gphoto2::DigitalCamera::_context, ctx_status_func, NULL);
    }

    return Gphoto2::DigitalCamera::_context;
}

/*
 * This function opens a camera depending on the specified model and port.
 */
int Gphoto2::DigitalCamera::Open()
{
    const char *camera_model = NULL;
    const char* camera_port = NULL;

    int ret;
    CameraAbilities abilities;
    GPPortInfo	pi;
    GPPortInfoList *portinfolist = nullptr;
    CameraAbilitiesList *abilitiesList = nullptr;

    if( this->camera == nullptr)
    {
        ret = gp_camera_new (&this->camera);
        if (ret < GP_OK)
        {
            cout << "Cannot create camera object: "<< ret << endl;

            gp_camera_free (camera);
            return ret;
        }
    }

    if (!abilitiesList)
    {
        // Load all the camera drivers we have...
        ret = gp_abilities_list_new (&abilitiesList);
        if (ret < GP_OK)
            return ret;

        ret = gp_abilities_list_load (abilitiesList, Gphoto2::DigitalCamera::Context());
        if (ret < GP_OK)
            return ret;
    }

    // First lookup the model / driver
    int m = gp_abilities_list_lookup_model(abilitiesList, camera_model);
    if (m < GP_OK)
        return ret;
    else
        this->model = camera_model;

    ret = gp_abilities_list_get_abilities (abilitiesList, m, &abilities);
    if (ret < GP_OK)
        return ret;

    ret = gp_camera_set_abilities (camera, abilities);
    if (ret < GP_OK)
        return ret;

    if (!portinfolist)
    {
        // Load all the port drivers we have...
        ret = gp_port_info_list_new (&portinfolist);
        if (ret < GP_OK)
            return ret;

        ret = gp_port_info_list_load (portinfolist);
        if (ret < 0)
            return ret;

        ret = gp_port_info_list_count (portinfolist);
        if (ret < 0)
            return ret;
    }

    // Then associate the camera with the specified port
    int p = gp_port_info_list_lookup_path (portinfolist, camera_port);
    switch (p) {
    case GP_ERROR_UNKNOWN_PORT:
            std::cout << "The port you specified "
                      << port << " can not be found. Please "
                      << "specify one of the ports found by "
                      << "'gphoto2 --list-ports' and make "
                      << "sure the spelling is correct "
                      << "(i.e. with prefix 'serial:' or 'usb:').";
            break;
    default:
            break;
    }

    if (p < GP_OK)
        return p;
    else
        this->port = camera_port;

    ret = gp_port_info_list_get_info (portinfolist, p, &pi);
    if (ret < GP_OK)
        return ret;

    ret = gp_camera_set_port_info (this->camera, pi);
    if (ret < GP_OK)
        return ret;

    return GP_OK;
}

void Gphoto2::DigitalCamera::CaptureToFile(const char *fn)
{
    int fd, retval;
    CameraFile *file;
    CameraFilePath camera_file_path;

    cout << "Capturing." << endl;

    // NOP: This gets overridden in the library to /capt0000.jpg
    strcpy(camera_file_path.folder, "/");
    strcpy(camera_file_path.name, "foo.jpg");

    retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, Gphoto2::DigitalCamera::Context());
    cout << "Retval: " << retval << endl;

    printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);

    fd = open(fn, O_CREAT | O_WRONLY, 0644);
    retval = gp_file_new_from_fd(&file, fd);
    cout << "Retval: " << retval << endl;

    retval = gp_camera_file_get(this->camera, camera_file_path.folder, camera_file_path.name, GP_FILE_TYPE_NORMAL, file, Gphoto2::DigitalCamera::Context());
    cout << "Retval: " << retval << endl;

    /*printf("Deleting.\n");
    retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name, context);
    printf("  Retval: %d\n", retval);*/

    gp_file_free(file);
}

void Gphoto2::DigitalCamera::CaptureToMemory(const char **ptr, size_t *size)
{
    int ret;
    CameraFile *file;
    CameraFilePath camera_file_path;
    CameraStorageInformation* sifs;
    int nrofsifs = -1;

    printf("Capturing.\n");

    ret = gp_camera_get_storageinfo(camera, &sifs, &nrofsifs, Gphoto2::DigitalCamera::Context());
     if (ret < GP_OK) {
        fprintf (stderr, "gp_camera_get_storageinfo: %s\n", gp_result_as_string(ret));
        return;
    }

    /* NOP: This gets overridden in the library to /capt0000.jpg */
    //strcpy(camera_file_path.folder, "/");
    //strcpy(camera_file_path.name, "foo.jpg");
    //strcpy(camera_file_path.name, sifs->basedir);
    //strcpy(camera_file_path.name, "foo.cr2");
    //strcat(camera_file_path.name, "/foo.cr2");

    ret = gp_camera_capture(this->camera, GP_CAPTURE_IMAGE, &camera_file_path, Gphoto2::DigitalCamera::Context());
    if (ret < GP_OK)
    {
        fprintf (stderr, "gp_camera_capture: %s\n", gp_result_as_string(ret));
        return;
    }

    printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);

    ret = gp_file_new(&file);
    if (ret < GP_OK) {
                fprintf (stderr, "gp_file_new: %s\n", gp_result_as_string(ret));
                return;
    } else {
                std::cout << "gp_file_new" << endl;
        }

        //ret = gp_camera_file_get(camera, folder, camera_file_path->name, GP_FILE_TYPE_RAW , file, context);
    ret = gp_camera_file_get(this->camera, camera_file_path.folder, camera_file_path.name, GP_FILE_TYPE_NORMAL, file, Gphoto2::DigitalCamera::Context());
        if (ret < GP_OK) {
                fprintf (stderr, "gp_camera_file_get: %s\n", gp_result_as_string(ret));
                return;
    } else {
                std::cout << "gp_camera_file_get" << endl;
        }

    ret = gp_file_get_data_and_size (file, ptr, (unsigned long int*)size);
        if (ret < GP_OK) {
                fprintf (stderr, "gp_file_get_data_and_size failed: %s\n", gp_result_as_string(ret));
                return;
    } else {
        std::cout << "gp_file_get_data_and_size" << endl;
    }

    /*printf("Deleting.\n");
    ret = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name, context);
    printf("  ret: %s\n", gp_result_as_string(ret));*/

    gp_file_free(file);
}

void Gphoto2::DigitalCamera::CapturePreviewToMemory(const char **ptr, size_t *size)
{
    int ret;
    CameraFile *file;

    cout << "Capturing." << endl;

    ret = gp_file_new(&file);
    printf("  ret: %s\n", gp_result_as_string(ret));

    ret = gp_camera_capture_preview(this->camera, file, Gphoto2::DigitalCamera::Context());
    printf("  ret: %s\n", gp_result_as_string(ret));

    ret = gp_file_get_data_and_size (file, ptr, (unsigned long int*)size);
    if (ret < GP_OK)
    {
        fprintf (stderr, "gp_file_get_data_and_size failed: %s\n", gp_result_as_string(ret));
        this->SetConfigValueString("viewfinder", "0");
        return;
    }

    this->SetConfigValueString("viewfinder", "0");
}

std::string Gphoto2::DigitalCamera::Model() {
    return this->model;
}

std::string Gphoto2::DigitalCamera::Port() {
    return this->port;
}

/* Gets a string configuration value.
 * This can be:
 *  - A Text widget
 *  - The current selection of a Radio Button choice
 *  - The current selection of a Menu choice
 *
 * Sample (for Canons eg):
 *   get_config_value_string (camera, "owner", &ownerstr, context);
 */
int Gphoto2::DigitalCamera::GetConfigValueString (const char *key, char **str)
{
    CameraWidget		*widget = NULL, *child = NULL;
    CameraWidgetType	type;
    int			ret;

    ret = gp_camera_get_single_config (camera, key, &child, Gphoto2::DigitalCamera::Context());
    if (ret == GP_OK) {
        if (!child) fprintf(stderr,"child is NULL?\n");
        widget = child;
    } else {
            ret = gp_camera_get_config (this->camera, &widget, Gphoto2::DigitalCamera::Context());
            if (ret < GP_OK) {
                fprintf (stderr, "camera_get_config failed: %d\n", ret);
                return ret;
            }

            ret = _lookup_widget (widget, key, &child);
            if (ret < GP_OK) {
                fprintf (stderr, "lookup widget failed: %d\n", ret);
                goto out;
            }
    }

    // This type check is optional, if you know what type the label
    // has already. If you are not sure, better check. */
    ret = gp_widget_get_type (child, &type);
    if (ret < GP_OK) {
        fprintf (stderr, "widget get type failed: %d\n", ret);
        goto out;
    }

    // https://github.com/code-iai/iai_photo/blob/master/src/libphoto2/photo_camera.cpp
    switch (type)
    {
    case GP_WIDGET_MENU:
    case GP_WIDGET_RADIO:
    case GP_WIDGET_TEXT:
            {
                char *val;
                ret = gp_widget_get_value (child, &val);
                if (ret < GP_OK) {
                        fprintf (stderr, "could not query widget value: %d\n", ret);
                        goto out;
                }
                // Create a new copy for our caller.
                //printf(val);
                *str = strdup (val);
                break;
            }
            case GP_WIDGET_TOGGLE: // int
            {
                    int t=-7;
                    int ret=gp_widget_get_value( child, &t );

                    if( ret != GP_OK )
                    {
                        fprintf (stderr, "could not get widget value: %d\n", ret);
                        gp_context_error( Gphoto2::DigitalCamera::Context(), "Failed to retrieve values of toggle widget %s.", key );
                    }

                    //cout << "GP_WIDGET_TOGGLE::" << t;
                    if( *str == NULL) *str = new char[1024];
                    sprintf( *str, "%d", t );

                    break;
            }
            case GP_WIDGET_DATE: // int
            {
                    /*int error_code, t;
                    time_t working_time;
                    struct tm *localtm;
                    char timebuf[200];

                    if( gp_widget_get_value( child, &t ) != GP_OK )
                    {
                      gp_context_error( context_,"Failed to retrieve values of date/time widget %s.", param.c_str() );
                      break;
                    }
                    working_time = t;
                    localtm = localtime( &working_time );
                    error_code = strftime( timebuf, sizeof(timebuf), "%c", localtm );
                    sprintf( *value, "%s", timebuf );*/
                    break;
            }
            default: {
                    fprintf (stderr, "widget has bad type %d\n", type);
                    ret = GP_ERROR_BAD_PARAMETERS;
                    goto out;
            }
    }

out:
    gp_widget_free (widget);
    return ret;
}


/* Sets a string configuration value.
 * This can set for:
 *  - A Text widget
 *  - The current selection of a Radio Button choice
 *  - The current selection of a Menu choice
 *
 * Sample (for Canons eg):
 *   get_config_value_string (camera, "owner", &ownerstr, context);
 */
int Gphoto2::DigitalCamera::SetConfigValueString (const char *key, const char *val)
{
    CameraWidget		*widget = NULL, *child = NULL;
    CameraWidgetType	type;
    int			ret;

    ret = gp_camera_get_config (camera, &widget, Gphoto2::DigitalCamera::Context());
    if (ret < GP_OK) {
        fprintf (stderr, "camera_get_config failed: %s\n", gp_result_as_string(ret));
        return ret;
    }

    ret = _lookup_widget (widget, key, &child);
    if (ret < GP_OK) {
        fprintf (stderr, "lookup widget failed: %s\n", gp_result_as_string(ret));
        goto out;
    }

    /* This type check is optional, if you know what type the label
     * has already. If you are not sure, better check. */
    ret = gp_widget_get_type (child, &type);
    if (ret < GP_OK) {
        cout <<  "widget get type failed: " <<  gp_result_as_string(ret) << endl;
        goto out;
    }

    switch (type)
    {
    case GP_WIDGET_MENU:
    case GP_WIDGET_RADIO:
    case GP_WIDGET_TEXT:{
                    /* This is the actual set call. Note that we keep
                     * ownership of the string and have to free it if necessary.
                     */
                    ret = gp_widget_set_value (child, val);
                    if (ret < GP_OK) {
                            fprintf (stderr, "could not set widget value: %s\n", gp_result_as_string(ret));
                            goto out;
                    }
                    break;
            }
            case GP_WIDGET_TOGGLE: // int
            {
                    int t = atoi((const char*) val);
                    ret = gp_widget_set_value (child, &t);
                    if (ret < GP_OK) {
                            fprintf (stderr, "could not set widget value: %s\n", gp_result_as_string(ret));
                            goto out;
                    } else {
                            fprintf (stdout, "set widget value for key '%s' to %d\n", key, t);
                    }

                    break;
            }
            default: {
                    fprintf (stderr, "widget has bad type %d\n", type);
                    ret = GP_ERROR_BAD_PARAMETERS;
                    goto out;
            }
    }

    ret = gp_camera_set_single_config (this->camera, key, child, Gphoto2::DigitalCamera::Context());
    if (ret != GP_OK)
    {
        // This stores it on the camera again
        ret = gp_camera_set_config (camera, widget, Gphoto2::DigitalCamera::Context());
        if (ret < GP_OK) {
                fprintf (stderr, "camera_set_config failed: %s\n", gp_result_as_string(ret));
                return ret;
        }
    }

out:
    gp_widget_free (widget);
    return ret;
}

/*
 * This enables/disables the specific canon capture mode.
 * 
 * For non canons this is not required, and will just return
 * with an error (but without negative effects).
 */
int Gphoto2::DigitalCamera::CanonEnableCapture(int onoff)
{
    CameraWidget *widget = NULL, *child = NULL;
    CameraWidgetType	type;
    int			ret;

    ret = gp_camera_get_single_config (this->camera, "capture", &widget, Gphoto2::DigitalCamera::Context());
    if (ret < GP_OK)
    {
        fprintf (stderr, "camera_get_config failed: %s\n", gp_result_as_string(ret));
        return ret;
    }

    ret = gp_widget_get_type (widget, &type);
    if (ret < GP_OK)
    {
        fprintf (stderr, "widget get type failed: %s\n", gp_result_as_string(ret));
        goto out;
    }

    switch (type)
    {
    case GP_WIDGET_TOGGLE:
            break;
    default:
            fprintf (stderr, "widget has bad type %d\n", type);
            ret = GP_ERROR_BAD_PARAMETERS;
            goto out;
    }

    // Now set the toggle to the wanted value
    ret = gp_widget_set_value (widget, &onoff);
    if (ret < GP_OK) {
            fprintf (stderr, "toggling Canon capture to %d failed with %d\n", onoff, ret);
            goto out;
    }
    // OK
    ret = gp_camera_set_single_config(this->camera, "capture", widget, Gphoto2::DigitalCamera::Context());
    if (ret < GP_OK) {
            fprintf (stderr, "camera_set_config failed: %d\n", ret);
            return ret;
    }
out:
    gp_widget_free (widget);
    return ret;
}

/*
 * This detects all currently attached cameras and returns
 * them in a list. It avoids the generic usb: entry.
 *
 * This function does not open nor initialize the cameras yet.
 */
std::vector<Gphoto2::DigitalCamera> Gphoto2::DigitalCamera::Autodetect()
{
    std::vector<Gphoto2::DigitalCamera> result;
    CameraList *list;
    const char *camera_model = NULL;
    const char* camera_port = NULL;

    gp_list_new (&list);
    //gp_list_reset (list);
    int ret = gp_camera_autodetect (list, Gphoto2::DigitalCamera::Context());
    if (ret < GP_OK)
    {
        cout << "No camera auto detected: " << gp_result_as_string(ret) << endl;

        //gp_camera_free (camera);
        gp_list_free (list);
    }
    else
    {
        int count = gp_list_count(list);
        std::cout << count << " Camera detected" << std::endl;

        for (int i=0; i < count; i++)
        {
            gp_list_get_name (list, i, &camera_model);
            gp_list_get_value (list, i, &camera_port);

            std::cout << camera_model << " - " << camera_port << std::endl;
            Gphoto2::DigitalCamera detectedCamera(camera_model, camera_port);
            result.push_back(detectedCamera);
        }
    }
    return result;
}

/* calls the Nikon DSLR or Canon DSLR autofocus method. */
int Gphoto2::camera_eosviewfinder(Camera *camera, GPContext *context, int onoff) {
        CameraWidget		*widget = NULL, *child = NULL;
        CameraWidgetType	type;
        int			ret,val;

        ret = gp_camera_get_config (camera, &widget, context);
        if (ret < GP_OK) {
                fprintf (stderr, "camera_get_config failed: %d\n", ret);
                return ret;
        }
        ret = _lookup_widget (widget, "eosviewfinder", &child);
        if (ret < GP_OK) {
                fprintf (stderr, "lookup 'eosviewfinder' failed: %d\n", ret);
                goto out;
        }

        /* check that this is a toggle */
        ret = gp_widget_get_type (child, &type);
        if (ret < GP_OK) {
                fprintf (stderr, "widget get type failed: %d\n", ret);
                goto out;
        }
        switch (type) {
        case GP_WIDGET_TOGGLE:
                break;
        default:
                fprintf (stderr, "widget has bad type %d\n", type);
                ret = GP_ERROR_BAD_PARAMETERS;
                goto out;
        }

        ret = gp_widget_get_value (child, &val);
        if (ret < GP_OK) {
                fprintf (stderr, "could not get widget value: %d\n", ret);
                goto out;
        }
        val = onoff;
        ret = gp_widget_set_value (child, &val);
        if (ret < GP_OK) {
                fprintf (stderr, "could not set widget value to 1: %d\n", ret);
                goto out;
        }

        ret = gp_camera_set_config (camera, widget, context);
        if (ret < GP_OK) {
                fprintf (stderr, "could not set config tree to eosviewfinder: %d\n", ret);
                goto out;
        }
out:
        gp_widget_free (widget);
        return ret;
}

int Gphoto2::camera_auto_focus(Camera *camera, GPContext *context, int onoff) {
        CameraWidget		*widget = NULL, *child = NULL;
        CameraWidgetType	type;
        int			ret,val;

        ret = gp_camera_get_config (camera, &widget, context);
        if (ret < GP_OK) {
                fprintf (stderr, "camera_get_config failed: %d\n", ret);
                return ret;
        }
        ret = _lookup_widget (widget, "autofocusdrive", &child);
        if (ret < GP_OK) {
                fprintf (stderr, "lookup 'autofocusdrive' failed: %d\n", ret);
                goto out;
        }

        /* check that this is a toggle */
        ret = gp_widget_get_type (child, &type);
        if (ret < GP_OK) {
                fprintf (stderr, "widget get type failed: %d\n", ret);
                goto out;
        }
        switch (type) {
        case GP_WIDGET_TOGGLE:
                break;
        default:
                fprintf (stderr, "widget has bad type %d\n", type);
                ret = GP_ERROR_BAD_PARAMETERS;
                goto out;
        }

        ret = gp_widget_get_value (child, &val);
        if (ret < GP_OK) {
                fprintf (stderr, "could not get widget value: %d\n", ret);
                goto out;
        }

        val = onoff;

        ret = gp_widget_set_value (child, &val);
        if (ret < GP_OK) {
                fprintf (stderr, "could not set widget value to 1: %d\n", ret);
                goto out;
        }

        ret = gp_camera_set_config (camera, widget, context);
        if (ret < GP_OK) {
                fprintf (stderr, "could not set config tree to autofocus: %d\n", ret);
                goto out;
        }
out:
        gp_widget_free (widget);
        return ret;
}

/* Manual focusing a camera...
 * xx is -3 / -2 / -1 / 0 / 1 / 2 / 3
 */
int Gphoto2::camera_manual_focus (Camera *camera, int xx, GPContext *context) {
        CameraWidget		*widget = NULL, *child = NULL;
        CameraWidgetType	type;
        int			ret;
        float			rval;
        char			*mval;

        ret = gp_camera_get_config (camera, &widget, context);
        if (ret < GP_OK) {
                fprintf (stderr, "camera_get_config failed: %d\n", ret);
                return ret;
        }
        ret = _lookup_widget (widget, "manualfocusdrive", &child);
        if (ret < GP_OK) {
                fprintf (stderr, "lookup 'manualfocusdrive' failed: %d\n", ret);
                goto out;
        }

        /* check that this is a toggle */
        ret = gp_widget_get_type (child, &type);
        if (ret < GP_OK) {
                fprintf (stderr, "widget get type failed: %d\n", ret);
                goto out;
        }
        switch (type) {
        case GP_WIDGET_RADIO: {
                int choices = gp_widget_count_choices (child);

                ret = gp_widget_get_value (child, &mval);
                if (ret < GP_OK) {
                        fprintf (stderr, "could not get widget value: %d\n", ret);
                        goto out;
                }
                if (choices == 7) { /* see what Canon has in EOS_MFDrive */
                        ret = gp_widget_get_choice (child, xx+4, (const char**)&mval);
                        if (ret < GP_OK) {
                                fprintf (stderr, "could not get widget choice %d: %d\n", xx+2, ret);
                                goto out;
                        }
                        fprintf(stderr,"manual focus %d -> %s\n", xx, mval);
                }
                ret = gp_widget_set_value (child, mval);
                if (ret < GP_OK) {
                        fprintf (stderr, "could not set widget value to 1: %d\n", ret);
                        goto out;
                }
                break;
        }
        case GP_WIDGET_RANGE:
                ret = gp_widget_get_value (child, &rval);
                if (ret < GP_OK) {
                        fprintf (stderr, "could not get widget value: %d\n", ret);
                        goto out;
                }

                switch (xx) { /* Range is on Nikon from -32768 <-> 32768 */
                case -3:	rval = -1024;break;
                case -2:	rval =  -512;break;
                case -1:	rval =  -128;break;
                case  0:	rval =     0;break;
                case  1:	rval =   128;break;
                case  2:	rval =   512;break;
                case  3:	rval =  1024;break;

                default:	rval = xx;	break; /* hack */
                }

                fprintf(stderr,"manual focus %d -> %f\n", xx, rval);

                ret = gp_widget_set_value (child, &rval);
                if (ret < GP_OK) {
                        fprintf (stderr, "could not set widget value to 1: %d\n", ret);
                        goto out;
                }
                break;
        default:
                fprintf (stderr, "widget has bad type %d\n", type);
                ret = GP_ERROR_BAD_PARAMETERS;
                goto out;
        }


        ret = gp_camera_set_config (camera, widget, context);
        if (ret < GP_OK) {
                fprintf (stderr, "could not set config tree to autofocus: %d\n", ret);
                goto out;
        }
out:
        gp_widget_free (widget);
        return ret;
}

void Gphoto2::ctx_error_func (GPContext *context, const char *str, void *data)
{
        fprintf  (stderr, "\n*** Contexterror ***              \n%s\n",str);
        fflush   (stderr);
}

void Gphoto2::ctx_status_func (GPContext *context, const char *str, void *data)
{
    fprintf  (stderr, "%s\n", str);
    fflush   (stderr);
}
