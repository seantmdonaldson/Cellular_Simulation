import cv2
import os
from alive_progress import alive_bar
import re

# takes the images in png folder and converts them into .avi video

image_folder = 'png'    
video_name = 'Colony.avi'

print("Generating video...")
images = [img for img in os.listdir(image_folder) if img.endswith(".png")]
images.sort(key=lambda f: int(re.sub('\D', '', f)))
frame = cv2.imread(os.path.join(image_folder, images[0]))
height, width, layers = frame.shape

video = cv2.VideoWriter(video_name, 0, 10, (width,height))

img_num = len(os.listdir(os.getcwd()+ '/png'))
with alive_bar(img_num) as bar:
    for image in images:
        # writing the images to video
        video.write(cv2.imread(os.path.join(image_folder, image)))
        bar()

cv2.destroyAllWindows()
video.release()