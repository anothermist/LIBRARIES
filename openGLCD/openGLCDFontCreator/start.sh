#
# @#$@# gnome doesn't set current directory to
# directory of clicked on program so we have to
# set it ourselves!
#

# First cd to the proper directory
cd `dirname $0`

#now crank up FontCreator
java FontCreator -classpath .
