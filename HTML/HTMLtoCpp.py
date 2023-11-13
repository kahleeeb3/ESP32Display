# Function to read and print each line of a file
def print_file_lines(file_path):
    try:
        with open(file_path, 'r') as file:
            for line in file:
                # Print the line with leading whitespace and "\n"
                print(f'"{line.rstrip()}\\n"')
    except FileNotFoundError:
        print(f"Error: File not found - {file_path}")
    except Exception as e:
        print(f"An error occurred: {e}")

# Replace 'your_file_path.html' with the path to your HTML file
file_path = './HTML/index.html'
print_file_lines(file_path)
