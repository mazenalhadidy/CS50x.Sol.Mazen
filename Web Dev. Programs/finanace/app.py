import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, abort
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

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
    person_id = session["user_id"]
    selltrans = db.execute("SELECT stocks, shares FROM STrans WHERE person_id=? GROUP BY stocks ORDER BY stocks ASC", person_id)
    buytrans = db.execute("SELECT stocks, shares FROM BTrans WHERE person_id=? GROUP BY stocks ORDER BY stocks ASC", person_id)
    current = []
    stocking = {}

    for buy in buytrans:
        stocking[buy["stocks"]] = buy["shares"]
    for sell in selltrans:
        if sell["stocks"] in stocking:
            newshare = stocking[sell["stocks"]] - sell["shares"]
            stocking[sell["stocks"]] = newshare
    current = [{"stocks": stock, "shares": share} for stock, share in stocking.items() if share > 0]
    current_sorted = sorted(current, key=lambda x: x["stocks"])

    pricelist = []
    for stock in current_sorted:
        stockdict = lookup(stock["stocks"])
        pricelist.append(stockdict["price"])

    currentbalance = db.execute("SELECT cash FROM users WHERE id=?", person_id)

    stockCount = range(len(current_sorted))
    profit = 0
    for i in stockCount:
        profit = profit + pricelist[i] * current_sorted[i]["shares"]
    return render_template("index.html", current=current_sorted, stockCount=stockCount, pricelist=pricelist,
                           currentbalance=currentbalance, profit=profit)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        date = datetime.now().isoformat(sep=" ")
        symbol = request.form.get("symbol")
        stockinfo = lookup(symbol)
        shares = request.form.get("shares")

        try:
            shares = int(shares)
            if shares <= 0:
                return apology("Shares must be a positive integer", 400)
        except ValueError:
            return apology("Invalid number of shares", 400)

        if stockinfo:
            amount = stockinfo["price"] * shares
            person_id = session["user_id"]
            balance = db.execute("SELECT cash FROM users WHERE id=?", person_id)
            balance = float(balance[0]["cash"])
            newbalance = balance - amount
            if newbalance < 0:
                return apology("We are sorry, but you don't have enough balance to continue this purchase.", 400)

            db.execute("INSERT INTO BTrans (person_id, stocks, shares, price, date) VALUES (?, ?, ?, ?, ?)",
                       person_id, symbol, shares, round(amount, 2), date)
            db.execute("UPDATE users SET cash = ? WHERE id=?", newbalance, person_id)
            flash(f"Purchased {shares} shares of {symbol} for {amount:.2f}")
            return render_template("buy.html", amount=newbalance)
        else:
            return apology("We are sorry, but stock symbol you entered isn't valid, try again with a valid symbol.", 400)
    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    person_id = session["user_id"]
    transactions = db.execute("SELECT ROW_NUMBER() OVER (ORDER BY Date ASC) AS Record, stocks AS Stocks, shares AS Shares, price AS Price, date AS Date, type AS Type FROM (SELECT BTrans.stocks, BTrans.shares, BTrans.price, BTrans.date, BTrans.type FROM BTrans WHERE BTrans.person_id=? UNION ALL SELECT STrans.stocks, STrans.shares, STrans.price, STrans.date, STrans.type FROM STrans WHERE STrans.person_id=?) AS CombinedTable ORDER BY Date DESC", person_id, person_id)

    return render_template("history.html", transactions=transactions)

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
    if request.method == "POST":
        stockcode = request.form.get("symbol")
        if lookup(stockcode) != None:
            stockinfo = lookup(stockcode)
            return render_template("quoted.html", stockinfo=stockinfo)
        else:
            return apology("The Symbol you entered doesn't match any of the stock symbols, please enter a valid symbol.", 400)

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if username and password:
            users = db.execute("SELECT * FROM users")
            for user in users:
                if user["username"] == username:
                    return apology("We are sorry, but your username already exists. please try again with a different username.", 400)
            if (password == confirmation):
                hashing = generate_password_hash(password)
                db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashing)
            else:
                return apology("We are sorry, but the confirmation doesn't match with your password, try again.", 400)
        else:
            return apology("We are sorry, but either your username or password or both are blanks, please fill them and try again.", 400)

        return redirect("/login")
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    person_id = session["user_id"]
    selltrans = db.execute("SELECT stocks, shares FROM STrans WHERE person_id=? GROUP BY stocks ORDER BY stocks ASC", person_id)
    buytrans = db.execute("SELECT stocks, shares FROM BTrans WHERE person_id=? GROUP BY stocks ORDER BY stocks ASC", person_id)
    current = []
    stocking = {}

    for buy in buytrans:
        stocking[buy["stocks"]] = buy["shares"]

    for sell in selltrans:
        if sell["stocks"] in stocking:
            newshare = stocking[sell["stocks"]] - sell["shares"]
            stocking[sell["stocks"]] = newshare

    current = [{"stocks": stock, "shares": share} for stock, share in stocking.items() if share > 0]
    current_sorted = sorted(current, key=lambda x: x["stocks"])
    if request.method == "POST":
        symbol = request.form.get("symbol")
        date = datetime.now().isoformat(sep=" ")
        person_id = session["user_id"]
        shares = request.form.get("shares")

        try:
            shares = int(shares)
            if shares <= 0:
                return apology("Shares must be a positive integer", 400)
        except ValueError:
            return apology("Invalid number of shares", 400)

        if not lookup(symbol):
            return apology("Invalid stock symbol", 400)

        stocks = db.execute("SELECT stocks FROM BTrans WHERE person_id = ? GROUP BY stocks ORDER BY stocks ASC", person_id)

        flag = 0
        for stock in stocks:
            if stock["stocks"] == symbol:
                flag = 1
        if flag == 1:
            currentShares = db.execute("SELECT SUM(shares) AS sum FROM BTrans WHERE person_id=? AND stocks=?", person_id, symbol)
            if shares <= currentShares[0]["sum"]:
                stockPrice = lookup(symbol)["price"]
                amountSold = stockPrice * shares
                db.execute("INSERT INTO STrans (person_id, stocks, shares, price, date) VALUES (?, ?, ?, ?, ?)",
                           person_id, symbol, shares, amountSold, date)
                oldbalance = db.execute("SELECT cash FROM users WHERE id=?", person_id)
                newbalance = oldbalance[0]["cash"] + amountSold
                db.execute("UPDATE users SET cash=? WHERE id=?", newbalance, person_id)
                flash(f"Sold {shares} shares of {symbol} for {amountSold:.2f}")

                return render_template("buy.html", amount=newbalance, current=current_sorted)

            else:
                return apology("we are sorry, but you don't have that much shares to sell, try again.", 400)
        else:
            return apology("we are sorry, but you didn't enter a valid stock, try again.", 400)
    else:
        return render_template("sell.html", current=current_sorted)

