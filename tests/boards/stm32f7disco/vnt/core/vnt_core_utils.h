
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware-shared for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_CORE_UTILS_H_
#define _VNT_CORE_UTILS_H_

#include <cstdint>
#include <cstring>


namespace vnt { namespace core { namespace utils {
    
    template<typename T>
    struct Triple
    {   // only for integral types
        T   x {0};
        T   y {0};
        T   z {0};
        Triple() = default;
        Triple(T _x, T _y, T _z) { set(_x, _y, _z); }
        void set(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }  
        void clear() { set(0, 0, 0); }
        void load(void *littleendianmemory) { T *p = reinterpret_cast<T*>(littleendianmemory); if(nullptr != p) { set(p[0], p[1], p[2]); } }
    };     
          
    template<typename T>
    struct Quadruple
    {   // only for integral types
        T   w {0};
        T   x {0};
        T   y {0};
        T   z {0};
        Quadruple() = default;
        Quadruple(T _w, T _x, T _y, T _z) { set(_w, _x, _y, _z); }
        void set(T _w, T _x, T _y, T _z) { w = _w; x = _x; y = _y; z = _z; }  
        void clear() { set(0, 0, 0, 0); }
        void load(void *littleendianmemory) { T *p = reinterpret_cast<T*>(littleendianmemory); if(nullptr != p) { set(p[0], p[1], p[2], p[3]); } }
    };  

       
    class Storage
    {
    public:
        virtual bool isInitted() = 0;
        virtual bool isAddressValid(std::uint32_t address) = 0;
        virtual std::uint32_t getBaseAddress() = 0;
        virtual std::uint32_t getSize() = 0;   
        virtual bool fullerase() = 0;  
        virtual bool erase(std::uint32_t address, std::uint32_t size) = 0;        
        virtual bool read(std::uint32_t address, std::uint32_t size, void *data) = 0;
        virtual bool write(std::uint32_t address, std::uint32_t size, const void *data) = 0;   

        virtual ~Storage() {};    
    };     
   
}}}



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
