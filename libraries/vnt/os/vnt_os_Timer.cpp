
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/


// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_os_Timer.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------


#include "vnt_os_Thread.h"
#include "vnt_os_theTimerManager.h"
#include "osal.h"

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------



namespace vnt { namespace os { namespace timertools {
        
    struct osalTMR 
    {
        Timer               *owner;
        osal_timer_t        *osaltimer;
        Timer::Config       tconfig;         
        Timer::Status       sta;             
        std::uint32_t       maxshots;
        std::uint32_t       count;        
        bool                canexecuteaction; 
        bool onemoretime;                 
        
        osalTMR(Timer *own)   
        {
            owner = own;
            reset(Timer::Status::idle);
            osaltimer = osal_timer_new();
        }
        
        ~osalTMR()   
        {
            stop();
            // do reverse deallocation and cleaning
            osal_timer_delete(osaltimer);
            osaltimer = nullptr;
            reset(Timer::Status::idle);              
            canexecuteaction = false;  
            owner = nullptr;            
        }
        
        void reset(Timer::Status st)
        {
            tconfig.clear();
            sta = st;
            canexecuteaction = false;
            onemoretime = false;
            tconfig.onexpiry.clear();
        }
        
        Timer::Status getstatus()
        {
            return sta;
        }
        
//        Timer::Mode getmode()
//        {
//            return mod;
//        }
        
        bool start(const Timer::Config &config)
        {            
            // config must be valid 
            if(false == config.isvalid())
            {
                return false;
            }
                                                    
            bool ret = false;
            
            // 1. lock the timer manager
            
            // 2. start it only if it is not counting
            if(Timer::Status::counting != getstatus())
            {
                tconfig = config;
                
                // 1. copy the onexp and disable it. it will be enabled only at expiry of the countdown so that it can be executed
                
                canexecuteaction = false;
                                
                osal_timer_timing_t timing { 
                    .startat = OSAL_abstimeNONE, 
                    .count = tconfig.countdown, 
                    .mode = ( (Timer::Mode::oneshot == tconfig.mode) ? (osal_tmrmodeONESHOT) : (osal_tmrmodeFOREVER) )
                };
                osal_timer_onexpiry_t onexpi {.cbk = OnExpiryCbk, .par = this };

                osal_result_t r = osal_timer_start(osaltimer, &timing, &onexpi, osal_callerAUTOdetect);                
                
                if(osal_res_OK == r)
                {
                    sta = Timer::Status::counting;
                    count = 0;
                    maxshots = ( (Timer::Mode::someshots == tconfig.mode) ? (tconfig.numofshots) : (0) );
                    onemoretime = false;
                    ret = true;
                }
                else
                {
                    sta  = Timer::Status::idle;
                    onemoretime =  false;
                    ret = false;
                }

            }
            
            // 5. unlock the timer manager
        
            return ret;
        }  

        // it is executed by osal inside the systick, hence ... keep it lean
        static void OnExpiryCbk(osal_timer_t *osaltmr, void *par) 
        {
            osalTMR *otm = reinterpret_cast<osalTMR*>(par);
            otm->canexecuteaction = true;
            vnt::os::theTimerManager &tm = vnt::os::theTimerManager::getInstance();
            if(true == tm.started())
            {
                tm.onexpiry(*otm->owner);
            }
            else
            {
                otm->owner->execute();
            }
            if(otm->tconfig.mode == Timer::Mode::someshots)
            {
                otm->count ++;
                if(otm->count >= otm->maxshots)
                {
                    otm->onemoretime = true;
                    otm->stop();
                }
            }
            else if(otm->tconfig.mode == Timer::Mode::oneshot)
            {
                    otm->onemoretime = true;
                    otm->stop();                
            }
            
        }  


        bool stop()
        {
            if(Timer::Status::counting == getstatus())
            {                                
                // stop the osal timer. operation is null safe.
                osal_timer_stop(osaltimer, osal_callerAUTOdetect);
            }
          
            if(false == onemoretime)
            {
                reset(Timer::Status::idle);   
                canexecuteaction = false;   
            }
            
            return true;            
        }
        
    }; 
        
    
}}} // namespace vnt { namespace os {

     
struct vnt::os::Timer::Impl
{          
    vnt::os::timertools::osalTMR *tmr;
    
    Impl(Timer *own) 
    {
        tmr = new vnt::os::timertools::osalTMR(own);
    }
    ~Impl()
    {
        delete tmr;
    }
};


// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------



vnt::os::Timer::Timer()
: pImpl(new Impl(this))
{ 

}

vnt::os::Timer::~Timer()
{   
    delete pImpl;
}

bool vnt::os::Timer::start(const Config &config)
{
    return pImpl->tmr->start(config);
}

bool vnt::os::Timer::stop()
{
    return pImpl->tmr->stop();
}

vnt::os::Timer::Status vnt::os::Timer::getStatus() const
{
    return pImpl->tmr->getstatus();   
}

//vnt::os::Timer::Mode vnt::os::Timer::getMode() const
//{
//    return pImpl->tmr->getmode();  
//}

//vnt::core::relTime vnt::os::Timer::getCountdown() const
//{
//    return pImpl->tmr->period;
//}

//const vnt::os::Action& vnt::os::Timer::getAction() const
//{
//    return pImpl->tmr->tconfig.onexpiry;
//}

const vnt::os::Timer::Config& vnt::os::Timer::getConfig() const
{
    return pImpl->tmr->tconfig;
}

bool vnt::os::Timer::execute()
{
    bool canexecute = pImpl->tmr->canexecuteaction;
    if(true == canexecute)
    {
        pImpl->tmr->tconfig.onexpiry.execute();  
        pImpl->tmr->canexecuteaction = false;   
        if(true == pImpl->tmr->onemoretime)
        {
            pImpl->tmr->onemoretime = false;
            pImpl->tmr->reset(Timer::Status::idle);   
        }
    }
    return canexecute;
}



// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

