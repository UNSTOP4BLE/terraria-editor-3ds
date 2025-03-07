from PIL import Image

# Load the font image
font_image = Image.open("font.png").convert("RGBA")

# List of filenames for separated characters
files = [
    "exmark.png", "quotes.png", "hashtag.png", "dollar.png", "percent.png", "and.png", "quote.png",
    "lbracket.png", "rbracket.png", "star.png", "plus.png", "coma.png", "minus.png", "dot.png", "slash.png",
    "0.png", "1.png", "2.png", "3.png", "4.png", "5.png", "6.png", "7.png", "8.png", "9.png", "colon.png",
    "semicolon.png", "less.png", "equal.png", "more.png", "questionm.png", "at.png", "A.png", "B.png", "C.png",
    "D.png", "E.png", "F.png", "G.png", "H.png", "I.png", "J.png", "K.png", "L.png", "M.png", "N.png", "O.png",
    "P.png", "Q.png", "R.png", "S.png", "T.png", "U.png", "V.png", "W.png", "X.png", "Y.png", "Z.png", "sqlbracket.png",
    "backslash.png", "sqrbracket.png", "caret.png", "underscore.png", "backtick.png", "la.png", "lb.png", "lc.png", "ld.png",
    "le.png", "lf.png", "lg.png", "lh.png", "li.png", "lj.png", "lk.png", "ll.png", "lm.png", "ln.png", "lo.png", "lp.png",
    "lq.png", "lr.png", "ls.png", "lt.png", "lu.png", "lv.png", "lw.png", "lx.png", "ly.png", "lz.png", "clbracket.png",
    "bar.png", "crbracket.png", "tilde.png"
]

# Constants
CHAR_HEIGHT = 30
PIXEL_THRESHOLD = 1  # Minimum gap between characters

# Get image dimensions
width, height = font_image.size

# Scan the image horizontally to find character positions
char_positions = []
x_start = None
for x in range(width):
    column = font_image.crop((x, 0, x + 1, CHAR_HEIGHT))
    if column.getbbox():  # Check if the column has any non-transparent pixels
        if x_start is None:
            x_start = x
    else:
        if x_start is not None:
            char_positions.append((x_start, x))
            x_start = None

# Ensure last character is included
if x_start is not None:
    char_positions.append((x_start, width))

# Validate character count
if len(char_positions) != len(files):
    raise ValueError("Mismatch between detected characters (" + str(len(char_positions)) + ") and filenames (" + str(len(files)) + ")")

# Save each character as a separate file
for (i, (x_start, x_end)) in enumerate(char_positions):
    char_image = font_image.crop((x_start, 0, x_end, CHAR_HEIGHT))
    char_image.save("out/" + files[i])

print("Character images saved successfully.")