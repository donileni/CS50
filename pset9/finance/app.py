import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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
    accumulated_stocks = get_user_stocks(session["user_id"])
    prices = get_stock_prices(accumulated_stocks)

    price_for_all_stocks = 0
    counter = 0
    for stock in accumulated_stocks:
        stock["price"] = prices[counter]
        stock["total_price"] = prices[counter] * stock["shares"]
        price_for_all_stocks += float(stock["total_price"])
        counter += 1

    counter = 0
    user_balance = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])
    total_value = float(user_balance[0]["cash"]) + price_for_all_stocks

    return render_template("index.html", accumulated_stocks=accumulated_stocks, user_balance=user_balance, total_value=total_value )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if (request.method == "POST"):
        symbol = request.form.get("symbol")
        string_shares = request.form.get("shares")
        stock = lookup(symbol)

        if string_shares.isdigit():
            shares = int(string_shares)
        else:
            return apology("invalid shares")

        if (symbol == ""):
            return apology("symbol missing")

        if (stock == None):
            return apology("invalid symbol", 400)

        share_price = float(stock["price"])

        user_balance = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])
        int_user_balance = int(user_balance[0]["cash"])
        total_price = shares * share_price

        if (total_price > int_user_balance):
            return apology("can't afford", 400)

        initial_timestamp = str(datetime.now())
        timestamp = initial_timestamp[:-7]
        db.execute("INSERT INTO user_stocks (user_id, stock_symbol, stock_name, price, nr_stocks, timestamp) VALUES (?, ?, ?, ?, ?, ?)", session["user_id"], symbol, stock["name"], share_price, shares, timestamp)

        new_balance = int_user_balance - total_price
        db.execute("UPDATE users SET cash = (?) WHERE id = (?)", new_balance, session["user_id"])

        stocks = db.execute("SELECT * FROM accumulated_stocks")

        if len(stocks) == 0:
            db.execute("INSERT INTO accumulated_stocks (user_id, symbol, name, shares) VALUES (?, ?, ?, ?)", session["user_id"], symbol, stock["name"], shares)
            return redirect("/")

        accumulated_stock = find_stock(stocks, symbol, session["user_id"])

        if accumulated_stock != None:
            current_shares = accumulated_stock["shares"]
            updated_shares = current_shares + shares

            db.execute("UPDATE accumulated_stocks SET shares = (?) WHERE symbol = (?) AND user_id = (?)", updated_shares, accumulated_stock["symbol"], accumulated_stock["user_id"])

        else:
            db.execute("INSERT INTO accumulated_stocks (user_id, symbol, name, shares) VALUES (?, ?, ?, ?)", session["user_id"], symbol, stock["name"], shares)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_transaction_history = db.execute("SELECT * FROM user_stocks WHERE user_id = (?)", session["user_id"])
    print(user_transaction_history)

    return render_template("history.html", user_transaction_history=user_transaction_history)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    if (request.method == "POST"):

        symbol = request.form.get("symbol")
        if (symbol == ""):
            return apology("input a symbol", 400)

        stock = lookup(symbol)
        if (stock == None):
            return apology("invalid symbol", 400)

        return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if (request.method == "POST"):

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        current_usernames = db.execute("SELECT username FROM users")

        for name in current_usernames:
            if name["username"] == username:
                return apology("username already exits", 400)

        if (username == ""):
            return apology("input usernames", 400)

        if (password == "" or confirmation == ""):
            return apology("input password/confirmation", 400)

        if (password != confirmation):
            return apology("Passwords do not match", 400)

        hashed_password = generate_password_hash(password)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashed_password)

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        negative_shares = shares * -1

        if (symbol == None):
            return apology("select symbol")

        stock = lookup(symbol)
        user_stocks = get_user_stocks(session["user_id"])

        if (stock == None):
            return apology("invalid symbol", 400)

        share_price = float(stock["price"])

        current_stock = {}
        for stock in user_stocks:
            if stock["symbol"] == symbol:
                current_stock = stock

        if current_stock["shares"] < shares:
            return apology("not enough shares")

        elif current_stock["shares"] > shares:

            initial_timestamp = str(datetime.now())
            timestamp = initial_timestamp[:-7]
            db.execute("INSERT INTO user_stocks (user_id, stock_symbol, stock_name, price, nr_stocks, timestamp) VALUES (?, ?, ?, ?, ?, ?)", session["user_id"], symbol, current_stock["name"], share_price, negative_shares, timestamp)

            original_shares = int(stock["shares"])
            updated_shares = original_shares - shares
            db.execute("UPDATE accumulated_stocks SET shares = (?) WHERE symbol = (?) AND user_id = (?)", updated_shares, symbol, session["user_id"])

            sales_return = share_price * shares
            balance = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])
            current_balance = int (balance[0]["cash"])
            new_balance = sales_return + current_balance
            db.execute("UPDATE users SET cash = (?) WHERE id = (?)", new_balance, session["user_id"])

        else:
            initial_timestamp = str(datetime.now())
            timestamp = initial_timestamp[:-7]
            db.execute("INSERT INTO user_stocks (user_id, stock_symbol, stock_name, price, nr_stocks, timestamp) VALUES (?, ?, ?, ?, ?, ?)", session["user_id"], symbol, current_stock["name"], share_price, negative_shares, timestamp)

            sales_return = share_price * shares
            balance = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])
            current_balance = int(balance[0]["cash"])
            new_balance = sales_return + current_balance
            db.execute("UPDATE users SET cash = (?) WHERE id = (?)", new_balance, session["user_id"])

            db.execute("DELETE FROM accumulated_stocks WHERE symbol = (?) AND user_id = (?)", symbol, session["user_id"])

        return redirect("/")

    else:
        accumulated_stocks = get_user_stocks(session["user_id"])

        return render_template("sell.html", accumulated_stocks=accumulated_stocks)


def get_user_stocks(user_id):
    user_stocks = db.execute("SELECT * FROM accumulated_stocks WHERE user_id = (?)", user_id)

    return user_stocks


def get_stock_prices(user_stocks):
    prices = []
    for stock in user_stocks:
        current_stock = lookup(stock["symbol"])
        price = current_stock["price"]

        prices.append(price)

    return prices


def find_stock(accumulated_stocks, symbol, user_id):

    for stock in accumulated_stocks:
        stock_symbol = stock["symbol"]
        stock_user_id = stock["user_id"]

        if stock_symbol == symbol and stock_user_id == user_id:
            return stock

    return None