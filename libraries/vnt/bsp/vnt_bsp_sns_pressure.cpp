
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

#if defined(VNTcfg_FI5)
    // nothing so far
#else

    // we dont use it anymore. it was used just for test

#define TEST_VNT_BSP_SNS_PRESSURE

// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_bsp_sns_pressure.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_core_binary.h"

// it will uses the stm32hal ...
#include "stm32hal.h"

#if defined(TEST_VNT_BSP_SNS_PRESSURE)
#include "vnt_os_Timer.h"
#include "vnt_os_theCallbackManager.h"
#include <array>

std::array<vnt::os::Timer*, vnt::core::tointegral(vnt::bsp::sns::PRS::maxnumberof)> test_timers;
constexpr std::array<vnt::core::relTime, vnt::core::tointegral(vnt::bsp::sns::PRS::maxnumberof)> test_delays = {
    1*vnt::core::time1millisec, 
    2*vnt::core::time1millisec + 200*vnt::core::time1microsec,
    3*vnt::core::time1millisec
};

std::array<vnt::measure::Pressure, vnt::core::tointegral(vnt::bsp::sns::PRS::maxnumberof)> test_values = { 1, 2, 3};

#endif

namespace vnt { namespace bsp { namespace sns { namespace pressure {
    
    
    constexpr uint32_t supportedmask = vnt::core::binary::mask::pos2mask<uint32_t>(sns::PRS::S1) |
                                       vnt::core::binary::mask::pos2mask<uint32_t>(sns::PRS::S2) |
                                       vnt::core::binary::mask::pos2mask<uint32_t>(sns::PRS::S3);

    static std::uint32_t initialisedmask = 0;
    
    
    bool supported(vnt::bsp::sns::PRS s)
    {
        return vnt::core::binary::bit::check(supportedmask, vnt::core::tointegral(s));
    }
  
    bool initialised(vnt::bsp::sns::PRS s)
    {   // tells if the particular s is already intialised or not.
        return vnt::core::binary::bit::check(initialisedmask, vnt::core::tointegral(s));        
    }
    
    
    result_t init(vnt::bsp::sns::PRS s, const Config &config)
    {
        if(!supported(s))
        {
            return resNOK;
        }  
        
        if(initialised(s))
        {   // dont need to re-init
            return resOK;
        }
        
        
        if(!vnt::bsp::initialised())
        {   // requires vnt::hw::bsp::init()
            return resNOK;
        }
        
        // TODO_CODE: start
        // in here add the code for initialization of the specific chip
        
        test_timers[vnt::core::tointegral(s)] = new vnt::os::Timer;
        
        // TODO_CODE: end
        
        vnt::core::binary::bit::set(initialisedmask, vnt::core::tointegral(s));
        
        return resOK;     
    }
    
    
    // - example of blocking calls w/ timeout as developed in embot: see the i2c::ping() + i2c::read()
#if 0
    
    bool isbusy(I2C b)
    {
        if(false == initialised(b))
        {
            return false;
        } 
        // marco.accame: in here `ongoing` is a `volatile` boolean variable set true by user-space and set false by the IRQhandler
        return s_privatedata.transaction[embot::core::tointegral(b)].ongoing;     
    }
    
    bool ping(I2C b, ADR adr, embot::core::relTime timeout)
    {
        if(false == initialised(b))
        {
            return false;
        } 
          
        embot::core::relTime remaining = timeout;
        if(true == isbusy(b, timeout, remaining))
        {
            return false;
        }
        
        std::uint8_t index = embot::core::tointegral(b);
        
        s_privatedata.transaction[index].ongoing = true;
                
        HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(s_privatedata.handles[index], adr, 1, remaining/1000);
        
        s_privatedata.transaction[index].ongoing = false;
        
        return (HAL_OK == status) ? true : false;       
    }
    
    static result_t s_wait(I2C b, embot::core::relTime timeout)
    {
        embot::core::Time start = embot::core::now();
        
        result_t res = resOK;
        for(;;)
        {
            if(false == isbusy(b))
            {
                break;
            }
            
            if(embot::core::now() > (start+timeout))
            {
                res = resNOKtimeout;
                break;
            }
        } 

        return res;
    }   
    
    result_t read(I2C b, ADR adr, REG reg, embot::core::Data &destination, embot::core::relTime timeout)
    {
        if(false == initialised(b))
        {
            return resNOK;
        } 
             
        if(false == destination.isvalid())
        {
            return resNOK;
        }
        
        embot::core::relTime remaining = timeout;       
        if(true == isbusy(b, timeout, remaining))
        {
            return resNOKtimeout;
        }
                        
        result_t r = s_read(b, adr, reg, destination);
        
        if(resOK == r)
        {
            r = s_wait(b, remaining); // in here s_wait() just waits until completion for a maximum of remaining time        
        }        
        return r;                      
    }    
    
#endif  

    result_t read(vnt::bsp::sns::PRS s, vnt::measure::Pressure &value, const vnt::core::relTime timeout)
    {
        if(false == initialised(s))
        {
            return resNOK;
        }   

        value = vnt::measure::pressure0;        
        
        // TODO_CODE: start
        // in here add the code for reading the value of the chip. return resNOKtimeout or resOK
        // value will contain the measure read from teh chip
        
        
        
        // TODO_CODE: end
        
        return resOK;       
    }
 
