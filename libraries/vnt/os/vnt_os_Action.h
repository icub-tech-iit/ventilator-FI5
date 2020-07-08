
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_OS_ACTION_H_
#define _VNT_OS_ACTION_H_

#include "vnt_core.h"

#include "vnt_os.h"

#include "vnt_os_Thread.h"


namespace vnt { namespace os {
    
    struct EventToThread
    {
        os::Event event {0};
        Thread* task {nullptr};  
        
        EventToThread() = default;
        EventToThread(os::Event e, Thread* t) : event(e), task(t) {}
        bool isvalid() const;
        bool execute();
    };
    
    struct MessageToThread
    {
        os::Message message {nullptr};
        Thread* task {nullptr};
        
        MessageToThread() = default;  
        MessageToThread(os::Message m, Thread* t) : message(m), task(t) {}    
        bool isvalid() const;
        bool execute(core::relTime timeout = core::reltimeWaitForever);
    };
    
    struct CallbackToThread
    {
        core::Callback callback {nullptr, nullptr};
        Thread* task {nullptr};  

        CallbackToThread() = default;        
        CallbackToThread(core::fpCaller c, void *a, Thread *t) : callback(c, a), task(t) {}  
        CallbackToThread(core::Callback cbk, Thread *t) : callback(cbk), task(t) {}                   
        bool isvalid() const;
        bool execute(core::relTime timeout = core::reltimeWaitForever);
    };  
            
    struct Action
    {
        enum class Type { none = 0, event2thread = 1, message2thread = 2, callback2thread = 3 };
               
        Type type {Type::none};
        union
        {
            EventToThread     evt;
            MessageToThread   msg;
            CallbackToThread  cbk;
        };
               
        Action() { clear(); };
        Action(const CallbackToThread &c) { load(c); }
        Action(const MessageToThread &m) { load(m); }
        Action(const EventToThread &e) { load(e); }        
        void clear() { type = Type::none; }        
        void load(const EventToThread &e);        
        void load(const MessageToThread &m);        
        void load(const CallbackToThread &c);               
        bool isvalid() const;        
        bool execute(core::relTime timeout = core::reltimeWaitForever);
    };
    
}} // namespace vnt { namespace os {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
