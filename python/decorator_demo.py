import functools


def decorator(fn):
    @functools.wraps(fn)
    def wrap(*args, **kwargs):
        print("--- before ---")
        fn(*args, **kwargs)
        print("--- after ---")
    return wrap


def decorator_arg(symbol="-"):
    def wrap(fn):
        @functools.wraps(fn)
        def w(*args, **kwargs):
            print("{0} before {0}".format(symbol))
            fn(*args, **kwargs)
            print("{0} after {0}".format(symbol))
        return w
    return wrap


@decorator
@decorator_arg(symbol="+")
def sayhi(name):
    print("hi,", name)


if __name__ == '__main__':
    sayhi("tom")
    """
    output:
        --- before ---
        + before +
        hi, tom
        + after +
        --- after ---
    """
