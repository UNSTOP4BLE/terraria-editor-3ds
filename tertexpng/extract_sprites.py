import os
from PIL import Image

def trim_duplicated_borders(img):
    w, h = img.size
    px = img.load()

    left = 0
    while left + 1 < w and all(px[left, y] == px[left + 1, y] for y in range(h)):
        left += 1

    right = w - 1
    while right - 1 > left and all(px[right, y] == px[right - 1, y] for y in range(h)):
        right -= 1

    top = 0
    while top + 1 < h and all(px[x, top] == px[x, top + 1] for x in range(w)):
        top += 1

    bottom = h - 1
    while bottom - 1 > top and all(px[x, bottom] == px[x, bottom - 1] for x in range(w)):
        bottom -= 1

    return img.crop((left, top, right + 1, bottom + 1))

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
