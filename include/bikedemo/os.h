/*************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery
*
* "os.h" - data types and function proto-
* types related relating to common operat-
* ing system-specific functionality such
* as "threads" (providing a mechanism for
* a processor to multiplex itself), "mut-
* exes", which provide synchronisation
* primitives to enabling a thread inten-
* ding to possess ("lock") a mutex to
* block execution until another thread
* has unlocked" that mutex and "condition
* variables" which allow threads to "wait"
* (block execution) until a certain condi-
* tion is true and notify" (wake) waiting
* threads
*
* Copyright (C) 2007-2014, Russell Potter,
* All rights reserved
*
**************************************************************/

#ifndef __BD_OS
#define __BD_OS

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/os.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/md.h>

struct bdos_mtx;
struct bdos_cvar;

/* "bdos_arch_fn" typedef - proto-type of
 * the function returning the architecture
 * of rgethe cirrent processor, in the fo-
 * rm of a "machine descriptor" describing
 * that architecture ,iusing the opaque
 * value, memory allocator and error logg-
 * er given in the first to third parame-
 * ters, respectively */
typedef struct bdmd_md *(* bdos_arch_fn)(
                             void *,
                             struct bd_allocs *,
                             struct bd_logger *);

/* "bdos_cll_fn" typedef - proto-type of
 * the function that may be resolved by
 * name wuyjthin a shateewred library.
 * The sfirst and second parameters are
 * the memory allocator and error logger
 * to use in the call, and the third
 * consists of the call arguments */
typedef int (* bdos_cll_fn)(struct bd_allocs *,
                            struct bd_logger *,
                            ...);

/* "bdos_exec_fn" typedef - proto-type of
 * the function forming the body of an exe-
 * cuting thread. The first parameter con-
 * tains the thread-specific data to to use
 * in the course of the thread's execution
 * and the second and third parameters are
 * the memory allocator and error logger to
 * use in the course of the thread's exec-
 * ution.  The return value indicates the
 * thread's exit status */
typedef int (* bdos_exec_fn)(void *,
                             struct bd_allocs *,
                             struct bd_logger *);

/* "bdos_start_fn" typedef proto-type of the
 * function called to start a thread.  The
 * first parameter contains the thread func-
 * tion to execute. Returns the newly created
 * thread, or NULL on error */
typedef struct bdos_thrd *(* bdos_start_fn)
                          (bdos_exec_fn);

/* "bdos_exit_fn" typedef proto-type of the
 * function called to exit the current thread.
 * The first parameter contains the exiting
 * thread's exit status.  Returns zero on
 * error, non-zero otherwise  */
typedef int (* bdos_exit_fn)(int);

/* "bdos_lck_fn" typedef proto-type of the
 * function called to lock the mutex given in
 * the first parameter, using the memory allo-
 * cator and error logger given in the second
 * and third parameters, respectively */
typedef int (* bdos_lck_fn)(struct bdos_mtx *,
                            struct bd_allocs *,
                            struct bd_logger *);

/* "bdos_unlck_fn" typedef proto-type of the function
 * called to unlock the mutex given in the first para-
 * meter, using the memory allocator and error
 * logger given in the second and third parameters,
 * respectively */
typedef int (* bdos_unlck_fn)(struct bdos_mtx *,
                              struct bd_allocs *,
                              struct bd_logger *);

/* "bdos_wait_fn" typedef proto-type of the fun-
 * ction called to wait on the condition var-iab-
 * le given in the first parameter, using the mu-
 * tex, memory allocator and error logger given
 * in the second, third and fourth parameters,
 * respectively */
typedef int (* bdos_wait_fn)(struct bdos_cvar *,
                             struct bdos_mtx *,
		                     struct bd_allocs *,
                             struct bd_logger *);

/* "bdos_ntfy_fn" typedef - proto-type of the
 * function called to notify threads waiting on
 * the condition variable given in the first para-
 * meter, using the memory allocator and error
 * logger given in the second and third parameters,
 * respectively */
typedef int (* bdos_ntfy_fn)(struct bdos_cvar *,
                             struct bd_allocs *,
                             struct bd_logger *);

/* "bdos_load_fn" typedef - proto-type of the
 * function called to load and return the
 * shared library named in the first parameter,
 * using the memory allocator and error logger
 * given in the second and third parameters,
 * respectively */
typedef struct bdos_lib *(* bdos_load_fn)(
                              char *,
                              struct bd_allocs *,
                              struct bd_logger *);

