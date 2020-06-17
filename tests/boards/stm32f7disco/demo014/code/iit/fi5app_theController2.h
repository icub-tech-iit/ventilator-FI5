
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------


#ifndef APP_THECONTROLLER2_H_
#define APP_THECONTROLLER2_H_


#include "vnt_core.h"
#include "vnt_core_binary.h"

#include "vnt_bsp_trace.h"

#include "board_driver.h"

#include <array>

namespace fi5app {
    
    struct knob_t 
    {
        struct Cfg
        {
            uint32_t _min {0};
            uint32_t _max {1024};
            bool quantization {false};
            uint32_t levelsofquantization {100};            
            constexpr Cfg(uint32_t mi, uint32_t ma, bool q = false, uint32_t l = 100) : _min(mi), _max(ma), quantization(q), levelsofquantization(l) {}        
        };
        
        static constexpr float _scale = 1.0f;
        float _bitresolution {1024};

        Cfg _cfg {0, 1024};
        
        constexpr knob_t(const Cfg &c) : _cfg(c) 
        {   // dont look at what happens if max is < min, both are zero etc. dont have time now BUT ............. hei i must do it
            _bitresolution = _scale / static_cast<float>(_cfg._max - _cfg._min);
        }
        
        float get(const uint32_t input) const
        {
            float v = std::max(std::min(input, _cfg._max), _cfg._min); // clipped in in [_min, _max]
            v -= _cfg._min; // removed _min: now we have range [0, _max-_min]            
            float r = v*_bitresolution;
            if((true == _cfg.quantization) && (0 != _cfg.levelsofquantization))
            {
                uint32_t q = static_cast<uint32_t>(static_cast<float>(_cfg.levelsofquantization)*r + 0.5f);
                r =  q / static_cast<float>(_cfg.levelsofquantization);
            }
            return r; // now we rescale in [0, 1]
        }
        
    };
           
    class theController2
    {
    public:
        
        enum class Mode { IDLE = 1, CPAP = 2, VCV = 3, PRCV = 4 };
        
        // buttons and leds are associated to Mode
        static constexpr uint8_t btnNUM = 4;
        static constexpr uint8_t ledNUM = 4;
        
        // knobs are associated to Regulation
        enum class Regulation { FREQ = 0, PMAX = 1, IERATIO = 2, TIDALVOL = 3 };
        static constexpr uint8_t knbNUM = 4;
        
        // this is the input for both fsm and controller
        struct Inp
        {           
            float S1pressure {0};       // inputs.S1_Pi = 0.F;         // S1 Pressure [code] (float)
            float S2flowrate {0};       // inputs.S2_Qi = 0.F;         // S2 Flow Rate [code] (float)
            float S3pressure {0};       // inputs.S3_i[0] = 0.F;       // S3 Pressure [code] (float)
            float S3temperature {0};    // inputs.S3_i[1] = 0.F;       // S3 Temperature [code] (float)
            float S4pressure {0};       // inputs.S4_Pi = 0.F;         // S4 Pressure [code] (float)
            float S5flowrate {0};       // inputs.S5_Qi = 0.F;         // S5 Flow Rate [code] (float)
            float S7pressure {0};       // inputs.S7_Pi = 0.F;         // S7 Pressure [code] (float)
            std::array<bool, btnNUM> BTNpressed {false};
            std::array<float, knbNUM> KNBvalue {0};   // normalised [0, 1]
                        
                  
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
                
                // conversion from buttons to pressed info is easy apart a remap of the positions
                static constexpr uint8_t btnPOSidle = 3;
                static constexpr uint8_t btnPOScpap = 2; 
                static constexpr uint8_t btnPOSvcv = 1;  
                static constexpr uint8_t btnPOSprcv = 0; 
                static constexpr uint8_t btnmap[btnNUM] = {btnPOSidle, btnPOScpap, btnPOSvcv, btnPOSprcv};
                for(int i=0; i< btnNUM; i++) BTNpressed[i] = vnt::core::binary::bit::check(sd.buttons, btnmap[i]);
                    
//                static constexpr uint8_t knbPOSfreq = 0;
//                static constexpr uint8_t kntPOSpmax = 1; 
//                static constexpr uint8_t kntPOSieratio = 2;  
//                static constexpr uint8_t kntPOStidalvol = 3; 
                    
                // conversions from analog_input to normalised values of knobs is less easy.
                // - we need to rescale uint16_t values into another range.
                    
