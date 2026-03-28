#ifndef __ZELDA_CONFIG_H__
#define __ZELDA_CONFIG_H__

#include <filesystem>
#include <string_view>
#include "ultramodern/config.hpp"

namespace zelda64 {
    inline const std::u8string program_id = u8"Starfox64Recompiled";
    inline const std::string program_name = "Starfox 64: Recompiled";

    namespace configkeys {
        namespace general {
            inline const std::string debug_mode = "debug_mode";
            inline const std::string film_grain_mode = "film_grain_mode";
            inline const std::string invert_y_axis = "invert_y_axis";
            inline const std::string radio_comm_mode = "radio_comm_mode";
            #if 0
            inline const std::string autosave_mode = "autosave_mode";
            inline const std::string camera_invert_mode = "camera_invert_mode";
            inline const std::string analog_cam_mode = "analog_cam_mode";
            inline const std::string analog_camera_invert_mode = "analog_camera_invert_mode";
            #endif
        }

        namespace sound {
            inline const std::string bgm_volume = "bgm_volume";
            inline const std::string sfx_volume = "sfx_volume";
            inline const std::string voice_volume = "voice_volume";
            #if 0
            inline const std::string low_health_beeps = "low_health_beeps";
            #endif
        }

        namespace graphics {
        }
    }

    // TODO: Move loading configs to the runtime once we have a way to allow per-project customization.
    void init_config();
    
    bool get_debug_mode_enabled();

    enum class AutosaveMode {
        Off,
        On,
    };

    AutosaveMode get_autosave_mode();
    
    enum class FilmGrainMode {
        On,
        Off
    };
    
    FilmGrainMode get_film_grain_mode();

    enum class RadioCommBoxMode {
        Expand,
        Original
    };
    
    RadioCommBoxMode get_radio_comm_mode();
    
    enum class InvertYAxisMode {
        On,
        Off
    };
    
    InvertYAxisMode get_invert_y_axis_mode();

    enum class CameraInvertMode {
        InvertNone,
        InvertX,
        InvertY,
        InvertBoth
    };
    
    CameraInvertMode get_camera_invert_mode();
    CameraInvertMode get_analog_camera_invert_mode();

    enum class AnalogCamMode {
        Off,
        On,
    };
    
    AnalogCamMode get_analog_cam_mode();

    #if 0
    enum class LowHealthBeepsMode {
        Off,
        On,
    };
    #endif
};

#endif
