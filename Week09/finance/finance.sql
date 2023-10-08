CREATE TABLE purchases (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    buyer_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL,
    price NUMERIC NOT NULL,
    FOREIGN KEY(buyer_id) REFERENCES users(id)
);

CREATE TABLE history (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    buyer_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    transaction_type TEXT NOT NULL,
    shares INTEGER NOT NULL,
    price NUMERIC NOT NULL,
    TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(buyer_id) REFERENCES users(id)
)