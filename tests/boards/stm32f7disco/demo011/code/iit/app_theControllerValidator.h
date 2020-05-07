
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------


#ifndef APP_THECONTROLLERVALIDATOR_H_
#define APP_THECONTROLLERVALIDATOR_H_



#include "vnt_core.h"

#include "app_theController.h" // so far to just see app::Input etc

namespace app {
    
    class theControllerValidator
    {
    public:
        
        struct Config
        {   
            std::uint32_t tbd {0};      
            Config() = default;
            bool isvalid() const { return true; }
        };
            
        
        static theControllerValidator& getInstance();
            
        bool init(const Config &cfg);

        bool get(Input &input, Output &target);
    
        bool check(const Output &candidate, const Output &target, float &ae);

    public:
        ~theControllerValidator();

    private:
        theControllerValidator();    
        
    private:        
        struct Impl;
        std::unique_ptr<Impl> pImpl;   
    };    
    
} // namespace app {



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

