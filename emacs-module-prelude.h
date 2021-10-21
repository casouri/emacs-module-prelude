#include "emacs-module.h"
#include <stdbool.h>

#ifndef EMACS_MODULE_PRELUDE_H
#define EMACS_MODULE_PRELUDE_H

#define EMP_MAJOR_VERSION 1
#define EMP_MINOR_VERSION 0
#define EMP_PATCH_VERSION 0


/*
  Copy a Lisp string VALUE into BUFFER, and store the string size in
  SIZE.  A user doesn’t need to allocate BUFFER, but it is the user’s
  responsibility to free it.
 */
bool
emp_copy_string_contents
(emacs_env *env, emacs_value value, char **buffer, size_t *size);

/*
  Intern NAME to a symbol.
 */
emacs_value
emp_intern (emacs_env *env, const char *name);

/*
  Call a function named FN which takes NARGS number of arguments.
  Example: funcall (env, "cons", 2, car, cdr);
 */
emacs_value
emp_funcall (emacs_env *env, const char* fn, ptrdiff_t nargs, ...);

/*
  Provide FEATURE like ‘provide’ in Lisp.
*/
void
emp_provide (emacs_env *env, const char *feature);
  
/*
  Raise a signal where NAME is the signal name and MESSAGE is the
  error message.
 */
void
emp_signal_message1
(emacs_env *env, const char *name, const char *message);

/*
  Define an error like ‘define-error’.
 */
void
emp_define_error
(emacs_env *env, const char *name,
 const char *description, const char *parent);

/*
  Return true if VAL is symbol nil.
 */
bool
emp_nilp (emacs_env *env, emacs_value val);

/*
  Define a function NAME. The number of arguments that the function
  takes is between MIN_ARITY and MAX_ARITY.  FUNCTION is a function
  with signature

  static emacs_value
  function
  (emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
  EMACS_NOEXCEPT

  DOCUMENTATION is the docstring for FUNCTION.
 */
void
emp_define_function
(emacs_env *env, const char *name, ptrdiff_t min_arity,
 ptrdiff_t max_arity,
 emacs_value (*function) (emacs_env *env,
                          ptrdiff_t nargs,
                          emacs_value* args,
                          void *data) EMACS_NOEXCEPT,
 const char *documentation);

#endif /* EMACS_MODULE_PRELUDE_H */
