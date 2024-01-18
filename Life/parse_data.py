import matplotlib.pyplot as plt
import array as arr
import os
import numpy as np
import re
import subprocess
import pandas as pd
from alive_progress import alive_bar
import csv

# parses data and dumps them into csv files for plotting

# adjustable variable
SIZE = 50
GIF_SPEED = 100 # in milliseconds

# containers
n = 0
m = 0
FILES_INDIR = []
GENERATION_DATA = []
GENERATION_MEMBER = []
CYCLE_NO = []
GEN_NO = []
CONSUMPTION = []
QUALITY = []
RATE = []
PENALTY = []
REPRODUCTION_LIMIT = []
POPULATION_FILES = []
POPULATION_DATA = []
FOODNPOP_CYCLE = []
POPULATION_RESULT = []
FOOD_RESULT = []

def parse_data(CYCLE, LIST, FILENAME):
    # read the data and dump it into its respective file
    # used for all data except food_data and Population_data
    print(FILENAME)
    SLICE = []
    buffer = '0'
    lower_lim = 0
    upper_lim = -1
    cycle_no = 0
    counter = 0
    with open(FILENAME + ".csv", mode='w') as csv_file:
        with alive_bar(len(CYCLE)) as bar:
            for cycle in CYCLE:
                if cycle == buffer:
                    upper_lim = upper_lim + 1
                    pass
                else:
                    rng = 0
                    if lower_lim == 0:
                        upper_lim = upper_lim + 1
                        rng = np.arange(lower_lim, upper_lim,1)
                        upper_lim = upper_lim - 1
                    else:
                        lower_lim = lower_lim + 1
                        upper_lim = upper_lim + 1
                        rng = np.arange(lower_lim, upper_lim,1)
                        lower_lim = lower_lim - 1
                        upper_lim = upper_lim - 1
                    lower_lim = upper_lim
                    buffer = cycle
                    upper_lim = upper_lim + 1
                    for i in rng:
                        SLICE.insert(counter, int(LIST[i]))
                        counter = counter + 1
                    counter = 0
                    index = 0
                    cycle_no = cycle_no + 1
                    csv_file.write(cycle)
                    csv_file.write(',')
                    p = 0
                    for val in SLICE:
                        csv_file.write(str(val))
                        if p != len(SLICE)-1:
                            csv_file.write(',')
                        p = p + 1
                    
                    csv_file.write('\n')
                    SLICE.clear()
                bar()
    csv_file.close()
            

cwd = os.getcwd()
img_data = cwd + "\png"
generation_data = cwd + "/csv"
gameboard_data = cwd + "\dat"

# GETTING COLONY MEMBER DATA
os.chdir(generation_data)
m=0
print("getting colony data list...")
with alive_bar(len(os.listdir(generation_data))) as bar:
    # get all of the generated file names in csv folder
    for file in os.listdir(generation_data):
        if file.endswith(".csv"):
            FILES_INDIR.insert(m,file)
            m = m + 1
        bar()

FILES_INDIR.sort(key=lambda f: int(re.sub('\D', '', f))) # organize file names

n = 0
print("importing colony data...")
with alive_bar(len(FILES_INDIR)) as bar:
    # read data in the csv files in csv folder
    for FILENAME in FILES_INDIR:
        with open(FILENAME, 'r') as FILE:
            for i in FILE:
                csv_dat = pd.read_csv(FILENAME)
                GENERATION_DATA.insert(n,i)
                n = n + 1
        bar()