@app.route("/changepass", methods=["POST", "GET"])
@login_required
def changepass():
    if request.method == "POST":
        person_id = session["user_id"]

        oldpass = request.form.get("oldpass")

        rows = db.execute("SELECT * FROM users WHERE id = ?", person_id)

        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], oldpass):
            return apology("invalid password", 400)

        newpass = request.form.get("newpass")
        confirm = request.form.get("confirmation")

        if newpass != confirm:
            return apology("Passwords don't match, try again.", 400)

        newpasshash = generate_password_hash(newpass)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", newpasshash, person_id)
        flash("Password Changed Successfully!")

        return redirect("/")
    else:
        return render_template("changepass.html")

@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "POST":
        amount = float(request.form.get("amount"))
        password = request.form.get("password")
        person_id = session["user_id"]
        rows = db.execute("SELECT * FROM users WHERE id = ?", person_id)

        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid password", 400)

        currentbalance = db.execute("SELECT cash FROM users WHERE id = ?", person_id)
        newbalance = currentbalance[0]["cash"] + amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newbalance, person_id)
        flash("Amount added Successfully!")

        return redirect("/")
    else:
        person_id = session["user_id"]
        currentbalance = db.execute("SELECT cash FROM users WHERE id = ?", person_id)

        return render_template("addcash.html", current=currentbalance)


