#ifndef VERONA_RT_C
#define VERONA_RT_C

#include <stdbool.h>

///
// (typedefs)

struct RTAlloc;
typedef struct RTAlloc RTAlloc;

struct RTObjectStack;
typedef struct RTObjectStack RTObjectStack;

struct RTObject {
  // This struct has an opaque size because it is meant to be used as padding
  // by C structs that need to act as "subclasses" of the C++ class Object.
  void* _opaque[3]; // TODO: cross-platform
};
typedef struct RTObject RTObject;

struct RTCown {
  // This struct has an opaque size because it is meant to be used as padding
  // by C structs that need to act as "subclasses" of the C++ class Cown.
  // The size of a Cown varies - it has extra fields when the runtime
  // was compiled with systematic testing features enabled.
#ifdef USE_SYSTEMATIC_TESTING
  void* _opaque[12]; // TODO: cross-platform
#else
  void* _opaque[8]; // TODO: cross-platform
#endif
};
typedef struct RTCown RTCown;

typedef void (*RTDescriptorTraceFunction)(RTObject* o, RTObjectStack* st);
typedef void (*RTDescriptorFinalFunction)(RTObject* o);
typedef void (*RTDescriptorNotifyFunction)(RTObject* o);

struct RTDescriptor {
  size_t size;
  RTDescriptorTraceFunction trace;
  RTDescriptorTraceFunction trace_possibly_iso;
  RTDescriptorFinalFunction finaliser;
  RTDescriptorNotifyFunction notified;
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

typedef void (*RTCallbackFunction)(void* arg);

///
// RTAlloc

RTAlloc* RTAlloc_get();
void* RTAlloc_alloc(RTAlloc* alloc, size_t size);

///
// RTObjectStack

bool RTObjectStack_empty(RTObjectStack* stack);
RTObject* RTObjectStack_peek(RTObjectStack* stack);
RTObject* RTObjectStack_pop(RTObjectStack* stack);
void RTObjectStack_push(RTObjectStack* stack, RTObject* obj);
// TODO: RTObjectStack_forall?

///
// RTObject

RTObject* RTObject_new_iso(RTAlloc* alloc, RTDescriptor* desc);
RTObject* RTObject_new_mut(RTAlloc* alloc, RTDescriptor* desc, RTObject* iso_root);
const RTDescriptor* RTObject_get_descriptor(RTObject* obj);
void RTObject_region_destroy(RTAlloc* alloc, RTObject* iso_root);
void RTObject_region_merge(RTAlloc* alloc, RTObject* into, RTObject* other);
void RTObject_region_swap_root(RTObject* iso_root, RTObject* new_iso_root);
void RTObject_region_freeze(RTAlloc* alloc, RTObject* iso_root);

///
// RTImmutable

void RTImmutable_acquire(RTObject* obj);
void RTImmutable_release(RTObject* obj, RTAlloc* alloc);

///
// RTCown

RTCown* RTCown_new(RTAlloc* alloc, RTDescriptor* desc);
const RTDescriptor* RTCown_get_descriptor(RTCown* cown);
// TODO: RTCown_get_epoch_mark?
// TODO: RTCown_cown_zero_rc?
// TODO: RTCown_send?
bool RTCown_can_lifo_schedule(RTCown* cown);
void RTCown_wake(RTCown* cown);
void RTCown_acquire(RTCown* cown);
void RTCown_release(RTCown* cown, RTAlloc* alloc);
void RTCown_weak_release(RTCown* cown, RTAlloc* alloc);
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

RTAction* RTAction_new(RTAlloc* alloc, RTActionDescriptor* desc);
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
void RTScheduler_run_with_startup(RTCallbackFunction f, void* arg);

///
// RTSystematic

void RTSystematic_enable_crash_logging();
void RTSystematic_log(char* message);
void RTSystematic_log_ptr(char* message, void* ptr);
void RTSystematicTestHarness_run(int argc, char** argv, RTCallbackFunction f, void* arg);

#endif
