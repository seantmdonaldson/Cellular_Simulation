import os
from alive_progress import alive_bar
dir = ["/csv", "/dat"]

# removes files in csv and dat folders
cwd = os.getcwd()
for folder in dir:
    os.chdir(cwd+folder)
    files = os.listdir()
    print(f"Removing files in {folder}...")
    with alive_bar(len(files)) as bar:
        for file in files:
            if os.path.isfile(file):
                os.remove(file)
            bar()