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

CGEventRef handleKeyboard (CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    CGKeyCode keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    CGEventFlags flags = CGEventGetFlags(event);
    
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
        return event;
    }
    
    // remap backtick to paragraph
    if (keyCode == 0x32) { // backtick key
        CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 0xa); // paragraph
        return event;
    }
    
    return event;
}
