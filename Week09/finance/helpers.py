import csv
import datetime
import pytz
import requests
import subprocess
import urllib
import uuid
import re

from flask import redirect, render_template, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""

    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [
            ("-", "--"),
            (" ", "-"),
            ("_", "__"),
            ("?", "~q"),
            ("%", "~p"),
            ("#", "~h"),
            ("/", "~s"),
            ('"', "''"),
        ]:
            s = s.replace(old, new)
        return s

    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)

    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Prepare API request
    symbol = symbol.upper()
    end = datetime.datetime.now(pytz.timezone("US/Eastern"))
    start = end - datetime.timedelta(days=7)

    # Yahoo Finance API
    url = (
        f"https://query1.finance.yahoo.com/v7/finance/download/{urllib.parse.quote_plus(symbol)}"
        f"?period1={int(start.timestamp())}"
        f"&period2={int(end.timestamp())}"
        f"&interval=1d&events=history&includeAdjustedClose=true"
    )

    # Query API
    try:
        response = requests.get(
            url,
            cookies={"session": str(uuid.uuid4())},
            headers={"User-Agent": "python-requests", "Accept": "*/*"},
        )
        response.raise_for_status()

        # CSV header: Date,Open,High,Low,Close,Adj Close,Volume
        quotes = list(csv.DictReader(response.content.decode("utf-8").splitlines()))
        quotes.reverse()
        price = round(float(quotes[0]["Adj Close"]), 2)
        return {"name": symbol, "price": price, "symbol": symbol}
    except (requests.RequestException, ValueError, KeyError, IndexError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


def validate_transaction(symbol, shares):
    """Validate symbol and shares of '/buy' and '/sell'."""
    apology_text = ""

    if not symbol:
        apology_text = "No Symbol"
    elif not shares:
        apology_text = "No Shares"
    else:
        stock_data = lookup(symbol)
        if stock_data == None:
            apology_text = "No Stock Data Found"

        try:
            shares = int(shares)
        except ValueError:
            apology_text = "Invalid Shares"
        else:
            if shares <= 0:
                apology_text = "Invalid Shares"

    return [not bool(apology_text), apology_text]


def validate_form(username, password, confirmation):
    """Validate registration form."""

    apology_text = ""

    if username == "":
        apology_text = "No Name"
    elif password == "":
        apology_text = "No Password"
    elif confirmation == "":
        apology_text = "Confirm your Password"
    elif password != confirmation:
        apology_text = "Password does not match"
    if apology_text:
        return [False, apology_text]

    # Extra Password Validation
    lowercase_regex = r"[a-z]"
    uppercase_regex = r"[A-Z]"
    special_regex = r"[!@#$%]"
    number_regex = r"[0-9]"

    if len(password) < 8:
        apology_text = "Password must be 8 characters above"
    elif not re.search(lowercase_regex, password):
        apology_text = "Password should contain at least one lowercase character"
    elif not re.search(uppercase_regex, password):
        apology_text = "Password should contain at least one uppercase character"
    elif not re.search(special_regex, password):
        apology_text = "Password should contain at least one special character (!@#$%)"
    elif not re.search(number_regex, password):
        apology_text = "Password should containt atleast one number"
    if apology_text:
        return [False, apology_text]

    return [True, ""]
