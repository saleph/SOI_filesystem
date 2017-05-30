size=1000000

program="SOI_filesystem"
fs="vfs"
name1="plik0"
name2="vania.jpg"
name3="vania2.jpg"

name4="temp/$name1"
name5="temp/$name2"
name6="temp/$name3"

echo "===TESTING==="

echo "===Creating file system==="

./$program $fs -c $size
./$program $fs -s

echo "===Copy file test==="

./$program $fs -v $name2
./$program $fs -s 
./$program $fs -l
./$program $fs -i

echo "===Copying additional files==="

./$program $fs -v $name1
./$program $fs -v $name3
./$program $fs -s
./$program $fs -l
./$program $fs -i

echo "===Move original files to temp directory==="

mv $name1 temp/
mv $name2 temp/
mv $name3 temp/

echo "===Retrieve files from virtual disk==="

./$program $fs -m $name1
./$program $fs -m $name2
./$program $fs -m $name3

echo "===Comparing md5sum==="

md5sum $name1 $name4
md5sum $name2 $name5
md5sum $name3 $name6

mv $name4 $name1
mv $name5 $name2
mv $name6 $name3

./$program $fs -r





