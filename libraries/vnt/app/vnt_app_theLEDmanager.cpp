 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the vnt library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/


// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_app_theLEDmanager.h"

// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stm32hal.h"
#include "vnt_bsp.h"
#include "vnt_core_binary.h"
#include "vnt_os_Timer.h"

#include "vnt_os_theCallbackManager.h"
#include "vnt_os_Timer.h"
#include "vnt_os_Action.h"

#include <vector>
#include <map>

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

namespace vnt { namespace app {
    
    using namespace vnt::bsp;
    
//    GPIO_PinState convert(vnt::bsp::gpio::State s)
//    {
//        return (s == vnt::bsp::gpio::State::RESET) ? (GPIO_PIN_RESET) : (GPIO_PIN_SET);
//    }
    

    // use this class if you know for sure that the led is supported. please check with a call to vnt::hw::bsp::led::getMAP()->supported(led) first
    class aSlimPulsableLED : public LEDhandle
    {
    public:
        LED led;
        //const vnt::bsp::GPIO *gpio;
        //vnt::bsp::GPIO::State stateON;
        //vnt::bsp::GPIO::State stateOFF;
        vnt::os::Timer *tmr;
        LEDwave *_ledw;
        uint64_t _memory[4];
    public:
        
//        aSlimPulsableLED(LED l = LED::none, const vnt::bsp::GPIO *g = nullptr, 
//                        vnt::bsp::GPIO::State ON = vnt::bsp::GPIO::State::SET, 
//                        vnt::bsp::GPIO::State OFF = vnt::bsp::GPIO::State::RESET) 
//                        : led(l), gpio(g), stateON(ON), stateOFF(OFF)
//        {
//            // assume led is ok.
//            // i defer creation of the timer.
//            tmr = nullptr;   
//            _ledw = nullptr;
//            std::memset(_memory, 0xff, sizeof(_memory));
//        };
        aSlimPulsableLED(LED l = LED::none) 
                        : led(l)
        {
            // assume led is ok.
            // i defer creation of the timer.
            tmr = nullptr;   
            _ledw = nullptr;
            std::memset(_memory, 0xff, sizeof(_memory));
        };
            
        ~aSlimPulsableLED() {};
            
        bool init()
        {
            return (vnt::bsp::result_t::OK == vnt::bsp::led::init(led));
        }            
            
        virtual bool supported() const
        {   // assume it is ok but i give correct result anyway
            return vnt::bsp::led::supported(led);
        }
        
        virtual LED id() const
        {
            return led;
        }
        
        virtual void on() 
        {   // assume it is ok, hence gpio is not null
            //HAL_GPIO_WritePin(static_cast<GPIO_TypeDef*>(gpio->port), static_cast<uint16_t>(gpio->pin), stateON);
            //vnt::bsp::::set(*gpio, stateON);
            vnt::bsp::led::on(led);
        }
        
        virtual void off() 
        {   // assume it is ok, hence gpio is not null
            //HAL_GPIO_WritePin(static_cast<GPIO_TypeDef*>(gpio->port), static_cast<uint16_t>(gpio->pin), stateOFF);
            //vnt::bsp::gpio::set(*gpio, stateOFF);
            vnt::bsp::led::off(led);
        }
        
        virtual void toggle() 
        {   // assume it is ok, hence gpio is not null
            //HAL_GPIO_TogglePin(static_cast<GPIO_TypeDef*>(gpio->port), static_cast<uint16_t>(gpio->pin));
            //vnt::bsp::gpio::toggle(*gpio);
            vnt::bsp::led::toggle(led);
        }   

        static void onexpirypulse(void *p)
        {
            aSlimPulsableLED * l = reinterpret_cast<aSlimPulsableLED*>(p);
            l->toggle();            
        }
        
        virtual void pulse(vnt::core::relTime period, std::uint32_t times) 
        {            
            if(nullptr == tmr)
            {
                tmr = new vnt::os::Timer;
            }
            
            if(0 == period)
            {
                tmr->stop();
                off();
            }
            else
            {
                tmr->stop();
                off();
                vnt::os::Timer::Mode mode = (0 == times) ? (vnt::os::Timer::Mode::forever) : (vnt::os::Timer::Mode::someshots);
                vnt::os::Action act(vnt::os::CallbackToThread(onexpirypulse, this, vnt::os::theCallbackManager::getInstance().thread()));
                vnt::os::Timer::Config cfg(period/2, act, mode, times);
                tmr->start(cfg);
            }
        }

        
        virtual void stop()
        {
            if(nullptr != tmr)
            {
                tmr->stop();
                off();
            }
            
            if(nullptr != _ledw)
            {
                _ledw->~LEDwave();
                _ledw = nullptr;
                std::memset(_memory, 0xff, sizeof(_memory));
            }
        }
        
                        
        static void onexpirywave(void *p)
        {
            aSlimPulsableLED * l = reinterpret_cast<aSlimPulsableLED*>(p);
            bool ledon = l->_ledw->tick();
            
            if(ledon)
            {
                l->on();
            }
            else
            {
                l->off();
            }
        }
        
