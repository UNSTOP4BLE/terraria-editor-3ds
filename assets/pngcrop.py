import os
from PIL import Image

def crop_transparent(image_path):
    """Crop transparent edges from a PNG image."""
    image = Image.open(image_path)
    if image.mode != "RGBA":
        print(f"Skipping non-RGBA image: {image_path}")
        return None

    bbox = image.getbbox()
    if bbox:
        cropped_image = image.crop(bbox)
        return cropped_image
    return None

def crop_all_pngs(directory):
    """Crop all PNG images in the specified directory."""
    for filename in os.listdir(directory):
        if filename.lower().endswith(".png"):
            file_path = os.path.join(directory, filename)
            cropped_image = crop_transparent(file_path)
            if cropped_image:
                output_path = os.path.join(directory, f"{filename}")
                cropped_image.save(output_path)
                print(f"Cropped and saved: {output_path}")
            else:
                print(f"Failed to crop: {file_path}")

if __name__ == "__main__":
    folder_path = input("Enter the folder path containing PNG images: ")
    crop_all_pngs(folder_path)
