import csv


def getData(FILENAME):
    DATA = []
    c = 0
    with open(FILENAME) as csv_file:
        for line in csv_file:
            DATA.append(list(map(str.strip, line.split(','))))
            c = c + 1

    c=0
    for line in DATA:
        DATA[c].pop(0)
        c = c + 1
    return DATA


def getMeans(DATA):
    MEANS = []
    for line in DATA:
        sum = 0
        count = 0
        for val in line:
            sum = sum + int(val)
            count = count + 1
        MEANS.append(sum/count)
    return MEANS



FILES_IN = ['Reproduction_lim.csv', 'Food_Quality.csv', 'Energy_Burn_Rate.csv', 'Birth_Penalty.csv']
FILES_OUT = ['Reproduction_means.csv', 'FoodQuality_means.csv', 'BurnRate_means.csv', 'BirthPenalty_means.csv']
FILE_MEANS = []

for file in FILES_IN:
    reproduction = getData(file)
    means = getMeans(reproduction)
    FILE_MEANS.append(means)
    
i = 0
for set in FILE_MEANS:
    with open(FILES_OUT[i], 'w') as csvfile:
        csvwriter = csv.writer(csvfile)  
        csvwriter.writerow(set) 
    i = i + 1