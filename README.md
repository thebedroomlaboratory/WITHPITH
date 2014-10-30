# WITHPITH

## The Arduino-based IR Beam People Counter

Homepage: http://thebedroomlaboratory.com/school-night-session-5-with-pith-3/

### How External Library Subtrees Were Configured

The following was done for each of the 4 external libraries listed below

#### Download parent repository

* git clone https://github.com/thebedroomlaboratory/WITHPITH.git
* cd WITHPITH

#### Setup Subtree

* git remote add -f Arduino-IRremote https://github.com/shirriff/Arduino-IRremote.git
* git checkout -b Arduino-IRremote Arduino-IRremote/master
* git checkout master
* git read-tree --prefix=Arduino/lib/IRremote/ -u Arduino-IRremote
* git merge --squash -s ours --no-commit Arduino-IRremote
* git commit -a -m "added Arduino-IRremote subtree"
* git push

#### Update subtree

* git checkout Arduino-IRremote
* git pull
* git checkout master
* Check to see if directory has been updated:
  * cd Arduino/lib/IRremote
  * git diff-tree -p Arduino-IRremote
* If so, then merge updates
  * git merge --squash -s ours --no-commit Arduino-IRremote

### Arduino Library Requirements (latest versions):

* https://github.com/shirriff/Arduino-IRremote
* https://github.com/rocketscream/Low-Power
* https://github.com/adafruit/Adafruit-GFX-Library
* https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library
