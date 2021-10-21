#include "emacs-module-prelude.h"

int plugin_is_GPL_compatible;

static const char emp_test_doc[] =
  "EMP test function.\n"
  "\n"
  "Return (upcase STRING).\n"
  "(fn STRING)";

emacs_value
Femp_test
(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
  EMACS_NOEXCEPT
{
  emacs_value lisp_string = args[0];
  if (emp_nilp (env, lisp_string))
    emp_signal_message1 (env, "wrong-type-argument", "oops");
  return emp_funcall (env, "upcase", 1, lisp_string);
}

int
emacs_module_init (struct emacs_runtime *ert) EMACS_NOEXCEPT
{
  emacs_env *env = ert->get_environment (ert);
  emp_define_error (env, "emp-test-error",
                    "This is impossible!!!", "error");
  emp_define_function (env, "emp-test", 1, 1,
                       &Femp_test, emp_test_doc);
  emp_provide (env, "emp-test");
  /* Return 0 to indicate module loaded successfully. */
  return 0;
}
