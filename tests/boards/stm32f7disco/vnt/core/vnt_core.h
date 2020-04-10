
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware-shared for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_CORE_H_
#define _VNT_CORE_H_


#include <cstdint>
#include <cstring>
#include <type_traits>
#include <string>

namespace vnt { namespace core {
    
    // data structures
        
    // the time is always expressed in micro-seconds
    
    // Time keeps absolute time since bootstrap. by using 8-bytes it expresses up to 599 years.
    using Time          = std::uint64_t;  
    // relTime keep relative time (timeouts, periods, ect). by using 4-bytes it expresses at least up to 1 hour
    using relTime       = std::uint32_t;    
    
    // some useful Time constants: use 5*time1millisec rather than 5000
    constexpr Time time0            = 0;
    constexpr Time time1microsec    = 1;    
    constexpr Time time1millisec    = 1000;
    constexpr Time time1second      = 1000000;
    constexpr Time time1minute      = 60*time1second;
    constexpr Time time1hour        = 60*time1minute;
    constexpr Time time1day         = 24*time1hour;
    constexpr Time timeNone         = 0xffffffffffffffff;   // used to express an invalid time
    
    // the above, up to time1hour, can be used also as relTime constants. but here are others used by blocking calls.
    constexpr relTime reltimeWaitNone     = 0;                 // the call does not apply any timeout            
    constexpr relTime reltimeWaitForever  = 0xffffffff;        // the call has an infinite timeout      
    
    // it formats Time (or relTime) for print purposes
    struct TimeFormatter
    {   
        uint16_t    D {0}; // day
        uint8_t     H {0}; // hour
        uint8_t     M {0}; // minute
        uint8_t     S {0}; // second
        uint16_t    m {0}; // milli
        uint16_t    u {0}; // micro
        
        TimeFormatter() = default;
        
        constexpr TimeFormatter(const vnt::core::Time &t) 
        {
            load(t);            
        }
        
        constexpr void load(const vnt::core::Time &t)
        {
            uint64_t tmp = t;
            u = tmp%1000; tmp /= 1000;
            m = tmp%1000; tmp /= 1000;
            S = tmp%60; tmp /= 60;
            M = tmp%60; tmp /= 60;
            H = tmp%24; tmp /= 24;
            D = tmp;              
        }
                
        constexpr uint64_t to_seconds() const { return S+60*M+3600*H+86400*D; }  

        constexpr uint64_t to_milliseconds() const { return 1000*to_seconds() + m; }          
        
        constexpr vnt::core::Time to_time() const
        {
            return static_cast<vnt::core::Time>(u) + time1millisec*static_cast<vnt::core::Time>(m) + time1second*to_seconds();
        }        
        
        enum class Mode : std::uint8_t { compact, full, milli };
        
        std::string to_string(const Mode &mode = Mode::compact) const
        {
            char _milli[4] = {0};
            char _micro[4] = {0};
            snprintf(_milli, sizeof(_milli), "%03d", m);
            snprintf(_micro, sizeof(_micro), "%03d", u);
            switch(mode)
            {
                default:
                case Mode::compact:
                {
                    //return "S" + std::to_string(to_seconds()) + ":m" + std::to_string(m) + ":u" + std::to_string(u);
                    return "S" + std::to_string(to_seconds()) + ":m" + _milli + ":u" + _micro;                    
                } break;
                
                case Mode::full:
                {
                    //return "D" + std::to_string(D) + ":H" + std::to_string(H) + ":M" + std::to_string(M) + ":S" + std::to_string(S) + ":m" + std::to_string(m) + ":u" + std::to_string(u); 
                    return "D" + std::to_string(D) + ":H" + std::to_string(H) + ":M" + std::to_string(M) + ":S" + std::to_string(S) + ":m" + _milli + ":u" + _micro; 
                } break;
                
                case Mode::milli:
                {
                    //return "m" + std::to_string(to_milliseconds()) + ":u" + std::to_string(u);
                    return "m" + std::to_string(to_milliseconds()) + ":u" + _micro;
                } break;                                
            }
        }  

