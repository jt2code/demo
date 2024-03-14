
class User:
    def __init__(self) -> None:
        self.__name = ""

    @property
    def name(self):
        return self.__name

    @name.setter
    def name(self, s: str):
        self.__name = s

    @name.deleter
    def name(self):
        self.__name = ""


if __name__ == '__main__':
    ins = User()
    ins.name = "jack"
    print('username: "%s"' % ins.name)  # output: username: "jack"
    del (ins.name)
    print('username: "%s"' % ins.name)  # output: username: ""
