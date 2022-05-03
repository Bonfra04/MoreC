#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__
#define UNPACK(...) __VA_ARGS__

#define FOR_EACH(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...) macro(a1) __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define FOR_EACH_ARG(macro, arg, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER_ARG(macro, arg, __VA_ARGS__)))
#define FOR_EACH_HELPER_ARG(macro, arg, a1, ...) macro(arg, a1) __VA_OPT__(FOR_EACH_AGAIN_ARG PARENS (macro, arg, __VA_ARGS__))
#define FOR_EACH_AGAIN_ARG() FOR_EACH_HELPER_ARG

#define FIRST(a, ...) a
#define FROM_FIRST(a, ...) __VA_ARGS__
#define SECOND(a, b, ...) b
#define FROM_SECOND(a, b, ...) __VA_ARGS__

#define ARG_CNT(vrg1,vrg2,vrg3,vrg4,vrg5,vrg6,vrg7,vrg8,vrgN, ...) vrgN
#define ARG_COUNT(...) ARG_CNT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define VA_EAT(...)
#define VA_IDENT(...) __VA_ARGS__
#define VA_ARG0_(A0,...) A0
#define VA_ARG0(...) VA_ARG0_(__VA_ARGS__)
#define VA_ARGTAIL_(A0,...) __VA_ARGS__
#define VA_ARGTAIL(...) VA_ARGTAIL_(__VA_ARGS__)
#define IFNE(...) VA_ARGTAIL __VA_OPT__((,)VA_ARG0)
#define IFE(...) VA_IDENT __VA_OPT__(()VA_EAT)

#define isCompatible(x, type) _Generic(x, type: 1, default: 0)
#define CONDITION_CHAIN(a, b) && IFNE(a)(isCompatible(a, b)) IFE(a)(0)

#define FOR_EACH_OVER_TWO(macro, a, ...) __VA_OPT__(FOR_EACH_OVER_TWO_HELPER(macro, a, __VA_ARGS__))
#define FOR_EACH_OVER_TWO_HELPER(macro, a, ...) __VA_OPT__(    \
    macro(FIRST a, FIRST(__VA_ARGS__))FOR_EACH_OVER_TWO_AGAIN PARENS (macro, (FROM_FIRST a), FROM_FIRST(__VA_ARGS__))\
)

#define FOR_EACH_OVER_TWO_AGAIN() FOR_EACH_OVER_TWO_HELPER

#define correctAmount(args, types, body...) if(            \
    isCompatible(FIRST args, FIRST types)               \
    FOR_EACH_OVER_TWO(CONDITION_CHAIN,                  \
        (FROM_FIRST args), FROM_FIRST types             \
    )                                                   \
) { body }

#define checkCompatibility(args, types, body...) \
if( ARG_COUNT args == ARG_COUNT types) correctAmount(args, types, body)

#define SELECT_OVERLOAD(args, sign) checkCompatibility(args, (FROM_SECOND sign), func_ptr = SECOND sign;)

#define __inner_foo_with_args(...) VA_IDENT
#define __inner_foo() VA_EAT
#define foo(...) __inner_foo##__VA_OPT__(_with_args)(__VA_OPT__(__VA_ARGS__))

#define TYPEOF(x) foo(x)(, typeof(x))
#define CALL(addr, type, args...) ((type (*)(   \
    typeof(FIRST(args))                         \
    FOR_EACH(TYPEOF, FROM_FIRST(args))          \
    ))addr)(args)

#define OVERLOAD(args...) ({                \
    void* func_ptr = 0;                     \
    FOR_EACH_ARG(                           \
        SELECT_OVERLOAD,                    \
        (FROM_FIRST(args)),                 \
        EXPAND(UNPACK FIRST(args))          \
    )                                       \
    CALL(func_ptr, void, FROM_FIRST(args)); \
})
