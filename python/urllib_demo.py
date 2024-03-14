import urllib.request as ur
from urllib.error import URLError
from http.cookiejar import MozillaCookieJar

if __name__ == '__main__':
    cookiejar = MozillaCookieJar()
    # build custom opener
    opener = ur.build_opener(ur.HTTPCookieProcessor(cookiejar))
    # create request obj
    req = ur.Request("http://127.0.0.1:8899", method="GET")
    # set header
    req.add_header("User-Agent", "Test-UserAgent")
    # do request
    try:
        resp = opener.open(req)  # return HTTPResponse obj
    except URLError as err:
        print(err.reason)
        exit(1)
    # output page content
    if resp.status == 200:
        content_len = resp.getheader("Content-Length")
        if content_len is not None and int(content_len):
            print(resp.read(128).decode())
    else:
        print("error, status code:", resp.status)
        exit(1)
    # output cookie
    for k in cookiejar:
        print(k.name, k.value, k.expires)
    # save cookie
    # cookiejar.save("cookie.txt")
