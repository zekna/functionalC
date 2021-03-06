#include <stdlib.h>
#include <unistd.h>
#include "list.h"
#include "closure.h"

envobj *
envitem(void *var, ssize_t size) {
  envobj *env = malloc(sizeof(envobj));
  env->val = var;
  env->size = size;
  return env;
}

void *
unbox(list *l) {
  envobj *env = (envobj *)l->val;
  return env->val; 
}

closure *
bind(closure *c, void *(*fn)(list *), envobj *env) {
  closure *cl;
  if (c == NULL) {
    cl = malloc(sizeof(closure)); 
    if (cl == NULL) {
      exit(1);
    }
    cl->env = NULL;
    cl->fn = fn;
  }
  else {
    cl = c;
  }
  cl->env = append(cl->env, (void *)env); 
  return cl;
}

void *
call(closure *c, envobj *env) {
  
  list *copylist = copy(c->env);
  copylist = append(copylist, (void *)env);
  return c->fn(copylist);
}

//helper functions (syntactic sugar...erm...i guess...)
//these make using closures easier
envobj *
liftint(int a) {
  int *v = malloc(sizeof(int));
  *v = a;
  envobj *o = envitem((void *)v, sizeof(int)); 
  return o;
}

list *
liftlist(list *l, ssize_t s) {
  list *o = NULL;
  list *curr;

  for (curr = l; curr != NULL; curr = curr->next) {
     envobj *lifted = envitem(curr->val, s); 
     o = append(o, (void *)lifted);
  }
  return o;
}

