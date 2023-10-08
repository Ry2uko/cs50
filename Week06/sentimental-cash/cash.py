from cs50 import get_float


def main():
    while True:
        user_change = get_float("Change owed: ")
        if user_change >= 0:
            break

    coins = get_coins(user_change)
    print(coins)


def get_coins(change):
    coins_amount = 0
    coins = {
        "quarter": 0.25,
        "dime": 0.10,
        "nickel": 0.05,
        "penny": 0.01,
    }

    for coin in coins:
        if change <= 0:
            break

        while change >= coins[coin]:
            change = round(change - coins[coin], 2)
            coins_amount += 1

    return coins_amount


main()
