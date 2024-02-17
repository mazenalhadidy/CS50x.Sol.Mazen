# TODO
from cs50 import get_string

text = get_string("Text: ")
numL = 0
numW = len(text.split())
numS = 0

for word in text:
    wordL = len(word)
    if word[wordL - 1] == "." or word[wordL - 1] == "?" or word[wordL - 1] == "!":
        numS = numS + 1
    numLA = len([letter for letter in word if letter.isalpha()])
    numL = numL + numLA

L = 100 * numL / numW
S = 100 * numS / numW
index = 0.0588 * L - 0.296 * S - 15.8
grade = round(index)

if grade < 1:
    print("Before Grade 1")
elif grade > 16:
    print("Grade 16+")
else:
    print(f"Grade: {grade}")
