import sys

menu = {
    "baja taco": 4.00,
    "burrito": 7.50,
    "bowl": 8.50,
    "nachos": 11.00,
    "quesadilla": 8.50,
    "super burrito": 8.50,
    "super quesadilla": 9.50,
    "taco": 3.00,
    "tortilla salad": 8.00,
}

total = 0
while True:
    try:
        user_order = input("Item: ").lower()
    except EOFError:
        break
    if user_order in menu:
        total += menu[user_order]
        print(f"Total: ${total:.2f}")

sys.exit(0)
