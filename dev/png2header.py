from PIL import Image
import sys
import os

#that script converts a png image to a header in include/gfx/

def convert_png_to_h(png_path, output_h, array_name="image_rgb565"):
    img = Image.open(png_path).convert("RGB")

    # rotate for 3DS framebuffer layout
    img = img.transpose(Image.ROTATE_270)

    width, height = img.size

    pixels = []

    for r, g, b in img.getdata():
        pixels.append(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))

    size = len(pixels) * 2

    with open("include/gfx/"+output_h, "w") as f:
        f.write("#pragma once\n")
        f.write("#include <stdint.h>\n")
        f.write("#include \"render.h\"\n\n")

        f.write(f"// Generated from {os.path.basename(png_path)}\n")
        f.write(f"// Size: {width}x{height} (RGB565)\n\n")

        f.write(f"uint16_t {array_name}_buf[] = {{\n    ")

        for i, pixel in enumerate(pixels):
            f.write(f"0x{pixel:04X}, ")

            if (i + 1) % 10 == 0:
                f.write("\n    ")

        f.write("\n};\n\n")

        f.write(f"int {array_name}_height = {width};\n")
        f.write(f"int {array_name}_width = {height};\n\n")
        f.write(f"image_t {array_name} = {{width}, {height}, {array_name}_buf};;\n\n")

    print(f"[OK] Generated {output_h}")
    print(f"     Size: {width}x{height}, {size} bytes")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python png2header.py input.png output.h [array_name]")
        sys.exit(1)

    png = sys.argv[1]
    out = sys.argv[2]
    name = sys.argv[3] if len(sys.argv) > 3 else "image_rgb565"

    convert_png_to_h(png, out, name)