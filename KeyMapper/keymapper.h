//
//  keymapper.h
//  KeyMapper
//
//  Created by Denys Honsiorovskyi on 29/12/2016.
//  Copyright © 2016 Denys Honsiorovskyi. All rights reserved.
//

#ifndef keymapper_h
#define keymapper_h

#include <Carbon/Carbon.h>


void emulateKeyPress(CGKeyCode keyCode, CGEventFlags downFlags, CGEventFlags upFlags) {
    CGEventRef keyDown = CGEventCreateKeyboardEvent (NULL, keyCode, true);
    CGEventRef keyUp   = CGEventCreateKeyboardEvent (NULL, keyCode, false);
    
    CGEventSetFlags(keyDown, downFlags);
    CGEventSetFlags(keyUp, upFlags);
    
    
    CGEventPost(kCGHIDEventTap, keyDown);
    CFRelease(keyDown);
    
    CGEventPost(kCGHIDEventTap, keyUp);
    CFRelease(keyUp);
}




CGEventRef eventCallback (CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    CGEventFlags flags = CGEventGetFlags(event);
    CGKeyCode keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    
    // remap option to cmd + space
    if ((flags & kCGEventFlagMaskAlternate) && (keyCode == kVK_Option)) {
        emulateKeyPress(kVK_Space, kCGEventFlagMaskCommand, kCGEventFlagMaskCommand);
        fprintf(stdout, "-- lang change\n");
        return event;
    }
    
    // remap fn + {key} to ctrl + {key} for non-functional keys
    if (
        (flags & kCGEventFlagMaskSecondaryFn)
        && !(flags & kCGEventFlagMaskNumericPad)
        && (keyCode >= 0x0 && keyCode <= 0x32) // alphanumeric
        ) {
        CGEventSetFlags(event, kCGEventFlagMaskControl);
        fprintf(stdout, "-- ctrl + letter\n");
        return event;
    }
    
    // remap paragraph to backtick
    if (keyCode == 0xa) { // paragraph key
        CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 0x32); // backtick
        fprintf(stdout, "-- backtick\n");
        return event;
    }
    
    // remap backtick to paragraph
    if (keyCode == 0x32) { // backtick key
        CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 0xa); // paragraph
        fprintf(stdout, "-- paragraph\n");
        return event;
    }
    
    
    return event;
}


void initKeyMapper()
{
    // Specify an event mask.
    CGEventMask eventMask = (
                             CGEventMaskBit(kCGEventFlagsChanged) |
                             CGEventMaskBit(kCGEventKeyDown) |
                             CGEventMaskBit(kCGEventKeyUp)
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