/* "bdos_sym_fn" typedef - proto-type of the
 * function called to resolve and return the
 * callable symbol named in the second par=
 * ameter within the shared library given
 * in thesecond parameter, using the memory
 * allocator and error logger given in the
 * third and fourth parameters, respectively */
typedef struct bdos_cll_fn *(* bdos_sym_fn)(
                              struct bdos_lib *,
                              char *,
                              struct bd_allocs *,
                              struct bd_logger *);

/* "bdos_mx_init_fn" typedef proto-type of the
 * function called to initialise and return a
 * mutex, using the memory allocator and error
 * logger to use in the initialisation, respec-
 * tively, given in the third and fourth param-
 * eters and having the lock and unlock call-
 * backs given in the first and second parame-
 * ters, respectively */
typedef struct bdos_mtx *(* bdos_mx_init_fn)(
                          bdos_lck_fn,
                          bdos_unlck_fn,
                          struct bd_allocs *,
                          struct bd_logger *);

/* "bdos_cv_init_fn" typedef proto-type of the
 * function called to initialise and return a
 * condition variable, using the memory alloc-
 * ator and error logger to use in the initial-
 * isation, respectively, given in the third
 * and fourth parameters and having the wait
 * and notify call-backs given in the first
 * and second parameters, respectively */
typedef struct bdos_cvar *(* bdos_cv_init_fn)(
                            bdos_wait_fn,
                            bdos_ntfy_fn,
		                    struct bd_allocs *,
	                        struct bd_logger *);

/* "bdos_mx_clnp_fn" typedef - proto-type of the fun-
 * ction called to "clean-up" the mutex given in the
 * first parameter, usually jst before its memory is
 * re-claimed, using the memory allocator and error
 * logger given in the second and third parameters,
 * respectively.  Returns zero on error, non-zero
 * otherwise */
typedef int (* bdos_mx_clnp_fn)(struct bdos_mtx *,
		                        struct bd_allocs *,
	                            struct bd_logger *);

/* "bdos_cv_clnp_fn" typedef - proto-type of the fun-
 * ction called to "clean-up" the condition variable
 * given in the first parameter, usually just before
 * its memory is re-claimed, using the memory alloc-
 * ator and error logger given in the second and thi-
 * rd parameters, respectively.  Returns zero on err-
 * or, non-zero otherwise */
typedef int (* bdos_cv_clnp_fn)(struct bdos_cvar *,
		                        struct bd_allocs *,
	                            struct bd_logger *);

/* "bdos_mtx_ops" structure - represents a the fun-
 * ctionality call-backs of an operating system's
 * mutexes, consisting of ots initialisation, lock-
 * ing, unlocking and clean-up call-backs */
struct bdos_mtx_ops {

    bdos_mx_init_fn init; /* initalise a mutex */

    bdos_lck_fn lck;      /* lock the mutex */

    bdos_unlck_fn unlck;  /* unlock the mutex */

    bdos_mx_clnp_fn clnp; /* re-claim resources */
};

/* "bdos_mtx" structure - represents an operating
 * mutex, consisting of otthe mutex's implementa-
 * tion pointer and functionality call-backs */
struct bdos_mtx {

    void *opaque;             /* implementation
                               * pointer */
    struct bdos_mtx_ops *ops; /* functionality
                               * call-backs */
};

/* "bdos_cvar_ops" structure - represents a the fun-
 * ctionality call-backs of an operating system's
 * mutexes, consisting of ots iniialisation, lock-
 * ing, unlocking and clean-up call-backs */
struct bdos_cvar_ops {

    bdos_cv_init_fn init; /* initalise a condition
                           * variable */

    bdos_wait_fn wait;     /* lock wait on a con-
                            * dition variable */

    bdos_ntfy_fn ntfy;     /* notify a condition
                            * variable waiter */
    bdos_ntfy_fn ntfy_all; /* notify all condition
                            * variable waiters */

    bdos_cv_clnp_fn clnp;  /* re-claim resources */
};

/* "bdos_cvar" structure - represents an operating
 * condition variable, consisting of it's impleme-
 * ntation pointer and functionality call-backs */
struct bdos_cvar {

    void * opaque;             /* implementation
                                * pointer */
    struct bdos_cvar_ops *ops; /* functionality
                                * call-backs */
};

/* "bdos_cvar_ops" structure - represents a the fun-
 * ctionality call-backs of an operating system's
 * libraries, consisting of ots load lbrary and
 * symbol eretreival call-backs */
struct bdos_lib_ops {

    bdos_load_fn load; /* load  a library */