    result_t read(vnt::bsp::sns::PRS s, vnt::measure::TimedPressure &timedvalue, const vnt::core::relTime timeout)
    {
        if(false == initialised(s))
        {
            return resNOK;
        }    
        
        timedvalue.pressure = vnt::measure::pressure0;
        timedvalue.timestamp = vnt::core::now();
        
        // TODO_CODE: start
        // in here add the code for reading the value of the chip. return resNOKtimeout or resOK
        // timedvalue.pressure will contain the measure read from teh chip
        // timedvalue.timestamp will contain the time, retrieved with vnt::core::now()
        
        
        
        // TODO_CODE: end
        
        return resOK;       
    }
    
    
    result_t read(vnt::bsp::sns::PRS s, vnt::measure::Pressure *pvalue,  const vnt::core::Callback &oncompletion)
    {
        if(false == initialised(s) || (nullptr == pvalue))
        {
            return resNOK;
        }    
        
        *pvalue = vnt::measure::pressure0;
        
        // TODO_CODE: start
        // in here add the code for starting reading from the chip. and if ok we return resOK
        // and let the reading go in background. when everything is done, the IRQHandler will
        // - copy the measure into memory pointed by value
        // - execute the callback 
        // an alternative if the IRQhandler is compiled in a .c module is to execute oncompletion.call(arg)
        // where arg is the pointer to the just read measure. 

        
        
        // TODO_CODE: end
        
        return resOK;                
    }
    
    result_t read(vnt::bsp::sns::PRS s, vnt::measure::TimedPressure *ptimedvalue, const vnt::core::Callback &oncompletion)
    {
        if(false == initialised(s) || (nullptr == ptimedvalue))
        {
            return resNOK;
        }    
        
        ptimedvalue->pressure = vnt::measure::pressure0;
        ptimedvalue->timestamp = vnt::core::now();
        
        // TODO_CODE: start
        // in here add the code for starting reading from the chip. and if ok we return resOK
        // and let the reading go in background. when everything is done, the IRQHandler will
        // - copy the measure into memory pointed by &(ptimedvalue->pressure)
        // - copy into memory pointed by &(ptimedvalue->timestamp) the time retrieved w/ vnt::core::now() which can
        //   be safely called inside an IRQHandler
        // - execute the callback 
        // an alternative if the IRQhandler is compiled in a .c module is to execute oncompletion.call(arg)
        // where arg is the pointer to the just read measure.

#if defined(TEST_VNT_BSP_SNS_PRESSURE)
       
        // ok, i tell what it is: we start a timer which at its expiry asks the callback manager to exec the callback, which send the event to the thread
        // and i already set the value
        vnt::os::Action act(vnt::os::CallbackToThread(oncompletion, vnt::os::theCallbackManager::getInstance().thread()));
        vnt::os::Timer::Config cfg{test_delays[vnt::core::tointegral(s)], act, vnt::os::Timer::Mode::oneshot, 0};
        test_timers[vnt::core::tointegral(s)]->start(cfg);
        ptimedvalue->timestamp = vnt::core::now() + test_delays[vnt::core::tointegral(s)];
        ptimedvalue->pressure = test_values[vnt::core::tointegral(s)];
        test_values[vnt::core::tointegral(s)] += 0.01;

#endif        
        
        // TODO_CODE: end
        
        return resOK;        
    }
    
    result_t read(vnt::bsp::sns::PRS s, vnt::measure::TimedPressure *ptimedvalue, const vnt::os::Action &alert)
    {
        if(false == initialised(s) || (nullptr == ptimedvalue))
        {
            return resNOK;
        }    
        
        ptimedvalue->pressure = vnt::measure::pressure0;
        ptimedvalue->timestamp = vnt::core::now();
        
        // TODO_CODE: start
        // in here add the code for starting reading from the chip. and if ok we return resOK
        // and let the reading go in background. when everything is done, the IRQHandler will
        // - copy the measure into memory pointed by &(ptimedvalue->pressure)
        // - copy into memory pointed by &(ptimedvalue->timestamp) the time retrieved w/ vnt::core::now() which can
        //   be safely called inside an IRQHandler
        // - execute the callback 
        // an alternative if the IRQhandler is compiled in a .c module is to execute oncompletion.call(arg)
        // where arg is the pointer to the just read measure.

#if defined(TEST_VNT_BSP_SNS_PRESSURE)
       
        // ok, i tell what it is: we start a timer which at its expiry sends the event to the thread
        // and i already set the value
        vnt::os::Timer::Config cfg{test_delays[vnt::core::tointegral(s)], alert, vnt::os::Timer::Mode::oneshot, 0};
        test_timers[vnt::core::tointegral(s)]->start(cfg);
        ptimedvalue->timestamp = vnt::core::now();
        ptimedvalue->pressure = test_values[vnt::core::tointegral(s)];
        test_values[vnt::core::tointegral(s)] += 0.01;

#endif        
        
        // TODO_CODE: end
        
        return resOK;                
        
        
    }
    
 
}}}} // namespace vnt { namespace bsp { namespace sns { namespace pressure {

#endif


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

