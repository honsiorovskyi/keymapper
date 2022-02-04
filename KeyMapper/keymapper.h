#ifndef keymapper_h
#define keymapper_h

#include <Carbon/Carbon.h>
#include "keys.h"
#include "touchpad.h"



CGEventRef eventCallback (CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    switch (type) {
        case kCGEventKeyDown:
        case kCGEventKeyUp:
            return handleKeyboard(proxy, type, event, refcon);
            
//        case kCGKindaPressureEvent:
//        case kCGEventLeftMouseUp:
//        case kCGEventLeftMouseDown:
//            return handleTouchpad(proxy, type, event, refcon);
    }
    
    return event;
}

void initKeyMapper()
{
    // Specify an event mask.
    CGEventMask eventMask = (
                             CGEventMaskBit(kCGEventKeyDown) |
                             CGEventMaskBit(kCGEventKeyUp) |
                             CGEventMaskBit(kCGKindaPressureEvent) |
                             CGEventMaskBit(kCGEventLeftMouseUp) |
                             CGEventMaskBit(kCGEventLeftMouseDown)
                             );
    
    // Create an event tap.
    CFMachPortRef eventTap = CGEventTapCreate(
                                              kCGHIDEventTap,
                                              kCGHeadInsertEventTap,
                                              kCGEventTapOptionDefault,
                                              eventMask,
                                              &eventCallback,
                                              NULL
                                              );
    
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
        exit(1);
    }
    
    // Create a run loop source.
    CFRunLoopSourceRef runLoopSourceRef = CFMachPortCreateRunLoopSource(NULL, eventTap, 0);
    CFRelease(eventTap);
    
    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSourceRef, kCFRunLoopDefaultMode);
    CFRelease(runLoopSourceRef);
    
    // Enable the event tap.
    CGEventTapEnable(eventTap, true);
    
    // Set it all running.
    CFRunLoopRun();
}

#endif /* keymapper_h */
