
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

#include "vnt_bsp_trace.h"

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
 
#if 0
        inputs.S1_Pi = 0.F;         // S1 Pressure [code] (float)
    inputs.S2_Qi = 0.F;         // S2 Flow Rate [code] (float)
    inputs.S3_i[0] = 0.F;       // S3 Pressure [code] (float)
    inputs.S3_i[1] = 0.F;       // S3 Temperature [code] (float)
    inputs.S4_Pi = 0.F;         // S4 Pressure [code] (float)
    inputs.S5_Qi = 0.F;         // S5 Flow Rate [code] (float)
    inputs.S7_Pi = 0.F;         // S7 Pressure [code] (float)
#endif    
        struct Inp
        {
            float S1pressure {0};       // inputs.S1_Pi = 0.F;         // S1 Pressure [code] (float)
            float S2flowrate {0};       // inputs.S2_Qi = 0.F;         // S2 Flow Rate [code] (float)
            float S3pressure {0};       // inputs.S3_i[0] = 0.F;       // S3 Pressure [code] (float)
            float S3temperature {0};    // inputs.S3_i[1] = 0.F;       // S3 Temperature [code] (float)
            float S4pressure {0};       // inputs.S4_Pi = 0.F;         // S4 Pressure [code] (float)
            float S5flowrate {0};       // inputs.S5_Qi = 0.F;         // S5 Flow Rate [code] (float)
            float S7pressure {0};       // inputs.S7_Pi = 0.F;         // S7 Pressure [code] (float)
                  
            Inp() = default;
            
            void load(const board_sensor_data_t &sd)
            {
                S1pressure = sd.pressure2_raw;       // S1 Pressure [code] (float)
                S2flowrate = sd.flow1_raw;           // S2 Flow Rate [code] (float)
                S3pressure = sd.pressure4_raw;       // S3 Pressure [code] (float)
                S3temperature = sd.temperature4_raw;    // S3 Temperature [code] (float)
                S4pressure = sd.pressure3_raw;       // S4 Pressure [code] (float)
                S5flowrate = sd.flow2_raw;           // S5 Flow Rate [code] (float)
                S7pressure = sd.pressure1_raw;       // S7 Pressure [code] (float)
            }
    
            void print() const
            {
                vnt::bsp::trace::puts(std::string("Inp:") +
                                " .S1pressure = " + std::to_string(S1pressure) +
                                " .S2flowrate = " + std::to_string(S2flowrate) +
                                " .S3pressure = " + std::to_string(S3pressure) +
                                " .S3temperature = " + std::to_string(S3temperature) +
                                " .S4pressure = " + std::to_string(S4pressure) +
                                " .S5flowrate = " + std::to_string(S5flowrate) +
                                " .S7pressure = " + std::to_string(S7pressure) 
                );
            }
        };
        
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
            void print() const
            {
                vnt::bsp::trace::puts(std::string("Out:") +
                                " .CPvalvePerc = " + std::to_string(CPvalvePerc) +
                                " .CFBvalveON = " + std::to_string(CFBvalveON) +
                                " .targetFlowRate = " + std::to_string(targetFlowRate) +
                                " .targetPressure = " + std::to_string(targetPressure) +
                                " .filtS1pressure = " + std::to_string(filtS1pressure) +
                                " .S2flowrate = " + std::to_string(S2flowrate) +
                                " .filtS3pressure = " + std::to_string(filtS3pressure) +
                                " .S3temperature = " + std::to_string(S3temperature) +
                                " .filtS4pressure = " + std::to_string(filtS4pressure) +   
                                " .S5flowrate = " + std::to_string(S5flowrate) +    
                                " .filtS7pressure = " + std::to_string(filtS7pressure) +    
                                " .estTidalVolume = " + std::to_string(estTidalVolume) +    
                                " .assistedVentilTrigger = " + std::to_string(assistedVentilTrigger)
                );
            }
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

        //bool set(const Input &inp);
        
        bool set(const board_sensor_data_t &sd);
    
        bool tick();

        //const Output& get() const; 
        
        const Out& getOut() const;
        
        const Inp& getInp() const;

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

