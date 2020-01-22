#ifndef VERONA_RT_C
#define VERONA_RT_C

#include <stdbool.h>

///
// (typedefs)

struct RTAlloc;
typedef struct RTAlloc RTAlloc;

struct RTObjectStack;
typedef struct RTObjectStack RTObjectStack;

struct RTObject;
typedef struct RTObject RTObject;

struct RTCown;
typedef struct RTCown RTCown;

typedef void (*RTDescriptorTraceFunction)(RTObject* o, RTObjectStack* st);
typedef void (*RTDescriptorTraceIsoFunction)(RTObject* o, RTObjectStack* st);
typedef void (*RTDescriptorTraceFinalFunction)(RTObject* o);
typedef void (*RTDescriptorTraceNotifiedFunction)(RTObject* o);

struct RTDescriptor {
  size_t size;
  RTDescriptorTraceFunction trace;
  RTDescriptorTraceIsoFunction trace_possibly_iso;
  RTDescriptorTraceFinalFunction finaliser;
  RTDescriptorTraceNotifiedFunction notified;
};
typedef struct RTDescriptor RTDescriptor;

struct RTAction;
typedef struct RTAction RTAction;

typedef void (*RTActionFunction)(RTAction* a);
typedef void (*RTActionTraceFunction)(const RTAction* a, RTObjectStack* st);

struct RTActionDescriptor {
  size_t size;
  RTActionFunction f;
  RTActionTraceFunction trace;
};
typedef struct RTActionDescriptor RTActionDescriptor;

typedef void (*RTSchedulerStartupFunction)(void* arg);

///
// RTObjectStack

bool RTObjectStack_empty(RTObjectStack* stack);
RTObject* RTObjectStack_peek(RTObjectStack* stack);
RTObject* RTObjectStack_pop(RTObjectStack* stack);
void RTObjectStack_push(RTObjectStack* stack, RTObject* obj);
// TODO: RTObjectStack_forall?

///
// RTObject

const RTDescriptor* RTObject_get_descriptor(RTObject* obj);
// TODO: RTObject_get_epoch_mark?
// TODO: RTObject_cown_zero_rc?

///
// RTCown

RTCown* RTCown_alloc(RTDescriptor* desc);
const RTDescriptor* RTCown_get_descriptor(RTCown* cown);
// TODO: RTCown_get_epoch_mark?
// TODO: RTCown_cown_zero_rc?
// TODO: RTCown_send?
bool RTCown_can_lifo_schedule(RTCown* cown);
void RTCown_wake(RTCown* cown);
void RTCown_acquire(RTCown* cown);
void RTCown_release(RTCown* cown);
void RTCown_weak_release(RTCown* cown);
void RTCown_weak_acquire(RTCown* cown);
bool RTCown_acquire_strong_from_weak(RTCown* cown);
// TODO: RTCown_mark_for_scan?
// TODO: RTCown_mark_notify?
// TODO: RTCown_init?
// TODO: RTCown_run?
// TODO: RTCown_try_collect?
// TODO: RTCown_is_live?
// TODO: RTCown_collect?

///
// RTAction

RTAction* RTAction_new(RTActionDescriptor* desc);
void RTAction_schedule(RTAction* action, RTCown** cowns, size_t count);

///
// RTScheduler

void RTScheduler_set_detect_leaks(bool b);
bool RTScheduler_get_detect_leaks();
void RTScheduler_set_allow_teardown(bool allow);
void RTScheduler_set_fair(bool fair);
bool RTScheduler_is_teardown_in_progress();
// TODO: RTScheduler_local?
// TODO: RTScheduler_round_robin?
// TODO: RTScheduler_epoch?
// TODO: RTScheduler_alloc_epoch?
// TODO: RTScheduler_should_scan?
// TODO: RTScheduler_in_prescan?
void RTScheduler_want_ld();
void RTScheduler_init(size_t cores);
void RTScheduler_run();
void RTScheduler_run_with_startup(RTSchedulerStartupFunction f, void* arg);

///
// RTSystematic

void RTSystematic_enable_crash_logging();
void RTSystematic_log(char* message);
void RTSystematic_log_ptr(char* message, void* ptr);

#endif
