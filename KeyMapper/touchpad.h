#ifndef touchpad_h
#define touchpad_h

#define kCGKindaPressureEvent 29

CGEventRef handleTouchpad(CGEventTapProxy proxy,
                             CGEventType type,
                             CGEventRef eventRef,
                             void *refcon);

#endif /* touchpad_h */
