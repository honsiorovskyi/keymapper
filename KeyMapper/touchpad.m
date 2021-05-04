//
// Huge thank you to https://github.com/Kolsha for this code.
// Original: https://github.com/Kolsha/Force-Command-Click
// 

#include <Carbon/Carbon.h>
#include <Cocoa/Cocoa.h>
#include "touchpad.h"

bool needIgnoreNextLeftMouseUp = false;

CGEventRef handleTouchpad(CGEventTapProxy proxy, CGEventType type, CGEventRef eventRef, void *refcon) {
    @autoreleasepool {
        if(needIgnoreNextLeftMouseUp &&
           (type == kCGEventLeftMouseUp || type == kCGEventLeftMouseDown)){
            return (NULL);
        }
        
        if ((type != kCGKindaPressureEvent)){
            return eventRef;
        }
        
        NSEvent *event = [NSEvent eventWithCGEvent:eventRef];

        if(needIgnoreNextLeftMouseUp && event.stage != 0){
            return (NULL);
        }
        
        if(needIgnoreNextLeftMouseUp){
            needIgnoreNextLeftMouseUp = false;
            return (NULL);
        }
        
        if (event.type == NSEventTypePressure && event.stage == 2){
            if(event.pressure > 0.000){
                return (NULL);
            }
            //NSLog(@"Deep click");
            
            CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
            CGPoint mouse_pos = CGEventGetLocation(eventRef);
            
            CGEventRef click_down = CGEventCreateMouseEvent(
                                                             src, kCGEventLeftMouseDown,
                                                             mouse_pos,
                                                             kCGMouseButtonLeft
                                                             );
            
            CGEventRef click_up = CGEventCreateMouseEvent(
                                                           src, kCGEventLeftMouseUp,
                                                           mouse_pos,
                                                           kCGMouseButtonLeft
                                                           );
            
            CGEventSetFlags(click_down, kCGEventFlagMaskCommand);
            CGEventSetFlags(click_up, kCGEventFlagMaskCommand);

            CGEventPost(kCGHIDEventTap, click_down);
            
            needIgnoreNextLeftMouseUp = true;

            CFRelease(src);
            CFRelease(click_down);

            return click_up;
        }
        
        return eventRef;
    }
}
