from flask import Flask, render_template, request

app = Flask(__name__)

GENRE_COLORS = {
    "Horror": "Red",
    "Romance": "Pink",
    "Comedy": "Yellow",
    "Drama": "Violet",
    "Adventure": "Orange",
}

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "GET":
        return render_template("index.html")

    print("Form submitted!")
    genre = request.form.get("genre")

    genre_color = GENRE_COLORS[genre]
    if not genre_color:
        genre_color = "Black"

    return render_template("genre.html", genre=genre, genre_color=genre_color)