import asyncio
import ipaddress


async def ping_cmd(ip: str):
    cmd = f"ping -c 1 -W 1 {ip}"
    proc = await asyncio.create_subprocess_shell(
        cmd,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE)
    retcode = await proc.wait()
    if retcode == 0:
        print("{:13} ok".format(ip))
    else:
        print("{:13} fail".format(ip))


async def main():
    subnet = ipaddress.IPv4Network("10.0.0.0/29")
    await asyncio.gather(
        *[ping_cmd(h.exploded) for h in subnet.hosts()]
    )

if __name__ == '__main__':
    asyncio.run(main())
