
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------


#ifndef APP_THECONTROLLER_H_
#define APP_THECONTROLLER_H_



#include "vnt_core.h"

#include "board_driver.h"

#include <array>

namespace app {

    using dataFmt = float;
    
    // so far just arrays, later on: maybe struct with what we want
    using Input = std::array<dataFmt, 7>;
    #if 0
    inputs.S1_Pi = 0.F;         // S1 Pressure [code] (float)
    inputs.S2_Qi = 0.F;         // S2 Flow Rate [code] (float)
    inputs.S3_i[0] = 0.F;       // S3 Pressure [code] (float)
    inputs.S3_i[1] = 0.F;       // S3 Temperature [code] (float)
    inputs.S4_Pi = 0.F;         // S4 Pressure [code] (float)
    inputs.S5_Qi = 0.F;         // S5 Flow Rate [code] (float)
    inputs.S7_Pi = 0.F;         // S7 Pressure [code] (float)
        // the others are FS
    inputs.params[0] = 50.F;    // Zephyr FS [SLPM] (float)
    inputs.params[1] = 160.F;   // HSC LP FS [mbar] (float)
    inputs.params[2] = 10000.F; // HSC HP FS [mbar] (float)       
    #endif
    
    bool fillCTRLinput(const board_sensor_data_t &sd, Input &in);


    
    using Output = std::array<dataFmt, 13>;
    #if 0
    outputs.IV_dc;              // CP valve DC command [%] (float)
    outputs.OV_cmd;             // CFB valve ON/OFF command [-] (boolean)
    outputs.Q_ref;              // Target Flow Rate [L/min] (float)
    outputs.P_ref;              // Target Pressure [cmH2O] (float)
    outputs.signals[0];         // S1 Filtered Pressure [cmH2O] (float)
    outputs.signals[1];         // S2 Flow Rate [L/min] (float)
    outputs.signals[2];         // S3 Filtered Pressure [cmH2O] (float)
    outputs.signals[3];         // S3 Temperature [°C] (float)
    outputs.signals[4];         // S4 Filtered Pressure [cmH2O] (float)
    outputs.signals[5];         // S5 Flow Rate [L/min] (float)
    outputs.signals[6];         // S7 Filtered Pressure [cmH2O] (float)
    outputs.signals[7];         // Tidal Volume estimates [L] (float)
    outputs.signals[8];         // Assisted Ventilation Trigger [-] (float)
    #endif
    
    
    class theController
    {
    public:
        
        struct Out
        {
            float CPvalvePerc {0};      // outputs.IV_dc;              // CP valve DC command [%] (float)
            bool CFBvalveON {false};    // outputs.OV_cmd;             // CFB valve ON/OFF command [-] (boolean)
            float targetFlowRate {0};   // outputs.Q_ref;              // Target Flow Rate [L/min] (float)
            float targetPressure {0};   // outputs.P_ref;              // Target Pressure [cmH2O] (float)
            float filtS1pressure {0};   // outputs.signals[0];         // S1 Filtered Pressure [cmH2O] (float)
            float S2flowrate {0};       // outputs.signals[1];         // S2 Flow Rate [L/min] (float)
            float filtS3pressure {0};   // outputs.signals[2];         // S3 Filtered Pressure [cmH2O] (float)
            float S3temperature {0};    // outputs.signals[3];         // S3 Temperature [°C] (float)
            float filtS4pressure {0};   // outputs.signals[4];         // S4 Filtered Pressure [cmH2O] (float)
            float S5flowrate {0};       // outputs.signals[5];         // S5 Flow Rate [L/min] (float)
            float filtS7pressure {0};   // outputs.signals[6];         // S7 Filtered Pressure [cmH2O] (float)
            float estTidalVolume {0};   // outputs.signals[7];         // Tidal Volume estimates [L] (float)
            float assistedVentilTrigger {0}; //outputs.signals[8];         // Assisted Ventilation Trigger [-] (float)            
            Out() = default;
        };
        
        struct Config
        {   
            std::uint32_t tbd {0};      
            Config() = default;
            bool isvalid() const { return true; }
        };
        
        enum class Mode { IDLE = 1 };

        static theController& getInstance();
            
        bool init(const Config &cfg);
        
        bool set(Mode mode);

        bool set(const Input &inp);
        bool set(const board_sensor_data_t &sd);
    
        bool tick();

        const Output& get() const; 
        
        const Out& getOut() const;

    public:
        ~theController();

    private:
        theController();    
        
    private:        
        struct Impl;
        std::unique_ptr<Impl> pImpl;     
    };    
    
} // namespace app


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