        std::string delta_string(const vnt::core::Time &t, const Mode &mode = Mode::compact) const
        {
            int64_t d = t - to_time(); 
            bool neg = d<0;
            if(neg)
            {
                d = -d;
            }
            TimeFormatter tfx(d);
            return (neg) ? ("-" + tfx.to_string(mode)) : (tfx.to_string(mode));                        
        }        

    };    
        
    
    // some pointer to functions
    using fpGetU32      = std::uint32_t (*)(void);
    using fpGetU64      = std::uint64_t (*)(void);
    using fpParU32      = void (*)(std::uint32_t);
    using fpBoolParU32  = bool (*)(std::uint32_t);
    using fpI08ParU32   = std::int8_t (*)(std::uint32_t);
    using fpWorker      = void (*)(void);
    using fpCaller      = void (*)(void *);
    
    // useful converter for enum class
    template<typename E>         // see C++14 [Meyers, pag. 73]
    constexpr auto tointegral(E enumerator) noexcept 
    {
        return static_cast<std::underlying_type_t<E>>(enumerator);
    }   
             
    // it represents a function call with a generic argument
    struct Callback
    {
        fpCaller call {nullptr};
        void * arg {nullptr};
        
        Callback() = default;
        constexpr Callback(fpCaller _c, void *_a) : call(_c), arg(_a) {}
        void load(fpCaller _c, void *_a) { call = _c; arg = _a; }
        void clear() { call = nullptr; arg = nullptr; }
        bool isvalid() const { if(nullptr != call) { return true; } else { return false; } } 
        void execute() const { if(true == isvalid()) { call(arg); } }
    };

    // it holds generic data plus its capacity
    struct Data
    {    
        void * pointer {nullptr};
        size_t capacity {0};
        
        Data() = default;
        constexpr Data(void *p, const size_t s) : pointer(p), capacity(s) {}
                    
        void load(void *littleendianmemory, const size_t s) { pointer = littleendianmemory; capacity = s; }
        void clear() { pointer = nullptr; capacity = 0; }
        bool isvalid() const { if((nullptr != pointer) && (0 != capacity)){ return true; } else { return false; } } 
      
        std::uint8_t  * getU08ptr() const { return reinterpret_cast<std::uint8_t*>(pointer); }  
        std::uint16_t * getU16ptr() const { return reinterpret_cast<std::uint16_t*>(pointer); } 
        std::uint32_t * getU32ptr() const { return reinterpret_cast<std::uint32_t*>(pointer); } 
        std::uint64_t * getU64ptr() const { return reinterpret_cast<std::uint64_t*>(pointer); } 
        
        std::uint8_t  getU08val(size_t pos) const { if(isvalid() && (pos < capacity)) { return getU08ptr()[pos]; } else { return 0; } }      
        std::uint16_t getU16val(size_t pos) const { if(isvalid() && ((2*pos) < capacity)) { return getU16ptr()[pos]; } else { return 0; } }
        std::uint32_t getU32val(size_t pos) const { if(isvalid() && ((4*pos) < capacity)) { return getU32ptr()[pos]; } else { return 0; } }       
        std::uint64_t getU64val(size_t pos) const { if(isvalid() && ((8*pos) < capacity)) { return getU64ptr()[pos]; } else { return 0; } }
        
        void * getVOIDptr(size_t offset = 0) const { if(isvalid() && (offset < capacity)) { std::uint8_t *d = getU08ptr(); return &d[offset]; } else { return nullptr;} }      
    };    

    
    // initialization of vnt::core
    // to use vnt::core as a standalone, you must call vnt::core::init(cfg), where cfg specifies a time base
    
    struct TimeConfig
    {         
        vnt::core::fpWorker init {nullptr};  
        vnt::core::fpGetU64 get {nullptr};         
        
        constexpr TimeConfig() = default;
        constexpr TimeConfig(vnt::core::fpWorker _init, vnt::core::fpGetU64 _get) : init(_init), get(_get) {}
        bool isvalid() const { if(nullptr != get) { return true; } else { return false; } }
    };    
    
    struct Config
    {         
        TimeConfig timeconfig {};         
        
        constexpr Config() = default;
        constexpr Config(const TimeConfig &tc) : timeconfig(tc.init, tc.get) {}
        bool isvalid() const { return timeconfig.isvalid(); }
    }; 
       
    
    bool initialised();
    
    bool init(const Config &config);

    vnt::core::Time now();
    
    void delay(vnt::core::Time t);
     
    
}} // namespace vnt { namespace core {



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
