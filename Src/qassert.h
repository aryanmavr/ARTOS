/*******************************************************************************
 * ARTOS - ARM Real-Time Operating System
 *
 * Author: Aryan
 * Date: 18 May 2025
 *
 * Description: Design by Contract (DbC) assertions for embedded systems
 ******************************************************************************/
#ifndef QASSERT_H
#define QASSERT_H

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef Q_NASSERT
/* Define the user-specified module name for assertions in this file */
#define Q_DEFINE_THIS_FILE static char const Q_this_file_[] = __FILE__;

/* General-purpose assertion with user-specified ID number */
#define Q_ASSERT_ID(id_, expr_) ((expr_) \
    ? ((void)0) : Q_onAssert(&Q_this_file_[0], (id_)))

/* Assertion for a wrong path through the code */
#define Q_ERROR_ID(id_) Q_onAssert(&Q_this_file_[0], (id_))

/* Assertion that always evaluates the expression */
#define Q_ALLEGE_ID(id_, expr_) Q_ASSERT_ID((id_), (expr_))
#else
#define Q_DEFINE_THIS_FILE
#define Q_ASSERT_ID(id_, expr_) ((void)0)
#define Q_ERROR_ID(id_) ((void)0)
#define Q_ALLEGE_ID(id_, expr_) ((void)(expr_))
#endif

/* Define the file name for assertions in this file */
#define Q_DEFINE_THIS_MODULE(name_) \
    static char const Q_this_module_[] = name_

/* General-purpose assertion with line number */
#define Q_ASSERT(expr_) Q_ASSERT_ID(__LINE__, (expr_))

/* Assertion for a wrong path through the code */
#define Q_ERROR() Q_ERROR_ID(__LINE__)

/* Assertion for checking preconditions */
#define Q_REQUIRE_ID(id_, expr_) Q_ASSERT_ID((id_), (expr_))
#define Q_REQUIRE(expr_) Q_ASSERT(expr_)

/* Assertion for checking postconditions */
#define Q_ENSURE_ID(id_, expr_) Q_ASSERT_ID((id_), (expr_))
#define Q_ENSURE(expr_) Q_ASSERT(expr_)

/* Assertion for checking invariants */
#define Q_INVARIANT_ID(id_, expr_) Q_ASSERT_ID((id_), (expr_))
#define Q_INVARIANT(expr_) Q_ASSERT(expr_)

/* Assertion that always evaluates the expression */
#define Q_ALLEGE(expr_) Q_ALLEGE_ID(__LINE__, (expr_))

/* Static (compile-time) assertion */
#define Q_ASSERT_STATIC(expr_) \
    extern char Q_static_assert_[(expr_) ? 1 : -1]

#ifndef Q_NORETURN
#define Q_NORETURN _Noreturn void
#endif

#ifndef QP_VERSION
typedef int int_t;
#endif

/* Callback function invoked in case of an assertion failure */
Q_NORETURN Q_onAssert(
    char const * module,
    int_t location);

#ifndef QP_VERSION
/* Helper macro to calculate static dimension of a 1-dim array */
#define Q_DIM(array_) (sizeof(array_) / sizeof((array_)[0U]))
#endif

#ifdef __cplusplus
    }
#endif

#endif /* QASSERT_H */
