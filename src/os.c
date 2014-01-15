/*************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery
*
* "os.c" - function implementations re-
* lated relating to "threads, mutexes and
* condition variables.
*
* Copyright (C) 2007-2014, Russell Potter,
* All rights reserved
*
**************************************************************/

#include <bikedemo/os.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "bdos_init_mtx" - returns a mu-
 * tex structure and whose initia-
 * lisation, locking, unlocking
 * and cleanup call-backs are set
 * from "init", "lck", "unlck"
 * and " ,clnp" , respectively,
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively. Retur-
 * ns NULL on error */
struct bdos_mtx_ops *bdos_init_mtx(
                     bdos_mx_init_fn init,
                     bdos_lck_fn lck,
                     bdos_unlck_fn unlck,
                     bdos_mx_clnp_fn clnp,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bdos_mtx_ops *ret; /* return
                          * value */

    ret = (struct bdos_mtx_ops *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdos_mtx_ops),
        logger);
    if (!(ret))
        return NULL;

    /* once allocation is
     * successful. set the
     * return value's
     * fields and return */
    ret->init  = init;
    ret->lck   = lck;
    ret->unlck = unlck;
    ret->clnp  = clnp;

    return ret;
}

/* "bdos_init_cvar" - returns
 * condition variable operat-
 * ions whose initialisation,
 * wait, notify  and cleanup
 * call-backs are set from
 * "init", wait", "ntfy" and
 * "clnp", respectively, usi-
 * ng the memory allocator
 * and error logger in "allo-
 * cs" and "logger", respect-
 * ively. Returns NULL on er-
 * ror */
struct bdos_cvar_ops *bdos_init_cvar(
                   bdos_cv_init_fn init,
                   bdos_wait_fn wait,
                   bdos_ntfy_fn ntfy,
                   bdos_cv_clnp_fn clnp,
                   struct bd_allocs *allocs,
	               struct bd_logger *logger)
{
    struct bdos_cvar_ops *ret; /* return
                                * value */

    ret = (struct bdos_cvar_ops *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdos_cvar_ops),
        		logger);
    if (!(ret))
    	return NULL;

    /* once allocation is
     * successful. set the
     * return value's
     * fields and return */
    ret->init = init;
    ret->wait = wait;
    ret->ntfy = ntfy;
    ret->clnp = clnp;

    return ret;
}

/* "bdos_init_thrd" - returns thr-
 * ead whose start and exit call-backs
 * are set from "start" and "exit",
 * respectively, using the memory all-
 * ocator and error logger in "allocs"
 * and "logger", respectively. Returns
 * NULL on error */
struct bdos_thrd_ops *bdos_init_thrd(
                     bdos_start_fn start,
                     bdos_exit_fn exit,
		             struct bd_allocs *allocs,
	                 struct bd_logger *logger)
{
    struct bdos_thrd_ops *ret; /* return
                                * value */

    ret = (struct bdos_thrd_ops *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdos_thrd_ops),
                 logger);
    if (!(ret))
    	return NULL;

    /* once allocation is
     * successful. set the
     * return value's
     * fields and return */
    ret->start = start;
    ret->exit  = exit;

    return ret;
}

/* "bdos_init - returns an operati-
 * ng-system structure whose archi-
 * tecture, mutex initalisation,
 * condition variable initalisation,
 * rgthread start and exit, mutex
 * locking and unlocking, condition
 * variable wait and notify and mut-
 * ex and condition variable cleanup
 * call- backs  are set from the fi-
 * rst to eleventh parameters, resp-
 * ectively, using the memory allo-
 * cator and error logger given in
 *  "mx_init", "cv_init", "start",
 * "exit", "lck", "unlck", "wait",
 * "ntfy", "load", "sym", "mx_clnp"
 * and "cv_clnp", respectively, us-
 * ing the memory allocator and er-
 * ror logger in "allocs" and "log-
 * ger", respectively.Returns NULL
 * on error */
struct bdos_os *bdos_init(bdos_mx_arch_fn arch,
                          bdos_mx_init_fn mx_init,
                          bdos_cv_init_fn cv_init,
                          bdos_start_fn start,
                          bdos_exit_fn exit,
                          bdos_lck_fn lck,
                          bdos_unlck_fn unlck,
                          bdos_wait_fn wait,
                          bdos_ntfy_fn ntfy,
                          bdos_load_fn load,
                          bdos_sym_fn sym,
                          bdos_mx_clnp_fn mx_clnp,
                          bdos_cv_clnp_fn cv_clnp,
                          struct bd_allocs *allocs,
	                      struct bd_logger *logger)
{
    struct bdos_os *ret;            /* return
                                     * value */
    struct bdos_mtx_ops *mtx_ops;   /* OS's
                                     * mutex
                                     * opera-
                                     * tions */
    struct bdos_cvar_ops *cvar_ops; /* OS's
                                     * condition
                                     * variable
                                     * opera-
                                     * tions */
    struct bdos_thrd_ops *thrd_ops; /* OS's
                                     * thread
                                     * opera-
                                     * tions */
    struct bdos_lib_ops *lib_ops;   /* OS's
                                     * shared
                                     * library
                                     * tions */



    if (!(mtx_ops = bdos_init_mtx(mx_init,
                                  lck,
                                  unlck,
                                  mx_clnp,
                                  allocs,
                                  logger)))
        return NULL;

    if (!(cvar_ops = bdos_init_cvar(cv_init,
                                    wait,
                                    ntfy,
                                    cv_clnp,
                                    allocs,
                                    logger)))
        return NULL;

    if (!(thrd_ops = bdos_init_thrd(start,
                                    exit,
                                    allocs,
                                    logger)))
        return NULL;

    if (!(lib_ops = bdos_init_lib(load,
                                  sym,
                                  allocs,
                                  logger)))
        return NULL;

    /* now these.initilisations
     * are successful we allocate
     * the operting system sto-
     * rage*/
    ret = (struct bdos_os *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdos_os),
                     logger);
    if (!(ret))
    	return NULL;

    /* ... and initialise its
     * fields */
    ret->arch   = arch;
    ret->mtxs   = mtx_ops;
    ret->cvars  = cvar_ops;
    ret->thrds  = thrd_ops;
    ret->libs   = lib_ops;

    return ret;
}
