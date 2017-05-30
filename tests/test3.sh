size=10000

program="SOI_filesystem"
fs="vfs"

name1="plik0"
name2="plik1"
name3="plik2"
name4="plik3"

./$program $fs -c $size

./$program $fs -v $name1
./$program $fs -v $name2
./$program $fs -v $name3
./$program $fs -v $name4
./$program $fs -s
./$program $fs -i

./$program $fs -d $name3
./$program $fs -s
./$program $fs -l
./$program $fs -i

./$program $fs -d $name2
./$program $fs -s
./$program $fs -l
./$program $fs -i

./$program $fs -v $name3
./$program $fs -s
./$program $fs -l
./$program $fs -i

./$program $fs -v $name2
./$program $fs -s
./$program $fs -l
./$program $fs -i 

./$program $fs -r