                // min and max are assigned upon readings
                
//                constexpr knob_t knobFreq {{77, 400}}; 
//                constexpr knob_t knobPmax {{58, 400}}; 
//                constexpr knob_t knobIEratio {{46, 400}}; 
//                constexpr knob_t knobTidalVolum {{56, 400}}; 
                
//                constexpr knob_t knobFreq {{32, 432}}; 
//                constexpr knob_t knobPmax {{32, 432}}; 
//                constexpr knob_t knobIEratio {{32, 432}}; 
//                constexpr knob_t knobTidalVolum {{32, 432}}; 
                constexpr uint32_t f = 1;
                constexpr bool quantize = false;
                constexpr knob_t knobFreq {{32, 432, quantize, f*35}}; 
                constexpr knob_t knobPmax {{32, 432, quantize, f*55}}; 
                constexpr knob_t knobIEratio {{32, 432, quantize, f*15}}; 
                constexpr knob_t knobTidalVolum {{32, 432, quantize, f*18}}; 

//                constexpr knob_t knobFreq {{77, 400, true, 35}}; 
//                constexpr knob_t knobPmax {{58, 400, true, 55}}; 
//                constexpr knob_t knobIEratio {{46, 400, true, 15}}; 
//                constexpr knob_t knobTidalVolum {{56, 400, true, 18}}; 
                             
                // luckily the order is w/out remap
                for(int i=0; i<4; i++)
                {    
//                    constexpr uint8_t factor = 4;                    
//                    volatile uint16_t t = sd.analog_input[i] >> factor;
//                    t <<= factor;
                    uint16_t t = sd.analog_input[i];
                    KNBvalue[i] = knobFreq.get(t);
                }
//                KNBvalue[0] = knobFreq.get(sd.analog_input[0]);
//                KNBvalue[1] = knobPmax.get(sd.analog_input[1]);
//                KNBvalue[2] = knobIEratio.get(sd.analog_input[2]);
//                KNBvalue[3] = knobTidalVolum.get(sd.analog_input[3]);                
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
            
            void printknobs() const
            {
                vnt::bsp::trace::puts(std::string("Inp: ") +
                                        " to fsm knobs = [" +
                                        std::to_string(KNBvalue[0]) + ", " +
                                        std::to_string(KNBvalue[1]) + ", " +
                                        std::to_string(KNBvalue[2]) + ", " +
                                        std::to_string(KNBvalue[3]) + "]"
                );
            }
            
            
        };
        
        struct ctrlOut
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
            float maxTidalVolume2Cycles {0};   // outputs.signals[8];  // Maximum Tidal Volume per 2 cycles [L] (float)
            float assistedVentilTrigger {0}; //outputs.signals[9];         // Assisted Ventilation Trigger [-] (float)            
            ctrlOut() = default;
            void print() const
            {
                vnt::bsp::trace::puts(std::string("ctrlOut:") +
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
        
        struct fsmOut
        {
            
            std::array<bool, ledNUM> LEDstates {false};            
            std::array<float, knbNUM> KNBgui {0};  
            
            int knobs_transfer {0}; 
            //#warning check: knobs_transfer  // (enum MapIndex) trigger to update controller’s params
            float testCPvalvePerc {0};      // output.test_IV_dc;       // (float) to be used to command CP valve in TEST
            bool testCFBvalveON {false};    // outputs.test_OV_cmd;      // (boolean) to be used to command CFB valve in TEST
            bool soundALARM {false};        // outputs.soundALARM;       // (boolean) to be used to activate the buzzer
            int idALARM {0};                // outputs.idALARM;          // (enum AlarmIndex) to be used for alarms messages LUT
            bool LEDalarm {false};
            Mode mode {Mode::IDLE};         // outputs.Controller_Mode;  // (enum ControlMode {IDLE, CPAP, VCV, PRVC})

            
            fsmOut() = default;
            void print() const
            {
                vnt::bsp::trace::puts(std::string("fsmOut:") + 
                                        " TBD" 
                );
            }
        };  


        struct Out 
        {
            float CPvalvePerc {0};      
            bool CFBvalveON {false};   

            float estTidalVolume {0};            
            float filtS1pressure {0};
            float S2flowrate {0};
            float S5flowrate {0};
                        
            std::array<bool, ledNUM+1> leds {false};            
            std::array<float, knbNUM> knobs {0};
            bool buzzer {false};  
            
            Out() = default;
            
            void print() const
            {
                vnt::bsp::trace::puts(std::string("Out:") + 
                                        " fsm knobs = [" +
                                        std::to_string(knobs[0]) + ", " +
                                        std::to_string(knobs[1]) + ", " +
                                        std::to_string(knobs[2]) + ", " +
                                        std::to_string(knobs[3]) + "]"
                );
            }
        };            
        
        struct Config
        {   
            std::uint32_t tbd {0};      
            Config() = default;
            bool isvalid() const { return true; }
        };
        

        static theController2& getInstance();
            
        bool init(const Config &cfg);
        
        bool set(Mode mode);
        
        bool load(const board_sensor_data_t &sd);
    
        bool tick();
        
        const Out& getOut() const;

        const ctrlOut& get_ctrlOut() const;
        
        const fsmOut& get_fsmOut() const;
        
        const Inp& getInp() const;
        
    public:
        ~theController2();

    private:
        theController2();    
        
    private:        
        struct Impl;
        std::unique_ptr<Impl> pImpl;     
    };    
    
} // namespace fi5app


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

