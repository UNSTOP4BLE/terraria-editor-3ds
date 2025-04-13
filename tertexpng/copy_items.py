import os
import re
from PIL import Image
from fuzzywuzzy import process

source_dir = "output_sprites"
destination_dir = "items_folder"
item_list_path = "../assets/items.txt"
os.makedirs(destination_dir, exist_ok=True)

item_dict = {}
with open(item_list_path, "r") as f:
    for line in f:
        parts = line.strip().split("=")
        if len(parts) == 2:
            item_id, item_name = parts
            item_dict[item_name.lower().replace(" ", "_")] = item_id

item_files_count = 0
not_found_items = []

def process_filename(filename):
    name_part = filename.split(".")[0]
    name_part = re.sub(r'_(\d+)_\d+$', r'_\1', name_part)
    name_part = 'Item_' + name_part[5:]
    return name_part

for filename in os.listdir(source_dir):
    if "item_" in filename.lower() and filename.endswith(".png"):
        item_files_count += 1
        name_part = process_filename(filename)
        name_lower = name_part.lower().replace("item_", "").replace("_", " ")
        match = process.extractOne(name_lower, item_dict.keys())

        src_path = os.path.join(source_dir, filename)

        if match and match[1] >= 80:
            item_id = item_dict[match[0]]
            new_filename = f"Item_{item_id}.png"
            dst_path = os.path.join(destination_dir, new_filename)
        else:
            dst_path = os.path.join(destination_dir, name_part + ".png")
            if not re.match(r'Item_\d+', name_part):
                not_found_items.append(filename)

        img = Image.open(src_path)
        img = img.resize((img.width * 2, img.height * 2), resample=Image.NEAREST)
        img.save(dst_path)

print(f"{item_files_count} 'item_' files were found and processed.")

if not_found_items:
    print("Warning: The following items were not found in items.txt and were copied as is:")
    for item in not_found_items:
        print(f"  - {item}")

print(f"All 'item_' files have been processed and copied to '{destination_dir}/'")
