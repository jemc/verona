#include <stdio.h>
#include "verona_c.h"

///
// Define an example object descriptor that does nothing.

void example_trace(RTObject* o, RTObjectStack* st) {
  return;
}

RTDescriptor example_desc = {512, example_trace, NULL, NULL, NULL};

///
// Allocate some stuff and run the runtime.

int main(int argc, const char* argv[])
{
  RTScheduler_init(4);
	printf("RTScheduler_init complete!\n");

  RTCown* my_cown = RTCown_alloc(&example_desc);
	printf("my_cown allocated at: %p\n", my_cown);

	printf("my_cown's descriptor at: %p\n", RTCown_get_descriptor(my_cown));
	printf("my original descriptor at: %p\n", &example_desc);

  RTCown_acquire(my_cown);
	printf("my_cown acquired!\n");

  RTScheduler_run();
	printf("RTScheduler_run complete!\n");
}
