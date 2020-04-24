
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

#include <array>

namespace app {

    using dataFmt = float;
    

    // so far just arrays, later on: maybe struct with what we want
    using Input = std::array<dataFmt, 4>;
    using Output = std::array<dataFmt, 5>;
    
    class theController
    {
    public:
        
        struct Config
        {   
            std::uint32_t tbd {0};      
            Config() = default;
            bool isvalid() const { return true; }
        };

        static theController& getInstance();
            
        bool init(const Config &cfg);

        bool set(const Input &inp);
    
        bool tick();

        const Output& get() const; 

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

