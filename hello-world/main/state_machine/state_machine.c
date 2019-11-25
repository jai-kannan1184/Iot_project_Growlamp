#include "state_machine.h"
#include <assert.h>
#include <stdlib.h>
#include "states.h"

typedef struct DeviceState* DeviceStatePtr;
static DeviceStatePtr deviceState;

typedef void (*Action)(void);

static void setupFirstState(DeviceStatePtr deviceState);
static void setupSecondState(DeviceStatePtr deviceState);
static void setupThirdState(DeviceStatePtr deviceState);
static void setupFourthState(DeviceStatePtr deviceState);

struct DeviceState {
   Action action;
   void (*onButtonPress)(DeviceStatePtr);
};

static void setupFirstState(DeviceStatePtr deviceState) {
   deviceState->action = firstState;
   deviceState->onButtonPress = setupSecondState;
}

static void setupSecondState(DeviceStatePtr deviceState) {
   deviceState->action = secondState;
   deviceState->onButtonPress = setupThirdState;
}

static void setupThirdState(DeviceStatePtr deviceState) {
   deviceState->action = thirdState;
   deviceState->onButtonPress = setupFourthState;
}

static void setupFourthState(DeviceStatePtr deviceState) {
   deviceState->action = fourthState;
   deviceState->onButtonPress = setupFirstState;
}

void triggerEvent(void) {
       deviceState->onButtonPress(deviceState);
       deviceState->action();
}
void createDeviceState(void){
   deviceState=malloc(sizeof* deviceState);
   assert (deviceState);
   if(deviceState){
      setupFirstState(deviceState);
   }
}
void destroyDeviceState(void){
   free(deviceState);
   deviceState=NULL;
}