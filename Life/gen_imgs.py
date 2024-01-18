import matplotlib.pyplot as plt
import array as arr
import os
import numpy as np
import imageio
import re
import subprocess
from alive_progress import alive_bar
from matplotlib.lines import Line2D
import matplotlib.cm as cm

# takes the data from the .dat files in dat folder and converts them into figure for gen_movie.py script

# adjustable variable
SIZE = 25
GIF_SPEED = 100 # in milliseconds

n = 0
m = 0
FILES_PRESENT = []
DATA_INPUT = [] # 
DATA_LINE = []
DATA_MAP = [] # holds one iteration of the map
DATA_DUMP = [] # holds all data ind a 3 dimension matrix
cwd = os.getcwd()
gameboard_data = cwd + "\dat"
img_data = cwd + "\png"

# get filenames in local dir
for file in os.listdir(gameboard_data):
    if file.endswith(".dat"):
        FILES_PRESENT.insert(m,file)
        m = m + 1
        
#remove old images
os.chdir(img_data)
for file in os.listdir(img_data):
    if file.endswith(".png"):
        os.remove(file)
os.chdir(cwd)


# reorder FILES_PRESENT list
FILES_PRESENT.sort(key=lambda f: int(re.sub('\D', '', f)))

# import data from all of the files
os.chdir(gameboard_data)
for FILENAME in FILES_PRESENT:
    with open(FILENAME, 'r') as FILE:
        for i in FILE:
            DATA_INPUT.insert(n,i)
            n = n + 1

j = 0
w = 0
for line in DATA_INPUT:
    DATA_BUFFER = []
    i = 0
    for char in line:
        if char != '\n':
            DATA_BUFFER.insert(i, char)
            i = i + 1
    DATA_INPUT[j] = DATA_BUFFER
    j = j + 1


#plot the data to images
y = 1
map_count = 0
print("generating images...")
os.chdir(img_data)
pt = 50

# generation IDs, used for colormapping to visually observe the change in generations
GEN_ID = ['A','B','C','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z']
colors = cm.hsv(np.linspace(0, 1, len(GEN_ID)))

with alive_bar(len(DATA_INPUT)) as bar:
    # figure generation
    c = np.random.randint(0,1)
    for line in DATA_INPUT:
        x = 1
        for char in line:
            if str.isdigit(char):
                if int(char) > 6:
                    plt.scatter(x,y,pt,color='r',marker='+')
                elif int(char) > 4:
                    plt.scatter(x,y,pt,color='m',marker='+')
                elif int(char) > 1:
                    plt.scatter(x,y,pt,color='b',marker='+')
                elif int(char) == 1:
                    plt.scatter(x,y,pt,color='g',marker='+')
                else:
                    pass
            else:
                if char == 'D':
                    plt.scatter(x,y,pt,color='black',marker='o')
                else:
                    i = 0
                    for val in GEN_ID:
                        if char == val:
                            plt.scatter(x,y,pt,color=colors[i],marker='o')
                        i = i + 1
                
                
            
            x = x + 1
        y = y + 1
        if y == SIZE+1:
            y = 1
            plt_name = "colony_img_" + str(map_count) + ".png"
            plt.xlim((0,SIZE+1))
            plt.ylim((0,SIZE+1))
            plt.style.use("Solarize_Light2")
            plt.title("Single Cell Simulation")
            handles, labels = plt.gca().get_legend_handles_labels()
            live = Line2D([0], [0],label='Living cell', color='r',marker='o',linestyle='')
            dead = Line2D([0], [0],label='Dying cell', color='black',marker='o',linestyle='')
            food1 = Line2D([0], [0],label='Food: [1]', color='g',marker='+',linestyle='')
            food2 = Line2D([0], [0],label='Food: [2:4]', color='b',marker='+',linestyle='')
            food3 = Line2D([0], [0],label='Food: [5:6]', color='m',marker='+',linestyle='')
            food4 = Line2D([0], [0],label='Food: [7:9]', color='r',marker='+',linestyle='')
            handles.extend([live, dead, food1, food2, food3, food4])
            plt.legend(handles=handles,loc='center right', bbox_to_anchor=(1.35, .5))
            plt.savefig(plt_name, bbox_inches='tight')
            plt.close()
            map_count = map_count + 1
        bar()
print("images generated...")
        