member = 0
cycle = 0
print("cleaning colony data...")
with alive_bar(len(GENERATION_DATA)) as bar:
    # splitting the data into its respective containers
    for line in GENERATION_DATA:
        string_buffer = ''
        for char in line:
            if char == ',':
                GENERATION_MEMBER.insert(member,string_buffer)
                member = member + 1
                string_buffer = ''
            elif char == '\n':
                GENERATION_MEMBER.insert(member,string_buffer)
                string_buffer = ''
                CYCLE_NO.insert(member, GENERATION_MEMBER[0])
                GEN_NO.insert(member, GENERATION_MEMBER[1])
                CONSUMPTION.insert(member, GENERATION_MEMBER[2])
                QUALITY.insert(member, GENERATION_MEMBER[3])
                RATE.insert(member, GENERATION_MEMBER[4])
                PENALTY.insert(member, GENERATION_MEMBER[5])
                REPRODUCTION_LIMIT.insert(member, GENERATION_MEMBER[6])
                member = member + 1
                GENERATION_MEMBER.clear()
            else:
                string_buffer = string_buffer + char
        bar()
# GETTING FOOD AND POPULATION DATA
m=0
print("getting food and population data files...")
with alive_bar(len(os.listdir(gameboard_data))) as bar:
    # importing food and population data
    for file in os.listdir(gameboard_data):
        if file.endswith(".csv"):
            POPULATION_FILES.insert(m,file)
            m = m + 1
        bar()

POPULATION_FILES.sort(key=lambda f: int(re.sub('\D', '', f))) # reorganize fnp data files 

n = 0
print("importing food and population data...")
os.chdir(gameboard_data)
with alive_bar(len(POPULATION_FILES)) as bar:
    # getting the actual food and population data
    for FILENAME in POPULATION_FILES:
        with open(FILENAME, 'r') as FILE:
            for i in FILE:
                csv_dat = pd.read_csv(FILENAME)
                POPULATION_DATA.insert(n,i)
            n = n + 1
        bar()
os.chdir(cwd)

# slicing the food and population data for plotting
cycle = 0
print("cleaning food and population data...")
with alive_bar(len(POPULATION_DATA)) as bar:
    # separating the data into respective containers
    for line in POPULATION_DATA:
        string_buffer = ''
        for char in line:
            if char == ',':
                FOODNPOP_CYCLE.insert(member,string_buffer)
                member = member + 1
                string_buffer = ''
            elif char == '\n':
                FOODNPOP_CYCLE.insert(member,string_buffer)
                string_buffer = ''
                POPULATION_RESULT.insert(member, int(FOODNPOP_CYCLE[0]))
                FOOD_RESULT.insert(member, int(FOODNPOP_CYCLE[1]))
                member = member + 1
                FOODNPOP_CYCLE.clear()
            else:
                string_buffer = string_buffer + char
        bar()

# getting the data from a list of all numbers and seperating them into their respective cycles
buffer = '0'
lower_lim = 0
upper_lim = 0
counter = 0
cycle_no = 0
DATA_DUMP = [GEN_NO, CONSUMPTION, QUALITY, RATE, PENALTY, REPRODUCTION_LIMIT]
count = 1
# for list in DATA_DUMP:
OUT = [[],[],[],[],[],[]]
i = 0
PLOTNAMES = ["Generation_Number", "Energy_Level","Food_Quality", "Energy_Burn_Rate", "Birth_Penalty", "Reproduction_lim"]
index = 0

num = 0


# writing the parsed data to their own respective files
print("parsing data for plotting...")
for DATA in DATA_DUMP:
    parse_data(CYCLE_NO, DATA, PLOTNAMES[num])
    num = num + 1
    
i = 0
TOTAL_CYCLES = ['0']
no = '0'
for cycle in CYCLE_NO:
    if cycle != no:
        no = cycle
        TOTAL_CYCLES.append(cycle)
    
with open("Population_data.csv", 'w') as FILE:
    for val in POPULATION_RESULT:
        FILE.write(str(TOTAL_CYCLES[i]))
        FILE.write(',')
        FILE.write(str(val))
        FILE.write('\n')
        i = i + 1

i = 0
with open("Food_data.csv", 'w') as FILE:
    for val in FOOD_RESULT:
        FILE.write(str(TOTAL_CYCLES[i]))
        FILE.write(',')
        FILE.write(str(val))
        FILE.write('\n')
        i = i + 1
        
print("Data Parsed...")