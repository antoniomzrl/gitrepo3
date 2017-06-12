

#for f in `find . -name "*.hg"`
for f in */*.hg
do
  echo $f
  f2=`echo $f | tr -d / | sed s/\.hg/\.txt/g`
  mv $f $f2
done
