/**
 * @file touch_event_handler.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Touch handler to keep track of events and their handlers.
 * @version 1.0
 * @date 2022-07-05
 */

#pragma once
#include "../../calc.hpp"
#include <sdk/os/input.hpp>

struct TouchHandler {
   uint32_t minX;
   uint32_t minY;
   uint32_t maxX;
   uint32_t maxY;
   uint32_t direction;
   void (*callback)();
};

struct ActBarHandler {
   uint16_t type;
   void (*callback)();
};

// can be changed to accomodate more events if needed

struct TouchHandler touchHandlers[64];
uint8_t touchHandlersLength = 0;

struct ActBarHandler actBarHandlers[6];
uint8_t actBarHandlersLength = 0;

struct InputEvent event;

void checkTouchEvents() {
   
   // check if there are no events to check
   if (touchHandlersLength == 0 && actBarHandlersLength == 0) {
      return;
   }

   // as have not found a way to directly check if a key is pressed, we will check the power timeout.

   // get power off time at 0x8C0A83B4
   uint32_t powerOffTime = *(uint32_t *)0x8C0A83B4;
   // if at 00 00 04 B0 then just got input (10 mins internally)
   if (powerOffTime == 0x004B0000) return;

   memset(&event, 0, sizeof(event));
   GetInput(&event, 0xFFFFFFFF, 0x10); // polls

   switch (event.type) {
      case EVENT_KEY:
         // for some reason, this is really slow with certain keys, so kept old event handler
         break;
         
      case EVENT_TOUCH:
         // check if there are any touch handlers
         if (touchHandlersLength > 0) {
            for (uint8_t i = 0; i < touchHandlersLength; i++) {
               if (event.data.touch_single.p1_x >= touchHandlers[i].minX && event.data.touch_single.p1_x <= touchHandlers[i].maxX &&
                  event.data.touch_single.p1_y >= touchHandlers[i].minY && event.data.touch_single.p1_y <= touchHandlers[i].maxY) {
                  // check direction
                  if (event.data.touch_single.direction == touchHandlers[i].direction) {
                     (*touchHandlers[i].callback)();
                     break;
                  }
               }
            }
         }
         break;

      case EVENT_ACTBAR_RESIZE:
      case EVENT_ACTBAR_SWAP:
      case EVENT_ACTBAR_ROTATE:
      case EVENT_ACTBAR_ESC:
      case EVENT_ACTBAR_SETTINGS:
         // check if there are any act bar handlers
         if (actBarHandlersLength > 0) {
            for (uint8_t i = 0; i < actBarHandlersLength; i++) {
               if (event.type == actBarHandlers[i].type) {
                  (*actBarHandlers[i].callback)();
                  break;
               }
            }
         }
         break;
      default:
         break;
   }
}

void addTouchListener(uint32_t minX, uint32_t minY, uint32_t maxX, uint32_t maxY, void (*callback)(), uint32_t direction = TOUCH_DOWN) {
   TouchHandler handler;
   handler.minX = minX;
   handler.minY = minY;
   handler.maxX = maxX;
   handler.maxY = maxY;
   handler.direction = direction;
   handler.callback = callback;
   touchHandlers[touchHandlersLength++] = handler;
}

void addActBarListener(uint16_t type, void (*callback)()) {
   ActBarHandler handler;
   handler.type = type;
   handler.callback = callback;
   actBarHandlers[actBarHandlersLength++] = handler;
}

void removeTouchListener(uint32_t minX, uint32_t minY, uint32_t maxX, uint32_t maxY, uint32_t direction = TOUCH_DOWN) {
   for (uint8_t i = 0; i < touchHandlersLength; i++) {
      if (touchHandlers[i].minX == minX && touchHandlers[i].minY == minY && touchHandlers[i].maxX == maxX && touchHandlers[i].maxY == maxY && touchHandlers[i].direction == direction) {
         touchHandlers[i] = touchHandlers[touchHandlersLength - 1];
         touchHandlersLength--;
      }
   }
}

void removeActBarListener(uint16_t type) {
   for (uint8_t i = 0; i < actBarHandlersLength; i++) {
      if (actBarHandlers[i].type == type) {
         actBarHandlers[i] = actBarHandlers[actBarHandlersLength - 1];
         actBarHandlersLength--;
      }
   }
}

void removeAllTouchListeners() {
   touchHandlersLength = 0;
}

void removeAllActBarListeners() {
   actBarHandlersLength = 0;
}