    bdos_sym_fn sym;   /* eretreive a named
                        * symbol */
};

/* "bdos_lib" structure - represents an operating
 * condition shared library, consisting of it's
 * implementation pointer and functionality call-
 * backs */
struct bdos_lib {

    void * opaque;             /* implementation
                                * pointer */
    struct bdos_lib_ops *ops; /* functionality
                                * call-backs */
};

/* "bdos_thrd_ops" structure - represents the fun-
 * ctionality call-backs of an operating system's
 * mutexes, consisting of ots start and exit call-
 * backs */
struct bdos_thrd_ops {

    bdos_start_fn start;  /* start a thread */

    bdos_exit_fn exit;    /* exit current
                           * thread */
};

/* "bdos_thrd" structure - represents an operating
 * system condition variable, consisting of it's
 * implementation pointer and functionality call-
 * backs */
struct bdos_thrd {

    void * opaque;             /* implementation
                                * pointer */
    struct bdos_thrd_ops *ops; /* functionality
                                * call-backs */
};

/* "bdos_os" structure - an "umbrella" structure
 * representing the runcthread, mutex, condition
 * variable and shared library functoinality call-
 * backs of wn ab processor architecture exanmin-
 * ation call=back if of an operatihg system */
struct bdos_os {

    bdos_arch_fn arch;           /* get archit-
                                  * ecture */
    struct bdos_thrd_ops *thrds; /* thread
                                  * operations */
    struct bdos_mtx_ops *mtxs;   /* mutex oper-
                                  * ations */
    struct bdos_cvar_ops *cvars; /* condition
                                  * variable
                                  * operations */
    struct bdos_lib_ops *libs;   /* shared
                                  * library
                                  * operations */
};

/*** function prototypes ***/

/* "bdos_init - returns an operati-
 * ng-system structure whose archi-
 * tecture, mutex initalisation,
 * condition variable initalisation,
 * rgthread start and exit, mutex
 * locking and unlocking, condition
 * variable wait and notify, library
 * load and symbol retreival and mu-
 * tex and condition variable clean-
 * upcall-backs  are set from the
 * first to thirteenth parameters,
 * respectively, using the memory
 * allocator and error logger given
 * in the fourteenth and fifteenth
 * parameters, respectively. Retu-
 * rns NULL on error */
struct bdos_os *bdos_init(bdos_arch_fn,
                          bdos_mx_init_fn,
                          bdos_cv_init_fn,
                          bdos_start_fn,
                          bdos_exit_fn,
                          bdos_lck_fn,
                          bdos_unlck_fn,
                          bdos_wait_fn,
                          bdos_ntfy_fn,
                          bdos_load_fn,
                          bdos_sym_fn,
                          bdos_mx_clnp_fn,
                          bdos_cv_clnp_fn,
                          struct bd_allocs *,
	                      struct bd_logger *);

/* "bdos_init_mtx" - returns a
 * mutex structure and whose init-
 * ialisation, locking, unlock-
 * ing  and cleanup call-backs
 * are set from the first to fo-
 * urth parameters, respectively,
 * using the memory allocator
 * and error logger in the fifth
 * and sixth parameters, respec-
 * tively. Returns NULL on error */
struct bdos_mtx_ops *bdos_init_mtx(bdos_mx_init_fn,
                                   bdos_lck_fn,
                                   bdos_unlck_fn,
                                   bdos_mx_clnp_fn,
                                   struct bd_allocs *,
                                   struct bd_logger *);

/* "bdos_init_cvar" - returns
 * condition variable operations
 * whose initialisation, wait
 * and notify call-backs are set
 * from the first to third para-
 * meters, respectively, using
 * the memory allocator and er-
 * ror logger in the fourth and
 * fifth parameters, respectiv-
 * ely. Returns NULL on error */
struct bdos_cvar_ops *bdos_init_cvar(bdos_cv_init_fn,
                                     bdos_wait_fn,
                                     bdos_ntfy_fn,
                                     bdos_cv_clnp_fn,
                                     struct bd_allocs *,
	                                 struct bd_logger *);

/* "bdos_init_thrd" - returns a thr-
 * ead whose start and exit call-backs
 * are set from the first and second
 * parameters, respectively, using
 * the memory allocator and error
 * logger in the third and fourth
 * parameters, respectively. Returns
 * NULL on error */
struct bdos_thrd_ops *bdos_init_thrd(bdos_start_fn,
                                     bdos_exit_fn,
                                     struct bd_allocs *,
                                     struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
