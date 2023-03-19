echo "Hello world"
ls  file1.sh
hostname
Date=$(date)
echo $Date
readonly country="Pakistan"
echo $country

read -p "Your country:" n
echo $n


if [ $country == $n ]
then
   echo "Same"
else
   echo "Not same"
fi


if [ -f file1.sh ]
then
  echo "it exist"
else
  echo "it do not exist"
fi


read choice

case $choice in
a)
 date;;
b)
 ls;;
*)
 echo "Not a valid input"
esac

for i in {1..10}
do
echo "Number: $i"
done

#while [ ]
#do
#done

#until [ ]
#do
#done

#while true
#do
#done

#${0}
