import time
import datetime

FORMAT = r'%Y-%m-%d %H:%M:%S'


def get_timestamp() -> int:
    return int(time.time())


def time_to_str() -> str:
    # time.localtime() -> struct_time
    return time.strftime(FORMAT, time.localtime())


def str_to_time(s: str) -> time.struct_time:
    return time.strptime(s, FORMAT)


def struct_to_timestamp(t: time.struct_time) -> int:
    return int(time.mktime(t))


def dt_get_timestamp() -> int:
    return int(datetime.datetime.now().timestamp())


def dt_str_to_datetime(s: str) -> datetime.datetime:
    return datetime.datetime.strptime(s, FORMAT)


def dt_str_to_timestamp(s: str) -> int:
    ts = dt_str_to_datetime(s).timestamp()
    return int(ts)


def dt_to_str(dt: datetime.datetime) -> str:
    return dt.strftime(FORMAT)
