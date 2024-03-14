import time
from concurrent.futures import ThreadPoolExecutor


def task(n: int, s: str) -> int:
    time.sleep(n)
    # print("number: %d, str: %s" % (n, s))
    return n


if __name__ == '__main__':
    future_list = []
    with ThreadPoolExecutor(max_workers=5) as executor:
        for i in [(1, "A"), (2, "B"), (3, "C")]:
            future = executor.submit(task, i[0], i[1])
            future_list.append(future)

        while future_list:
            for future in future_list:
                if future.done():
                    print("task result:", future.result())
                    future_list.remove(future)
                else:
                    print("task running")
            print("-")
            time.sleep(1)
