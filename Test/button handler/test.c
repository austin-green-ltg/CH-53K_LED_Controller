/*============================================================================
* ET: embedded test; very simple test example
============================================================================*/
// #include "pwm_handler.h"
#include "et.h" /* ET: embedded test */
#include "button_handler.h"  /* CUT */
#include <stdio.h>

#define PRESS (0)
#define HOLD  (1)

void setup(void) {
    /* executed before *every* non-skipped test */
}

void teardown(void) {
    /* executed after *every* non-skipped and non-failing test */
}

/* test group --------------------------------------------------------------*/
TEST_GROUP("Button Handler")
{

    /* start button_handler tests */
    // Currently No Additional Tests
    /* end button_handler tests */

} /* TEST_GROUP() */