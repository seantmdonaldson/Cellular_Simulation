import subprocess
import time
import os
 
# TODO
# export starting values into json
# take values in cpp script
# average final colony member characteristics 
# export them into json start vals and rerun

# measure dP/dt (population vs time) and try and minimize

# record start time
start = time.time()

print("Deleting old data...")
EXECUTABLE = "Python Delete_data.py" # removes old data for plot gen and video gen
subprocess.run(EXECUTABLE)

print("Running simulation...")
EXECUTABLE = "Life.exe" # main simulation
subprocess.run(EXECUTABLE)

PARSE_SCRIPT = "python parse_data.py" # reads and cleans data for plotting
subprocess.run(PARSE_SCRIPT)

#FIG_SCRIPT = "python gen_fig.py" # generates plots
#subprocess.run(FIG_SCRIPT)

#IMG_SCRIPT = "python gen_imgs.py" # generates images for video
#subprocess.run(IMG_SCRIPT)

#VID_SCRIPT = "python gen_movie.py" # generates video
#subprocess.run(VID_SCRIPT)

end = time.time()
print(f"Process time {end}")
