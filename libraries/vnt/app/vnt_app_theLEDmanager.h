
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the vnt library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_APP_THELEDMANAGER_H_
#define _VNT_APP_THELEDMANAGER_H_

#include "vnt_core.h"
#include "vnt_bsp_led.h"

#include <initializer_list>
#include <memory>

#include "vnt_core_binary.h"

#include <bitset>

namespace vnt { namespace app {
    
    struct LEDwave
    {   // LEDwave describes a sequence of on/off values of a LED across time
        // length() tells the number of elements, each one of duration frequency() micro-seconds. 
        // with tick() we advance the wave by one element and retrieve the value of the LED (true = on)
        // duplicate() is used to copy the derived object into a pointer to the interface. if external memory is provided duplicate() does not allocate ram
        virtual size_t length() const = 0;  
        virtual vnt::core::relTime frequency() const = 0;
        virtual bool tick() = 0;
        virtual LEDwave* duplicate(void* extmem, size_t sizeofextmem) const = 0;
        virtual ~LEDwave() {};        
    };
    

    // safe but does not copy        
    class LEDhandle 
    {
    public:
        // interface
        virtual bool supported() const = 0;
        virtual vnt::bsp::LED id() const = 0;
        virtual void on() = 0;
        virtual void off() = 0;
        virtual void toggle() = 0; 
        virtual void pulse(vnt::core::relTime period, std::uint32_t times = 0) = 0;
        virtual void stop() = 0; 
        virtual void wave(const LEDwave *lw, std::uint32_t times = 0) = 0;
        
    protected:
        virtual ~LEDhandle() {};    // cannot delete from interface but only from derived object
    };
    
    class theLEDmanager 
    {
    public:
    
        static theLEDmanager& getInstance();
               
        bool init(const std::initializer_list<vnt::bsp::LED> &leds);


        bool initialised(vnt::bsp::LED led) const;    
        LEDhandle & get(vnt::bsp::LED led) const;
                
    public:   
        ~theLEDmanager();

    private:
        theLEDmanager();  

    public:
        // even if i could allow a deinit, i prefer not adding it because i cannot find a way to disable the handle return by get()
        //bool deinit(const std::initializer_list<vnt::bsp::LED> &leds);
        //bool deinit(); 
    
    private:    
        struct Impl;
        std::unique_ptr<Impl> pImpl;    
    };  
    


    template<uint16_t C>
    struct LEDwaveC : LEDwave
    {
        static constexpr std::uint8_t NB = 32;
        static constexpr uint16_t capacity = C;       
        static constexpr uint16_t NWORDS = (0==C) ? (0) : ((C-1)/NB+1);
         
        vnt::core::relTime  _freq;       
        uint16_t                _len;
        uint16_t                _cnt;
        std::uint32_t           _mask[NWORDS];
        
        LEDwaveC() : _freq(100*vnt::core::time1millisec), _len(7), _cnt(0) { memset(_mask, 0, sizeof(_mask)); _mask[0] = 0b00110011;  }
                
        LEDwaveC(const vnt::core::relTime t, const uint16_t l, const void* m = nullptr, const uint16_t c = 0) : _freq(t), _len(l), _cnt(c) 
        { 
            if(0 == _len) { _len = 1; }
            
            memset(_mask, 0, sizeof(_mask));
            if(nullptr != m)
            {
                memcpy(_mask, m, sizeof(_mask)); 
            }
        }
        
        LEDwaveC(const vnt::core::relTime t, const uint16_t l, const std::initializer_list<bool> &on, const uint16_t c = 0) : _freq(t), _len(l), _cnt(c)
        { 
            if(0 == _len) { _len = 1; }
            memset(_mask, 0, sizeof(_mask));
            
            size_t siz = on.size();
            if(siz > capacity)
            {
                siz = capacity;
            }
            auto a = on.begin();
            for(uint16_t i=0; i<siz; i++)
            {                
                if(*a++)
                {
                    uint32_t *m = &_mask[i/NB];
                    vnt::core::binary::bit::set(*m, i%NB);   
                }                    
            }
        }
        
