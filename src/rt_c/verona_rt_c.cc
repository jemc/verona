#include <verona.h>
using namespace verona;

extern "C" {
  #include "verona_rt_c.h"
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

RTCown* RTCown_alloc(RTDescriptor* desc) {
  rt::Cown* cown = rt::Cown::alloc(
    rt::ThreadAlloc::get(),
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

void RTCown_release(RTCown* cown) {
  rt::Cown::release(
    rt::ThreadAlloc::get(),
    reinterpret_cast<rt::Cown*>(cown)
  );
}

void RTCown_weak_release(RTCown* cown) {
  reinterpret_cast<rt::Cown*>(cown)->weak_release(rt::ThreadAlloc::get());
}

void RTCown_weak_acquire(RTCown* cown) {
  reinterpret_cast<rt::Cown*>(cown)->weak_acquire();
}

bool RTCown_acquire_strong_from_weak(RTCown* cown) {
  return reinterpret_cast<rt::Cown*>(cown)->acquire_strong_from_weak();
}

///
// RTAction

RTAction* RTAction_new(RTActionDescriptor* desc) {
  return reinterpret_cast<RTAction*>(
    new rt::Action(reinterpret_cast<rt::Action::Descriptor*>(desc))
  );
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

void RTScheduler_run_with_startup(RTSchedulerStartupFunction f, void* arg) {
  rt::Scheduler::get().run_with_startup(f, arg);
}
