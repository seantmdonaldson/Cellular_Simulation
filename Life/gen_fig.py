import matplotlib.pyplot as plt
import csv
from alive_progress import alive_bar
import numpy as np

PLOTNAMES = ["Energy_Level","Generation_Number","Population_data", "Food_Quality", "Energy_Burn_Rate", "Birth_Penalty", "Reproduction_lim", "Food_data"]

FIGNAMES = ["Energy Level","Generation Number","Population", "Food Quality", "Energy Burn Rate", "Birth Penalty", "Reproduction Limit", "Total Food"]

print("Generating plots...")
fig = plt.figure()
plt.style.use("Solarize_Light2")
pointsize = 50
i = 0
for FILENAME in PLOTNAMES:
    # plotting and saving the figures
    maxy = 0
    maxx = 0
    print(f"Plotting {FILENAME}...")
    row_count = 0
    with open(FILENAME + '.csv', 'r') as FILE:
        csv_reader = csv.reader(FILE, delimiter=',')
        row_count = len(list(csv_reader))
        FILE.close()
    with alive_bar(row_count) as bar:
        with open(FILENAME + '.csv', 'r') as FILE:
            csv_reader = csv.reader(FILE, delimiter=',')
            index = 0
            for row in csv_reader:
                index = index + 1
                count = 0
                cycle = 0
                for point in row:
                    if count == 0:  
                        cycle = point
                    else:
                        # plotting the actual points
                        plt.scatter(int(cycle), int(point),pointsize)
                        if int(cycle) > maxx:
                            maxx = int(cycle)
                        if int(point) > maxy:
                            maxy = int(point)
                    count = count + 1
                bar()
                
        print("saving figure..")
        x_axis = np.linspace(0,maxx,num=4)
        y_axis = np.linspace(0,maxy,num=4)
        plt.xticks(x_axis)
        plt.yticks(y_axis)
        plt.ylabel(FIGNAMES[i])
        plt.xlabel("Time steps")
        plt.savefig(FILENAME)
        plt.close()
        FILE.close()
        i = i + 1
print("plots generated...")