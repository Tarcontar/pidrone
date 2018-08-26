import os, zipfile
import shutil

gerber = 'C:\\Users\\micha\\Desktop\\Drohne_Gerber.zip'
drill = 'C:\\Users\\micha\\Desktop\\Drohne_NC_Drill.zip'
dest = 'C:\\Users\\micha\\Desktop\\Aisler'

zip = zipfile.ZipFile(gerber)
zip.extractall(dest)
zip = zipfile.ZipFile(drill)
zip.extractall(dest)

shutil.make_archive(dest, 'zip', dest)

os.remove(gerber)
os.remove(drill)
os.remove(dest)