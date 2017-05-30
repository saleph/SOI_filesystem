size=10000

program="SOI_filesystem"
fs="vfs"
name="vania2.jpg"

./$program $fs -c $size

./$program $fs -s
./$program $fs -v $name
./$program $fs -s
./$program $fs -l
./$program $fs -i
./$program $fs -r
