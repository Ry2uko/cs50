class Jar:
    def __init__(self, capacity=12):
        if capacity < 0:
            raise ValueError("Invalid jar capacity.")

        self._capacity = capacity
        self._size = 0

    def __str__(self):
        return "🍪" * self._size

    def deposit(self, n):
        new_size = self._size + n
        if new_size > self._capacity:
            raise ValueError("Jar capacity exceeded.")

        self._size = new_size

    def withdraw(self, n):
        new_size = self._size - n
        if new_size < 0:
            raise ValueError("No cookies left to take :(")

        self._size = new_size

    @property
    def capacity(self):
        return self._capacity

    @property
    def size(self):
        return self._size


def main():
    jar = Jar(20)
    jar.deposit(15)
    print(str(jar))



main()