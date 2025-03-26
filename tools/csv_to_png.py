import numpy as np
import cv2
import argparse
import os
import glob

def csv_to_png(csv_file, png_file, colormap="inferno"):
    """
    Convert a CSV file of temperature values to a colorized PNG image.

    Args:
        csv_file (str): Path to the input CSV file.
        png_file (str): Path to the output PNG file.
        colormap (str): OpenCV colormap name (inferno, jet, plasma, gray).
    """
    # Load CSV file
    data = np.loadtxt(csv_file, delimiter=",")

    # Normalize temperature values (0-255) for image representation
    min_temp, max_temp = np.min(data), np.max(data)
    norm_data = ((data - min_temp) / (max_temp - min_temp) * 255).astype(np.uint8)

    # OpenCV colormap selection
    colormap_dict = {
        "inferno": cv2.COLORMAP_INFERNO,
        "jet": cv2.COLORMAP_JET,
        "plasma": cv2.COLORMAP_PLASMA,
        "gray": cv2.COLORMAP_BONE
    }

    if colormap not in colormap_dict:
        print(f"❌ Invalid colormap '{colormap}'. Using 'inferno' as default.")
        colormap = "inferno"

    # Apply colormap
    colorized_image = cv2.applyColorMap(norm_data, colormap_dict[colormap])

    # Save PNG
    cv2.imwrite(png_file, colorized_image)
    print(f"✅ Saved: {png_file}")

def process_directory(directory, colormap):
    """
    Process all CSV files in a directory and convert them to PNG.

    Args:
        directory (str): Path to the folder containing CSV files.
        colormap (str): Colormap to apply.
    """
    csv_files = glob.glob(os.path.join(directory, "*.csv"))

    if not csv_files:
        print(f"❌ No CSV files found in directory: {directory}")
        return

    print(f"🔍 Found {len(csv_files)} CSV files in {directory}. Converting...")

    for csv_file in csv_files:
        png_file = os.path.splitext(csv_file)[0] + ".png"
        csv_to_png(csv_file, png_file, colormap)

def print_help():
    """Print usage examples."""
    help_text = """
    🛠️ CSV to PNG Converter - Usage Examples:
    
    Convert a single CSV file:
      python csv_to_png.py -f thermal_1.csv
    
    Convert a single CSV file and specify output name:
      python csv_to_png.py -f thermal_1.csv -o output_image.png
    
    Convert all CSV files in a folder:
      python csv_to_png.py -d C:\\path\\to\\folder
    
    Convert using a different colormap (inferno, jet, plasma, gray):
      python csv_to_png.py -f thermal_1.csv -c jet
      python csv_to_png.py -d C:\\thermal_data -c plasma

    Show this help message:
      python csv_to_png.py -help
    """
    print(help_text)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert CSV thermal data to PNG images.", add_help=False)
    parser.add_argument("-f", "--file", type=str, help="Path to a single CSV file.")
    parser.add_argument("-d", "--directory", type=str, help="Path to a folder with multiple CSV files.")
    parser.add_argument("-o", "--output", type=str, help="Path to save the output PNG file (only for single file mode).")
    parser.add_argument("-c", "--colormap", type=str, default="inferno", help="Colormap to use (inferno, jet, plasma, gray).")
    parser.add_argument("-help", action="store_true", help="Show usage examples.")

    args = parser.parse_args()

    if args.help:
        print_help()
    elif args.directory:
        process_directory(args.directory, args.colormap)
    elif args.file:
        png_file = args.output if args.output else os.path.splitext(args.file)[0] + ".png"
        csv_to_png(args.file, png_file, args.colormap)
    else:
        print("❌ Please specify either a file (-f) or a directory (-d). Use -help for examples.")
