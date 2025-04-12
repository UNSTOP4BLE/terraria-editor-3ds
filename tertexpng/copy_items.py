import os
import shutil
import re
from fuzzywuzzy import process

source_dir = "output_sprites"
destination_dir = "items_folder"
item_list_path = "../assets/items.txt"
os.makedirs(destination_dir, exist_ok=True)

# Load item IDs and names from the items.txt file
item_dict = {}
with open(item_list_path, "r") as f:
    for line in f:
        parts = line.strip().split("=")
        if len(parts) == 2:
            item_id, item_name = parts
            item_dict[item_name.lower().replace(" ", "_")] = item_id  # Normalize

item_files_count = 0
not_found_items = []  # Track items that weren't found in items.txt

def process_filename(filename):
    # Remove file extension and normalize the name
    name_part = filename.split(".")[0]
    
    # If the filename contains '_x_<number>', remove the last '_x_<number>'
    name_part = re.sub(r'_(\d+)_\d+$', r'_\1', name_part)
    
    # Ensure the first 'i' in 'item_' is capitalized
    name_part = 'Item_' + name_part[5:]  # 'item_' -> 'Item_'
    
    return name_part

# Loop through each file in the source directory
for filename in os.listdir(source_dir):
    if "item_" in filename.lower() and filename.endswith(".png"):
        item_files_count += 1
        
        # Process the filename to remove _x and capitalize Item_
        name_part = process_filename(filename)
            
        # Fuzzy matching: find the best match from the item dictionary
        name_lower = name_part.lower().replace("item_", "").replace("_", " ")
        match = process.extractOne(name_lower, item_dict.keys())

        if match and match[1] >= 80:  # If a good match is found
            item_id = item_dict[match[0]]
            new_filename = f"Item_{item_id}.png"
            shutil.copy(os.path.join(source_dir, filename), os.path.join(destination_dir, new_filename))
        else:
            # If no match is found, use the processed filename
            shutil.copy(os.path.join(source_dir, filename), os.path.join(destination_dir, name_part + ".png"))
            if not re.match(r'Item_\d+', name_part):
                not_found_items.append(filename)  # Track this item as not found

# Print the total number of files processed
print(f"{item_files_count} 'item_' files were found and processed.")

# Print items that weren't found in items.txt, but exclude those with the format "Item_<number>"
if not_found_items:
    print("Warning: The following items were not found in items.txt and were copied as is:")
    for item in not_found_items:
        print(f"  - {item}")

print(f"All 'item_' files have been processed and copied to '{destination_dir}/'")
