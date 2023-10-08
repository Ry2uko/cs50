from cs50 import SQL

db = SQL("sqlite:///favorites.db")

favorite = input("Favorite: ")
rows = db.execute("SELECT * FROM favorites WHERE problem = ?", favorite)
for row in rows:
    print(row['Timestamp'])