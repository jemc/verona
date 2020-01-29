#include <verona.h>
#include "test/harness.h"
using namespace verona;

extern "C" {
  #include "verona_rt_c.h"
}

///
// RTAlloc

RTAlloc* RTAlloc_get() {
  return reinterpret_cast<RTAlloc*>(rt::ThreadAlloc::get());
}

void* RTAlloc_alloc(RTAlloc* alloc, size_t size) {
  return reinterpret_cast<rt::Alloc*>(alloc)->alloc(size);
}

///
// RTObjectStack

bool RTObjectStack_empty(RTObjectStack* stack) {
  return reinterpret_cast<rt::ObjectStack*>(stack)->empty();
}

RTObject* RTObjectStack_peek(RTObjectStack* stack) {
  return reinterpret_cast<RTObject*>(
    reinterpret_cast<rt::ObjectStack*>(stack)->peek()
  );
}

RTObject* RTObjectStack_pop(RTObjectStack* stack) {
  return reinterpret_cast<RTObject*>(
    reinterpret_cast<rt::ObjectStack*>(stack)->pop()
  );
}

void RTObjectStack_push(RTObjectStack* stack, RTObject* obj) {
  reinterpret_cast<rt::ObjectStack*>(stack)->push(
    reinterpret_cast<rt::Object*>(obj)
  );
}

///
// RTObject

const RTDescriptor* RTObject_get_descriptor(RTObject* obj) {
  return reinterpret_cast<const RTDescriptor*>(
    reinterpret_cast<rt::Object*>(obj)->get_descriptor()
  );
}

///
// RTCown

RTCown* RTCown_new(RTAlloc* alloc, RTDescriptor* desc) {
  static_assert(sizeof(RTCown) == sizeof(rt::Cown),
    "Expected the opaque RTCown to be the same size as the actual rt::Cown");

  rt::Cown* cown = rt::Cown::alloc(
    reinterpret_cast<rt::Alloc*>(alloc),
    reinterpret_cast<rt::Descriptor*>(desc),
    rt::Scheduler::alloc_epoch()
  );
  new (cown) rt::Cown;
  return reinterpret_cast<RTCown*>(cown);
}

const RTDescriptor* RTCown_get_descriptor(RTCown* cown) {
  return reinterpret_cast<const RTDescriptor*>(
    reinterpret_cast<rt::Cown*>(cown)->get_descriptor()
  );
}

bool RTCown_can_lifo_schedule(RTCown* cown) {
  return reinterpret_cast<rt::Cown*>(cown)->can_lifo_schedule();
}

void RTCown_wake(RTCown* cown) {
  reinterpret_cast<rt::Cown*>(cown)->wake();
}

void RTCown_acquire(RTCown* cown) {
  rt::Cown::acquire(reinterpret_cast<rt::Cown*>(cown));
}

void RTCown_release(RTCown* cown, RTAlloc* alloc) {
  rt::Cown::release(
    reinterpret_cast<rt::Alloc*>(alloc),
    reinterpret_cast<rt::Cown*>(cown)
  );
}

void RTCown_weak_release(RTCown* cown, RTAlloc* alloc) {
  reinterpret_cast<rt::Cown*>(cown)->weak_release(
    reinterpret_cast<rt::Alloc*>(alloc)
  );
}

void RTCown_weak_acquire(RTCown* cown) {
  reinterpret_cast<rt::Cown*>(cown)->weak_acquire();
}

bool RTCown_acquire_strong_from_weak(RTCown* cown) {
  return reinterpret_cast<rt::Cown*>(cown)->acquire_strong_from_weak();
}

///
// RTAction

RTAction* RTAction_new(RTAlloc* alloc, RTActionDescriptor* desc) {
  rt::Action* action = reinterpret_cast<rt::Action*>(
    (reinterpret_cast<rt::Alloc*>(alloc))->alloc(desc->size)
  );
  new (action) rt::Action(reinterpret_cast<rt::Action::Descriptor*>(desc));
  return reinterpret_cast<RTAction*>(action);
}

void RTAction_schedule(RTAction* action, RTCown** cowns, size_t count) {
  rt::Cown::schedule(
    count,
    reinterpret_cast<rt::Cown**>(cowns),
    reinterpret_cast<rt::Action*>(action)
  );
}

///
// RTScheduler

void RTScheduler_set_detect_leaks(bool b) {
  rt::Scheduler::set_detect_leaks(b);
}

bool RTScheduler_get_detect_leaks() {
  return rt::Scheduler::get_detect_leaks();
}

void RTScheduler_set_allow_teardown(bool allow) {
  rt::Scheduler::set_allow_teardown(allow);
}

void RTScheduler_set_fair(bool fair) {
  rt::Scheduler::set_allow_teardown(fair);
}

bool RTScheduler_is_teardown_in_progress() {
  return rt::Scheduler::is_teardown_in_progress();
}

void RTScheduler_want_ld() {
  rt::Scheduler::want_ld();
}

void RTScheduler_init(size_t cores) {
  rt::Scheduler::get().init(cores);
}

void RTScheduler_run() {
  rt::Scheduler::get().run();
}

void RTScheduler_run_with_startup(RTCallbackFunction f, void* arg) {
  rt::Scheduler::get().run_with_startup(f, arg);
}

///
// RTSystematic

void RTSystematic_enable_crash_logging() {
  rt:Systematic::enable_crash_logging();
}

void RTSystematic_log(char* message) {
  rt:Systematic::cout() << message << std::endl;
}

void RTSystematic_log_ptr(char* message, void* ptr) {
  rt:Systematic::cout() << message << ": " << ptr << std::endl;
}

void RTSystematicTestHarness_run(int argc, char** argv, RTCallbackFunction f, void* arg) {
  SystematicTestHarness harness(argc, argv);
  harness.run(f, arg);
}