        virtual void wave(const LEDwave *lw, std::uint32_t times = 0)
        {
            if(nullptr == tmr)
            {
                tmr = new vnt::os::Timer;
            }
            
            if(0 == lw->frequency())
            {
                stop();
            }
            else
            {
                stop();
                _ledw = lw->duplicate(_memory, sizeof(_memory));
                vnt::os::Timer::Mode mode = (0 == times) ? (vnt::os::Timer::Mode::forever) : (vnt::os::Timer::Mode::someshots);
                vnt::os::Action act(vnt::os::CallbackToThread(onexpirywave, this, vnt::os::theCallbackManager::getInstance().thread()));
                vnt::os::Timer::Config cfg(_ledw->frequency(), act, mode, times*_ledw->length());
                tmr->start(cfg);
            }              
            
        }
        
        
    };    
    
    
    class aDummyLED : public LEDhandle
    {
    public:
        LED led;
    public:
        
        aDummyLED(LED le = LED::none) : led(le) {};
            
        ~aDummyLED() {};
            
        virtual bool supported() const
        {
            return false;
        }
        
        virtual LED id() const
        {
            return led;
        }
        
        virtual void on() {}        
        virtual void off() {}        
        virtual void toggle() {}  
        virtual void pulse(vnt::core::relTime period, std::uint32_t times) {}  
        virtual void stop() {}
        virtual void wave(const LEDwave *lw, std::uint32_t times = 0) {}
    };
    
}}

namespace vnt { namespace app {
    
    struct theLEDmanager::Impl
    {    
        bool started;
                
        std::uint32_t initialisedmask = 0;
        
        std::map<LED, aSlimPulsableLED> mapofleds;
        aDummyLED lednone{LED::none};
                     
        Impl() 
        {              
            started = false;
            initialisedmask = 0;
        }
        
        ~Impl() 
        {              
            started = false;
            initialisedmask = 0;
        }
        
        
        bool initialised(LED led) const
        {
            return vnt::core::binary::bit::check(initialisedmask, vnt::core::tointegral(led));
        }
           

        bool init(LED led)
        {
            if(!vnt::bsp::led::supported(led))
            {
                return false;
            }  
                        
            if(initialised(led))
            {   // dont need to re-init
                return true;
            }
            
            // every led initalisation is done in the bsp ...
            
            if(!vnt::bsp::initialised())
            {   // requires vnt::hw::bsp::init()
                return false;
            }
                    
            vnt::core::binary::bit::set(initialisedmask, vnt::core::tointegral(led));
                        
            //aSlimPulsableLED a{led, nullptr, vnt::bsp::GPIO::State::SET, vnt::bsp::GPIO::State::RESET};
            aSlimPulsableLED a{led};
            a.init();
            a.off();            
            mapofleds.insert(std::pair<LED, aSlimPulsableLED>(led, a));
                       
            return true;        
        }
        
        bool deinit()
        {           
            initialisedmask = 0;
            mapofleds.clear();                                   
            return true;        
        }
        
        bool deinit(LED led)
        { 
            if(initialised(led))
            {
                mapofleds.erase(led);            
                vnt::core::binary::bit::clear(initialisedmask, vnt::core::tointegral(led));
            }
            return true;
        }

    };
    
    theLEDmanager& theLEDmanager::getInstance()
    {
        static theLEDmanager* p = new theLEDmanager();
        return *p;
    }
    
    theLEDmanager::theLEDmanager()
    {
        pImpl = std::make_unique<Impl>();
    }  
    
    
    theLEDmanager::~theLEDmanager() { }
   

    bool theLEDmanager::init(const std::initializer_list<vnt::bsp::LED> &leds)
    {
        for(auto a : leds)
        {
            pImpl->init(a);
        }
        
        return true;
    } 

//    bool theLEDmanager::deinit()
//    {
//        pImpl->deinit();
//        return true;
//    }  

//    bool theLEDmanager::deinit(const std::initializer_list<LED> &leds)
//    {
//        for(auto a : leds)
//        {
//            pImpl->deinit(a);
//        }
//        return true;
//    }     

    bool theLEDmanager::initialised(LED led) const
    {
        return pImpl->initialised(led);     
    }    
    
    LEDhandle & theLEDmanager::get(LED led) const
    {
        auto a = pImpl->mapofleds.find(led);
        if(a != pImpl->mapofleds.end())
        {
            return a->second;
        }
        else
        {
            return pImpl->lednone;
        }        
    }
    
}} // namespace vnt { namespace app {

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


