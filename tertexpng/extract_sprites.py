import os
from PIL import Image
from PIL import ImageChops

def trim_duplicated_borders(img):
    w, h = img.size

    def get_column(img, x):
        return img.crop((x, 0, x + 1, h))

    def get_row(img, y):
        return img.crop((0, y, w, y + 1))

    left = 0
    right = w
    top = 0
    bottom = h

    if w > 1 and ImageChops.difference(get_column(img, 0), get_column(img, 1)).getbbox() is None:
        left = 1
    if w > 1 and ImageChops.difference(get_column(img, w - 1), get_column(img, w - 2)).getbbox() is None:
        right = w - 1
    if h > 1 and ImageChops.difference(get_row(img, 0), get_row(img, 1)).getbbox() is None:
        top = 1
    if h > 1 and ImageChops.difference(get_row(img, h - 1), get_row(img, h - 2)).getbbox() is None:
        bottom = h - 1

    if right > left and bottom > top:
        return img.crop((left, top, right, bottom))
    else:
        return img

spritesheet_dir = "."
output_dir = "output_sprites"
os.makedirs(output_dir, exist_ok=True)

with open("sprites.txt", "r") as f:
    lines = f.readlines()

spritesheet_cache = {}

for line in lines:
    parts = line.strip().split(",")
    if len(parts) != 6:
        continue

    texname, name, x, y, w, h = parts
    x, y, w, h = map(int, (x, y, w, h))

    if texname not in spritesheet_cache:
        sheet_path = os.path.join(spritesheet_dir, texname)
        if not os.path.isfile(sheet_path):
            continue
        sheet = Image.open(sheet_path).convert("RGBA")
        spritesheet_cache[texname] = sheet

    sheet = spritesheet_cache[texname]
    sprite = sheet.crop((x, y, x + w, y + h))
    trimmed = trim_duplicated_borders(sprite)
    safe_name = f"{name}.png".replace("/", "_").replace(":", "_")
    trimmed.save(os.path.join(output_dir, safe_name))

print(f"Done. Sprites saved in '{output_dir}/'")
