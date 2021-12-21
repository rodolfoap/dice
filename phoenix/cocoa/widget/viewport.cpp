@implementation CocoaViewport : NSView

-(id) initWith:(phoenix::Viewport&)viewportReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    viewport = &viewportReference;
  }
  return self;
}

-(void) drawRect:(NSRect)rect {
  [[NSColor blackColor] setFill];
  NSRectFillUsingOperation(rect, NSCompositeSourceOver);
}

-(BOOL) acceptsFirstResponder {
  return YES;
}

-(NSDragOperation) draggingEntered:(id<NSDraggingInfo>)sender {
  NSPasteboard* pboard = [sender draggingPasteboard];
  if([[pboard types] containsObject:NSFilenamesPboardType]) {
    if([sender draggingSourceOperationMask] & NSDragOperationGeneric) {
      return NSDragOperationGeneric;
    }
  }
  return NSDragOperationNone;
}

-(BOOL) performDragOperation:(id<NSDraggingInfo>)sender {
  NSPasteboard* pboard = [sender draggingPasteboard];
  if([[pboard types] containsObject:NSFilenamesPboardType]) {
    NSArray* files = [pboard propertyListForType:NSFilenamesPboardType];
    lstring result;
    for(unsigned n = 0; n < [files count]; n++) {
      string path = [[files objectAtIndex:n] UTF8String];
      if(directory::exists(path) && !path.endswith("/")) path.append("/");
      result.append(path);
    }
    if(viewport->onDrop) viewport->onDrop(result);
    return YES;
  }
  return NO;
}

-(void) keyDown:(NSEvent*)event {
}

-(void) keyUp:(NSEvent*)event {
}

@end

namespace phoenix {

uintptr_t pViewport::handle() {
  return (uintptr_t)cocoaViewport;
}

void pViewport::setDroppable(bool droppable) {
  @autoreleasepool {
    if(droppable) {
      [cocoaViewport registerForDraggedTypes:[NSArray arrayWithObject:NSFilenamesPboardType]];
    } else {
      [cocoaViewport unregisterDraggedTypes];
    }
  }
}

void pViewport::constructor() {
  @autoreleasepool {
    cocoaView = cocoaViewport = [[CocoaViewport alloc] initWith:viewport];
  }
}

void pViewport::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
