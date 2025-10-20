import os
import argparse

parser = argparse.ArgumentParser(description="Docker Demo Script")
parser.add_argument("--name", default="World", help="Name to greet")
parser.add_argument("--file", default="/data_example/greeting.txt", help="File to save greeting")
args = parser.parse_args()

with open(args.file, "w") as f:
   f.write(f"Hello, {args.name}!\n")

print("Files in /app:")
print(os.listdir("/app"))

print("\nFiles in /data_example:")
print(os.listdir("/data_example"))

print(f"\nGreeting: Hello, {args.name}!")
print(f"Greeting saved to: {args.file}")
