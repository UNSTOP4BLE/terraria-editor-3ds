import os
import re

source_dir = "items_folder"

valid_filename_pattern = re.compile(r"^item_\d+(_\d+)*\.png$", re.IGNORECASE)

invalid_files = []

for filename in os.listdir(source_dir):
    if not valid_filename_pattern.match(filename):
        invalid_files.append(filename)
    else:
        new_filename = filename.lower()
        new_filename = new_filename.replace("item_", "Item_", 1)

        if new_filename.count('_') > 1:
            parts = new_filename.split('_')
            new_filename = '_'.join(parts[:-1]) + '.png'

            os.rename(os.path.join(source_dir, filename), os.path.join(source_dir, new_filename))
            print(f"Renamed {filename} to {new_filename}")
        else:
            if filename != new_filename:
                os.rename(os.path.join(source_dir, filename), os.path.join(source_dir, new_filename))
                print(f"Renamed {filename} to {new_filename}")

if invalid_files:
    print(f"The following files do not match the 'item_x' or 'item_x_x' format:")
    for file in invalid_files:
        print(f"  - {file}")
else:
    print("All files are correctly named in the 'item_x' or 'item_x_x' format.")
