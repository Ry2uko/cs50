import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import (
    apology,
    login_required,
    lookup,
    usd,
    validate_transaction,
    validate_form,
)

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get user data
    user_id = session.get("user_id")
    user_data = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]
    user_cash = round(user_data["cash"], 2)
    user_total = user_cash

    # Get purchases data of user
    user_purchases = db.execute("SELECT * FROM purchases WHERE buyer_id = ?", user_id)

    # Get stock data
    stock_prices = {}
    for purchase in user_purchases:
        user_total += purchase["price"]
        purchase["price"] = round(purchase["price"], 2)

        symbol = purchase["symbol"]
        stock_data = lookup(symbol)

        stock_prices[symbol] = stock_data["price"]

    user_total = round(user_total, 2)
    return render_template(
        "index.html",
        cash=user_cash,
        purchases=user_purchases,
        prices=stock_prices,
        total=user_total,
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol", "")
        shares = request.form.get("shares", "")

        # Validation
        validation = validate_transaction(symbol, shares)
        # [bool, apology_text]
        if validation[0] == False:
            return apology(validation[1])

        stock_data = lookup(symbol)
        symbol = stock_data["symbol"]
        shares = int(shares)

        # Get user data
        shares_amount = stock_data["price"] * shares
        user_id = session.get("user_id")
        user_data = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

        if shares_amount > user_data["cash"]:
            return apology("Not Enough Cash")
        new_user_cash = user_data["cash"] - shares_amount

        # Check if user already has already bought that symbol before
        user_purchases = db.execute(
            "SELECT * FROM purchases WHERE buyer_id = ? AND symbol = ?", user_id, symbol
        )

        if len(user_purchases):
            user_purchases = user_purchases[0]
            new_user_shares = user_purchases["shares"] + shares
            new_user_price = user_purchases["price"] + shares_amount

            db.execute(
                "UPDATE purchases SET shares = ?, price = ? WHERE id = ?",
                new_user_shares,
                new_user_price,
                user_purchases["id"],
            )
        else:
            db.execute(
                "INSERT INTO purchases(buyer_id, symbol, shares, price) VALUES(?, ?, ?, ?)",
                user_id,
                symbol,
                shares,
                shares_amount,
            )

        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_user_cash, user_id)
        db.execute(
            "INSERT INTO history(buyer_id, symbol, transaction_type, shares, price) VALUES(?, ?, ?, ?, ?)",
            user_id,
            symbol,
            "BUY",
            shares,
            shares_amount,
        )

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get history data
    user_id = session.get("user_id")
    user_history = db.execute("SELECT * FROM history WHERE buyer_id = ?", user_id)

    for transaction in user_history:
        transaction["price"] = round(transaction["price"], 2)

    return render_template("history.html", history=user_history)


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Add more cash for the user"""

    if request.method == "POST":
        cash = request.form.get("cash", "")

        if not cash:
            return apology("No Cash")
        try:
            cash = int(cash)
        except ValueError:
            return apology("Invalid Cash Input")

        # Get user data
        user_id = session.get("user_id")
        user_data = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]
        new_user_cash = round(user_data["cash"] + cash, 2)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_user_cash, user_id)

        return redirect("/")
    else:
        return render_template("add_cash.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        symbol = request.form.get("symbol", "")

        if not symbol:
            return apology("No Symbol")

        stock_data = lookup(symbol)
        if stock_data == None:
            return apology("No Stock Data Found")

        name, price, symbol = (
            stock_data["name"],
            stock_data["price"],
            stock_data["symbol"],
        )
        return render_template("quoted.html", name=name, price=price, symbol=symbol)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    if request.method == "POST":
        # Validate form
        username = request.form.get("username", "")
        password = request.form.get("password", "")
        confirmation = request.form.get("confirmation", "")

        validation = validate_form(username, password, confirmation)
        if validation[0] == False:
            return apology(validation[1])

        # Extra Username Validation
        username_found = len(
            db.execute("SELECT username FROM users WHERE username = ?", username)
        )
        if username_found:
            return apology("Username already exists")

        hashed_password = generate_password_hash(confirmation)
        db.execute(
            "INSERT INTO users(username, hash) VALUES (?, ?)", username, hashed_password
        )

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol", "")
        shares = request.form.get("shares", "")

        # Validation
        validation = validate_transaction(symbol, shares)
        if validation[0] == False:
            return apology(validation[1])

        stock_data = lookup(symbol)
        symbol = stock_data["symbol"]
        shares = int(shares)

        # Get user data
        shares_amount = stock_data["price"] * shares
        user_id = session.get("user_id")
        user_data = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

        # Check if user has already bought that symbol
        user_purchases = db.execute(
            "SELECT * FROM purchases WHERE buyer_id = ? AND symbol = ?", user_id, symbol
        )

        if len(user_purchases):
            user_purchases = user_purchases[0]
            user_shares = user_purchases["shares"]
            user_shares_price = user_purchases["price"]

            # Check if user has enough shares to sell
            if shares > user_shares:
                return apology("Not Enough Shares")

            # Update user
            new_user_cash = user_data["cash"] + shares_amount
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_user_cash, user_id)

            # Update purchases
            if shares == user_shares:
                # Delete purchase in DB if shares are 0
                db.execute("DELETE FROM purchases WHERE id = ?", user_purchases["id"])
            else:
                new_user_shares = user_shares - shares
                new_user_price = user_shares_price - shares_amount

                db.execute(
                    "UPDATE purchases SET shares = ?, price = ? WHERE id = ?",
                    new_user_shares,
                    new_user_price,
                    user_purchases["id"],
                )

            db.execute(
                "INSERT INTO history(buyer_id, symbol, transaction_type, shares, price) VALUES(?, ?, ?, ?, ?)",
                user_id,
                symbol,
                "SELL",
                shares,
                shares_amount,
            )

            return redirect("/")
        else:
            return apology("No Shares Of This Symbol")
    else:
        # Get user available stocks
        user_id = session.get("user_id")
        user_purchases = db.execute(
            "SELECT * FROM purchases WHERE buyer_id = ?", user_id
        )
        stock_symbols = []
        for purchase in user_purchases:
            stock_symbols.append(purchase["symbol"])

        return render_template("sell.html", symbols=stock_symbols)
