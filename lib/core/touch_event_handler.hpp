/**
 * @file touch_event_handler.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Touch handler to keep track of events and their handlers.
 * @version 1.0
 * @date 2022-07-05
 */

#pragma once
#include "../../calc.hpp"
#include <sdk/os/input.h>
#include <sdk/os/mem.h>

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

struct KeyListener {
    Input_Keycode key;
    void (*callback)();
};

// can be changed to accomodate more events if needed

struct TouchHandler touchHandlers[64];
uint32_t touchHandlersLength = 0;

struct ActBarHandler actBarHandlers[6];
uint32_t actBarHandlersLength = 0;

struct KeyListener keyListeners[32];
uint32_t keyListenersLength = 0;

struct Input_Event event __attribute__((aligned(4)));

void checkTouchEvents() {
   
   // check if there are no events to check
   if (touchHandlersLength == 0 && actBarHandlersLength == 0 && keyListenersLength == 0) {
      return;
   }

   // as have not found a way to directly check if a key is pressed, we will check the power timeout.

   // get power off time at 0x8C0A83B4
   uint32_t powerOffTime = *(uint32_t *)0x8C0A83B4;
   // if at 00 00 04 B0 then just got input (10 mins internally)
   if (powerOffTime == 0x004B0000) return;

   Mem_Memset(&event, 0, sizeof(event));
   GetInput(&event, 0xFFFFFFFF, 0x10); // polls

   switch (event.type) {
      case EVENT_KEY:
         if (keyListenersLength > 0 && event.data.key.direction == KEY_PRESSED) {
             for (uint32_t i = 0; i < keyListenersLength; i++) {
                 if (event.data.key.keyCode == keyListeners[i].key) {
                     (*keyListeners[i].callback)();
                     break;
                 }
             }
         }
         break;
         
      case EVENT_TOUCH:
         // check if there are any touch handlers
         if (touchHandlersLength > 0) {
            for (uint32_t i = 0; i < touchHandlersLength; i++) {
               if ((uint32_t)event.data.touch_single.p1_x >= touchHandlers[i].minX && (uint32_t)event.data.touch_single.p1_x <= touchHandlers[i].maxX &&
                  (uint32_t)event.data.touch_single.p1_y >= touchHandlers[i].minY && (uint32_t)event.data.touch_single.p1_y <= touchHandlers[i].maxY) {
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
            for (uint32_t i = 0; i < actBarHandlersLength; i++) {
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

void addKeyListener(Input_Keycode key, void (*callback)()) {
    KeyListener handler;
    handler.key = key;
    handler.callback = callback;
    keyListeners[keyListenersLength++] = handler;
}

void removeTouchListener(uint32_t minX, uint32_t minY, uint32_t maxX, uint32_t maxY, uint32_t direction = TOUCH_DOWN) {
   for (uint32_t i = 0; i < touchHandlersLength; i++) {
      if (touchHandlers[i].minX == minX && touchHandlers[i].minY == minY && touchHandlers[i].maxX == maxX && touchHandlers[i].maxY == maxY && touchHandlers[i].direction == direction) {
         touchHandlers[i] = touchHandlers[touchHandlersLength - 1];
         touchHandlersLength--;
      }
   }
}

void removeActBarListener(uint32_t type) {
   for (uint32_t i = 0; i < actBarHandlersLength; i++) {
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
