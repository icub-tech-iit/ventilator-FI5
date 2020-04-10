
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_MEASURE_H_
#define _VNT_MEASURE_H_


#include "vnt_core.h"

// single point of entry for all measurement units. 
// except for vnt::core::Time which is extensively used and cannot be easily changed
    
namespace vnt { namespace measure {
      
    // - definitions of basic types together with their constants (NEVER use magic numbers)
    
    // Pression keeps ... expressed in .... cmH2O
    using Pressure = float;  
    constexpr Pressure pressure0    = 0;
    
    // Flow keeps ... expressed in ... l/min ???
    using Flow = float;
    constexpr Flow flow0            = 0;    

    // O2Percentage keeps ... expressed in ... ???
    using O2Percentage = float;
    constexpr O2Percentage o2percent0            = 0;   
    
    // ValveCtrl keeps ... expressed in ....  ????
    using ValveCtrl = float;  
    constexpr ValveCtrl valvectrl0  = 0;    
    
    // - definitions of timed types: they hold the value + the timestamp of their acquisition
    //   this design is wanted: we force the user to explicitly assign the .timestamp and .pressure instead of
    //   doing a dangerous {val1, val2} where the timestamp an the pressure values may be exchanged.
    //   when TimeXXX is derived from baseTimed, then we have slimmer code, we can access both timestamp and xxx
    //   and mostly important we prevent the mistake of exchanging xxx with time:
    //   TimedXXX evilcode { xxxconstant, vnt::core::now()};
    //struct BadTimedPressure
    //{
    //    vnt::core::Time timestamp {0};
    //    uint64_t  pressure {0}; 
    //};
    //   BadTimedPressure evilcode { vnt::measure::pressure0, vnt::core::now()};

    struct baseTimed
    {
       vnt::core::Time          timestamp {vnt::core::time0};
    };
    
    struct TimedPressure : public baseTimed
    {
        vnt::measure::Pressure  pressure {vnt::measure::pressure0};          
    };
    
    struct TimedFlow : public baseTimed
    {
        vnt::measure::Flow      flow {vnt::measure::flow0};      
    };    

    struct TimedO2Percentage : public baseTimed
    {
        vnt::measure::O2Percentage      o2percent {vnt::measure::o2percent0};      
    };     
    
    // - formatters: for print purposes or for transformations into other measurement units. 
    
    struct PressionFormatter
    {   
        vnt::measure::Pressure _p {vnt::measure::pressure0}; 
        
        PressionFormatter() = default;
        
        constexpr PressionFormatter(const vnt::measure::Pressure &p) 
        {
            load(p);            
        }
        
        constexpr void load(const vnt::measure::Pressure &p)
        {
            _p = p;              
        }
                                            
        std::string to_string() const
        {
            return std::to_string(_p) + " cmH2O"; 
        }  
    };    
            
    
}} // namespace vnt { namespace measure {



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
