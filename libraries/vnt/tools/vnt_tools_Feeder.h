
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------


#ifndef _VNT_TOOLS_FEEDER_H_
#define _VNT_TOOLS_FEEDER_H_



#include "vnt_core.h"

namespace vnt { namespace tools { 

    using dataFmt = float;
    
    struct Result
    {
        dataFmt v0 {0};
        dataFmt v1 {0};
        dataFmt v2 {0};
        dataFmt v3 {0};
        constexpr Result() = default;  
        constexpr Result(const dataFmt a, const dataFmt b, const dataFmt c, const dataFmt d) : v0(a), v1(b), v2(c), v3(d) {}
    };
    
    class Feeder
    {
    public:
        
        struct Config
        {   
            std::uint32_t tbd {0};      
            Config() = default;
            bool isvalid() const { return true; }
        };
            
        
        Feeder();
        ~Feeder();
    
        bool get(dataFmt &v);
        
        bool check(const Result &r, vnt::tools::Result &ref);

    private:        
        struct Impl;
        Impl *pImpl;    
    };    
    
} } // namespace embot { namespace tools {



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