        void load(const uint16_t atpos, const void* m,  const uint16_t s)
        {
            if((nullptr != m) && (s > 0) && (atpos < _len) && ((s+atpos) < _len))
            {
                const uint8_t *from = static_cast<const uint8_t*>(m);
                for(uint16_t i=0; i<s; i++)
                {
                    bool on = vnt::core::binary::bit::check(from[i/8], i%8);
                    uint16_t maskpos = i+atpos;
                    if(on)
                        vnt::core::binary::bit::set(_mask[maskpos/NB], maskpos%NB);  
                    else
                        vnt::core::binary::bit::clear(_mask[maskpos/NB], maskpos%NB);                    
                }                               
            }            
        }
        
        LEDwaveC(const vnt::core::relTime t, const uint16_t l, const std::bitset<C> &on, const uint16_t c = 0) : _freq(t), _len(l), _cnt(c)
        { 
            if(0 == _len) { _len = 1; }
            memset(_mask, 0, sizeof(_mask));
            
            size_t siz = on.size();
            if(siz > capacity)
            {
                siz = capacity;
            }
            for(uint16_t i=0; i<siz; i++)
            {                
                if(true == on[i])
                {
                    uint32_t *m = &_mask[i/NB];
                    vnt::core::binary::bit::set(*m, i%NB);   
                }                    
            }
        }
                
       
        virtual vnt::core::relTime frequency() const override { return _freq; }
        
        virtual size_t length() const override { return _len; }
        
        virtual bool tick() override
        { 
            if(0 == _len)
            {
                return false;
            }
            uint16_t n = _cnt;
            _cnt = (_cnt+1)%_len;
            const uint32_t *msk = &_mask[n/NB];
            return vnt::core::binary::bit::check(*msk, n%NB);           
        }
                
        LEDwave* duplicate(void* mem, size_t sizeofmem) const override
        {
            LEDwaveC<C> *p = nullptr;
            if((nullptr != mem) && (sizeofmem >= sizeof(LEDwaveC<C>)))
            {
                p = new(mem) LEDwaveC<C>(_freq, _len, _mask, _cnt);
            }
            else
            {
                p = new LEDwaveC<C>(_freq, _len, _mask, _cnt); 
            }
            return p; 
        }
        
        ~LEDwaveC() override
        {
            memset(_mask, 0, sizeof(_mask));
            _len = 0;
            _freq = 0;
            _cnt = 0;
        }
    };
    
    
    template<uint16_t C>
    struct LEDwaveT : LEDwave
    {                       
        vnt::core::relTime  _freq;       
        uint16_t                _len;
        uint16_t                _cnt;
        std::bitset<C>          _bitset;
        
        //static constexpr uint16_t capacity = C;
        //static constexpr size_t sizeofLEDwaveT = sizeof(void*) + sizeof(_freq) + sizeof(_len) + sizeof(_cnt) + sizeof(_bitset); // = 12 + C
        
        LEDwaveT() : _freq(100*vnt::core::time1millisec), _len(10), _cnt(0), _bitset(0b00110011) { }
                
        LEDwaveT(const vnt::core::relTime t, const uint16_t l, std::bitset<C> b, const uint16_t c = 0) : _freq(t), _len(l), _bitset(b), _cnt(c) 
        { 
            if(0 == _len) { _len = 1; }
        }
                
        void load(const uint16_t atpos, std::bitset<C> b,  const uint16_t s)
        {
            if((atpos < _len) && ((s+atpos) < _len))
            {
                for(uint16_t i=0; i<s; i++)
                {
                    _bitset[atpos+i] = b[i];                 
                }                               
            }            
        }
                       
       
        virtual vnt::core::relTime frequency() const override { return _freq; }
        
        virtual size_t length() const override { return _len; }
        
        virtual bool tick() override
        { 
            if(0 == _len)
            {
                return false;
            }
            uint16_t n = _cnt%_len;
            _cnt = (_cnt+1)%_len;
            return _bitset[n];           
        }
                
        LEDwave* duplicate(void* mem, size_t sizeofmem) const override
        {
            LEDwaveT<C> *p = nullptr;
            if((nullptr != mem) && (sizeofmem >= sizeof(LEDwaveT<C>)))
            {
                p = new(mem) LEDwaveT<C>(_freq, _len, _bitset, _cnt);
            }
            else
            {
                p = new LEDwaveT<C>(_freq, _len, _bitset, _cnt); 
            }
            return p; 
        }
        
        ~LEDwaveT() override
        {
            _bitset.reset();
            _len = 0;
            _freq = 0;
            _cnt = 0;
        }
    };    
    

}} // namespace vnt { namespace app {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
