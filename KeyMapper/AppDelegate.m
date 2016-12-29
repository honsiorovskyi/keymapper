//
//  AppDelegate.m
//  KeyMapper
//
//  Created by Denys Honsiorovskyi on 29/12/2016.
//  Copyright © 2016 Denys Honsiorovskyi. All rights reserved.
//
//  Icon made by Google from www.flaticon.com
//

#import "AppDelegate.h"
#import "keymapper.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSMenu *iconMenu;
@property (strong,nonatomic) NSStatusItem *statusIcon;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // add status icon
    _statusIcon = [[NSStatusBar systemStatusBar] statusItemWithLength:NSSquareStatusItemLength];
    _statusIcon.image = [NSImage imageNamed:@"AppIcon"];
    [_statusIcon.image setSize:NSSizeFromString(@"16x16")];
    
    _statusIcon.toolTip = @"Keys remapper";
    _statusIcon.menu = _iconMenu;
    
    // request permissions
    if (!AXIsProcessTrusted()) {
        NSAlert *alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:@"OK"];
        [alert setMessageText:@"Accessibility features disabled!"];
        [alert setInformativeText:@"This application requires accessibility permissions to run correctly. Please enable them using the following dialog and restart the application."];
        [alert setAlertStyle:NSAlertStyleWarning];
        [alert runModal];
        
        NSDictionary *options = @{(__bridge id)kAXTrustedCheckOptionPrompt: @YES};
        AXIsProcessTrustedWithOptions((CFDictionaryRef)options);

        [self quitClicked:self];
    }
    
    // run keymapper
    initKeyMapper();
}

- (IBAction) quitClicked:(id)sender {
    [NSApp terminate:self];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
}

@end
