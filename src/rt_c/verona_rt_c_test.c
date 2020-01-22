#include <stdio.h>
#include <stdint.h>

#include "verona_rt_c.h"

///
// C API example - implements the same semantics as the bank1.verona example.

struct BankAccount {
  void* _rt_cown_overhead[8];
  uint64_t balance;
};
typedef struct BankAccount BankAccount;
void BankAccount_Trace(BankAccount* o, RTObjectStack* st) { return; }
RTDescriptor BankAccount_Desc = {
  sizeof(BankAccount),
  (RTDescriptorTraceFunction)BankAccount_Trace,
  NULL,
  NULL,
  NULL
};

///
// BankAccount_create - create a cown[BankAccount] with an opening balance.

BankAccount* BankAccount_create(uint64_t opening_balance) {
  BankAccount* self =
    (BankAccount*)RTCown_alloc(&BankAccount_Desc);

  self->balance = opening_balance;

  printf("opening balance: %lld\n", self->balance);
  return self;
}

///
// BankAccount_add - asynchronously add an amount to the given BankAccount.

struct BankAccount_add_Action {
  RTActionDescriptor* desc;
  BankAccount* self;
  uint64_t amount;
};
typedef struct BankAccount_add_Action BankAccount_add_Action;
void BankAccount_add_ActionTrace(BankAccount_add_Action* a, RTObjectStack* st) {
  RTObjectStack_push(st, (RTObject*)a->self);
}
void BankAccount_add_fn(BankAccount_add_Action* action);
RTActionDescriptor BankAccount_add_ActionDesc = {
  sizeof(BankAccount_add_Action),
  (RTActionFunction)BankAccount_add_fn,
  (RTActionTraceFunction)BankAccount_add_ActionTrace
};
void BankAccount_add(BankAccount* self, uint64_t amount) {
  BankAccount_add_Action* action =
    (BankAccount_add_Action*)RTAction_new(&BankAccount_add_ActionDesc);
  action->self = self;
  action->amount = amount;

  RTAction_schedule((RTAction*)action, (RTCown**)&self, 1);
}
void BankAccount_add_fn(BankAccount_add_Action* action) {
  // All the above is just boilerplate; this is where the action is implemented.
  action->self->balance = action->self->balance + action->amount;
  printf("new balance: %lld\n", action->self->balance);
}

///
// Example creation of a bank account and payments made into it.

void startup(void* arg) {
  BankAccount* account = BankAccount_create(1000);
  // RTCown_acquire((RTCown*)account);
  BankAccount_add(account, 200);
  BankAccount_add(account, 500);
  BankAccount_add(account, 1500);
  RTCown_release((RTCown*)account);

  printf("startup function complete!\n");
}

int main(int argc, const char* argv[])
{
  RTSystematic_enable_crash_logging();
  RTScheduler_init(1);
  RTScheduler_want_ld();
  RTScheduler_set_detect_leaks(true);
  printf("RTScheduler setup complete!\n");

  RTScheduler_run_with_startup(startup, NULL);
  printf("RTScheduler finished!\n");
}
