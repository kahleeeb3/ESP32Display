def print_file_lines(file_path):
    try:
        with open(file_path, 'r') as file:
            for line in file:
                # Replace double quotes with escaped double quotes
                modified_line = line.replace('"', '\\"')
                # Print the modified line with leading whitespace and "\n"
                print(f'"{modified_line.rstrip()}\\n"')
    except FileNotFoundError:
        print(f"Error: File not found - {file_path}")
    except Exception as e:
        print(f"An error occurred: {e}")

# Replace 'your_file_path.html' with the path to your HTML file
file_path = './HTML/index.html'
print_file_lines(file_path)