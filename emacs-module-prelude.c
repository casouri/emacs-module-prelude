#include "emacs-module-prelude.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


/* Copied from Pillipp’s document.  I commented out assertions. */
bool
emp_copy_string_contents
(emacs_env *env, emacs_value value, char **buffer, size_t *size)
{
  ptrdiff_t buffer_size;
  if (!env->copy_string_contents (env, value, NULL, &buffer_size))
    return false;
  /* assert (env->non_local_exit_check (env) == emacs_funcall_exit_return); */
  /* assert (buffer_size > 0); */
  *buffer = (char*) malloc ((size_t) buffer_size);
  if (*buffer == NULL)
    {
      env->non_local_exit_signal (env, env->intern (env, "memory-full"),
                                  env->intern (env, "nil"));
      return false;
    }
  ptrdiff_t old_buffer_size = buffer_size;
  if (!env->copy_string_contents (env, value, *buffer, &buffer_size))
    {
      free (*buffer);
      *buffer = NULL;
      return false;
    }
  /* assert (env->non_local_exit_check (env) == emacs_funcall_exit_return); */
  /* assert (buffer_size == old_buffer_size); */
  *size = (size_t) (buffer_size - 1);
  return true;
}

emacs_value
emp_intern (emacs_env *env, const char *name)
{
  return env->intern (env, name);
}

emacs_value
emp_funcall (emacs_env *env, const char* fn, ptrdiff_t nargs, ...)
{
  va_list argv;
  va_start (argv, nargs);
  emacs_value *args = (emacs_value *) malloc(nargs * sizeof(emacs_value));
  for (int idx = 0; idx < nargs; idx++)
    {
      args[idx] = va_arg (argv, emacs_value);
    }
  va_end (argv);
  emacs_value val = env->funcall (env, emp_intern (env, fn), nargs, args);
  free (args);
  return val;
}

void
emp_provide (emacs_env *env, const char *feature)
{
  emp_funcall (env, "provide", 1, emp_intern (env, feature));
}

void
emp_signal_message1
(emacs_env *env, const char *name, const char *message)
{
  env->non_local_exit_signal
    (env, env->intern (env, name),
     emp_funcall (env, "cons", 2,
                  env->make_string (env, message, strlen (message)),
                  emp_intern (env, "nil")));
}

void
emp_define_error
(emacs_env *env, const char *name,
 const char *description, const char *parent)
{
  emp_funcall (env, "define-error", 3,
               emp_intern (env, name),
               env->make_string (env, description, strlen (description)),
               emp_intern (env, parent));
}

bool
emp_nilp (emacs_env *env, emacs_value val)
{
  return !env->is_not_nil (env, val);
}

void
emp_define_function
(emacs_env *env, const char *name, ptrdiff_t min_arity,
 ptrdiff_t max_arity,
 emacs_value (*function) (emacs_env *env,
                          ptrdiff_t nargs,
                          emacs_value* args,
                          void *data) EMACS_NOEXCEPT,
 const char *documentation)
{
  emacs_value fn = env->make_function
    (env, min_arity, max_arity, function, documentation, NULL);
  emp_funcall (env, "fset", 2, emp_intern (env, name), fn);
}
