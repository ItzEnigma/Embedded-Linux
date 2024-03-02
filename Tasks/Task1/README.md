``` bash
# Variables Manipulation
x=20
((x+=1))
x=$((x+1*2))

# For In Range Loop Syntax
for i in {1..5}
do
    echo $i
   
    ls
done

# For Loop Syntax
for ((i=0;i<5;i++))
do
    echo $i
done

echo $x
```
