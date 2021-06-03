#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>
#include "omp-tools.h"


int ompt_get_task_type_flags(int ancestor_level);
void assert_task_type_flags(int detected_task_type, int expected_task_type);


int main(int argc, char **argv) {

#pragma omp parallel num_threads(1)
  {
    printf("Parallel region 1.\n");
    assert_task_type_flags(ompt_get_task_type_flags(0), ompt_task_implicit);

#pragma omp parallel num_threads(1)
    {
      printf("Parallel region 2.\n");
      assert_task_type_flags(ompt_get_task_type_flags(0), ompt_task_implicit);

#pragma omp task
      {
        printf("Explicit task 1.\n");
        assert_task_type_flags(ompt_get_task_type_flags(0), ompt_task_explicit);

#pragma omp task
        {
          printf("Explicit task 2.\n");
          assert_task_type_flags(ompt_get_task_type_flags(0), ompt_task_explicit);

#pragma omp parallel num_threads(1)
          {
            printf("Parallel region 3.\n");
            // ompt_get_task_info returns explicit task flags
            assert_task_type_flags(ompt_get_task_type_flags(0), ompt_task_implicit);
#if 0
#pragma omp parallel num_threads(1)
            {
              printf("Parallel region 4.\n");
              assert_task_type_flags(ompt_get_task_type_flags(0), ompt_task_implicit);
            }
#endif
          }
        }
      }
    }
  }
}


#define ompt_interface_fn(f) \
  static f ## _t f ## _fn;

FOREACH_OMPT_INQUIRY_FN(ompt_interface_fn)

#undef ompt_interface_fn


ompt_start_tool_result_t init;


static void
ompt_init_inquiry_fn_ptrs
(
  ompt_function_lookup_t ompt_fn_lookup
)
{
#define ompt_interface_fn(f) \
  f ## _fn = (f ## _t) ompt_fn_lookup(#f);

  FOREACH_OMPT_INQUIRY_FN(ompt_interface_fn)

#undef ompt_interface_fn
}



static int
ompt_initialize
(
  ompt_function_lookup_t lookup,
  int initial_device_num,
  ompt_data_t *tool_data
)
{
  printf("Initializing the tool\n");

  ompt_init_inquiry_fn_ptrs(lookup);

  return 1;
}


void
ompt_finalize
(
  ompt_data_t *tool_data
)
{
  printf("Finalizing the tool\n");
}


ompt_start_tool_result_t *
ompt_start_tool
(
  unsigned int omp_version,
  const char *runtime_version
)
{
  printf("Starting the tool\n");

  init.initialize = &ompt_initialize;
  init.finalize = &ompt_finalize;

  return &init;
}


int ompt_get_task_type_flags(int ancestor_level) {
  int task_type_flags;
  int ret = ompt_get_task_info_fn(ancestor_level, &task_type_flags,
                                  NULL, NULL, NULL, NULL);
  assert(ret == 2);
  return task_type_flags;
}


void assert_task_type_flags(int detected_task_type, int expected_task_type) {
  if (! (detected_task_type & expected_task_type) ) {
    printf("Detected flags: %x, expected flags: %x\n",
           detected_task_type, expected_task_type);
  }
  assert(detected_task_type & expected_task_type);
}



